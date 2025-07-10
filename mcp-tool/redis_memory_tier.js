/**
 * Redis Memory Tier System
 * Implements the three-tier memory architecture (STM, MTM, LTM) using Redis
 */

import { createClient } from 'redis';
import { promisify } from 'util';
import { gzip, gunzip } from 'zlib';
import ContentTypeHandler from './content_type_handler.js';

// Promisify zlib functions
const gzipAsync = promisify(gzip);
const gunzipAsync = promisify(gunzip);

/**
 * RedisMemoryTier class
 * 
 * Provides a unified interface to the three-tier memory system with:
 * - STM (Short-Term Memory): Volatile pattern storage with 1-hour TTL
 * - MTM (Medium-Term Memory): Semi-persistent storage with 7-day TTL
 * - LTM (Long-Term Memory): Persistent storage with compression and no expiry
 */
class RedisMemoryTier {
    constructor(options = {}) {
        this.options = {
            host: options.host || '127.0.0.1',
            port: options.port || 6379,
            socket: options.socket,
            prefix: options.prefix || 'sep:', // Use exact prefix without adding memory:
            coherenceThresholds: {
                minimum: options.coherenceThresholds?.minimum || 0.1,
                mtm_promotion: options.coherenceThresholds?.mtm_promotion || 0.7,
                ltm_promotion: options.coherenceThresholds?.ltm_promotion || 0.9,
                demotion: options.coherenceThresholds?.demotion || 0.3
            },
            stabilityThresholds: {
                mtm_promotion: options.stabilityThresholds?.mtm_promotion || 0.6,
                ltm_promotion: options.stabilityThresholds?.ltm_promotion || 0.85
            },
            generationRequirements: {
                mtm_promotion: options.generationRequirements?.mtm_promotion || 5,
                ltm_promotion: options.generationRequirements?.ltm_promotion || 100
            },
            poolSizes: {
                stm: options.poolSizes?.stm || 1000,
                mtm: options.poolSizes?.mtm || 10000,
                ltm: options.poolSizes?.ltm || 100000
            },
            fragmentationThreshold: options.fragmentationThreshold || 0.3
        };

        // TTL settings for different memory tiers (in seconds)
        this.ttlSettings = {
            STM: 60 * 60,      // 1 hour
            MTM: 7 * 24 * 60 * 60, // 7 days
            LTM: -1            // No expiry
        };

        // Redis client instance
        this.client = null;
        
        // Content type handler
        this.contentTypeHandler = new ContentTypeHandler();
        
        // Key prefixes for different memory tiers and metadata
        this.keyPrefixes = {
            pattern: `${this.options.prefix}pattern:`,
            tier: {
                STM: `${this.options.prefix}tier:stm`,
                MTM: `${this.options.prefix}tier:mtm`,
                LTM: `${this.options.prefix}tier:ltm`
            },
            metadata: `${this.options.prefix}metadata:`,
            stats: `${this.options.prefix}stats`,
            fragmentation: `${this.options.prefix}fragmentation`,
            pool: `${this.options.prefix}pool:`
        };
        
        // Initialization state
        this.initialized = false;
    }

    async initialize() {
        if (this.initialized) {
            return;
        }

        try {
            console.error('RedisMemoryTier: Initializing with options:', {
                host: this.options.host,
                port: this.options.port,
                socket: this.options.socket || process.env.SEP_REDIS_SOCKET,
                prefix: this.options.prefix
            });
            
            // Create Redis client with increased timeout and retry logic
            const socketPath = this.options.socket || process.env.SEP_REDIS_SOCKET;
            console.error('RedisMemoryTier: Using connection:', socketPath ? `Unix socket: ${socketPath}` : `TCP: ${this.options.host}:${this.options.port}`);
            
            try {
                const redisOptions = {
                    socket: socketPath ? {
                        path: socketPath,
                        connectTimeout: 30000, // Increased timeout to 30 seconds
                        reconnectStrategy: (retries) => {
                            console.error(`RedisMemoryTier: Retry attempt ${retries}`);
                            if (retries > 5) {
                                console.error('RedisMemoryTier: Max retries reached');
                                return new Error('Max retries reached');
                            }
                            return Math.min(retries * 100, 3000); // Exponential backoff with max 3s
                        }
                    } : {
                        host: this.options.host,
                        port: this.options.port,
                        connectTimeout: 30000, // Increased timeout to 30 seconds
                        reconnectStrategy: (retries) => {
                            console.error(`RedisMemoryTier: Retry attempt ${retries}`);
                            if (retries > 5) {
                                console.error('RedisMemoryTier: Max retries reached');
                                return new Error('Max retries reached');
                            }
                            return Math.min(retries * 100, 3000); // Exponential backoff with max 3s
                        }
                    }
                };
                
                console.error('RedisMemoryTier: Creating Redis client with options:', JSON.stringify(redisOptions));
                this.client = createClient(redisOptions);
                console.error('RedisMemoryTier: Redis client created successfully');
            } catch (clientError) {
                console.error('RedisMemoryTier: Failed to create Redis client:', clientError);
                throw clientError;
            }

            // Add error handler with more detailed logging
            this.client.on('error', (err) => {
                console.error('RedisMemoryTier error:', {
                    error: err.message,
                    code: err.code,
                    command: err.command,
                    stack: err.stack
                });
            });

            // Add connection status logging
            this.client.on('connect', () => {
                console.error('RedisMemoryTier: Connecting...');
            });

            this.client.on('ready', () => {
                console.error('RedisMemoryTier: Connection ready');
            });

            this.client.on('reconnecting', () => {
                console.error('RedisMemoryTier: Reconnecting...');
            });

            this.client.on('end', () => {
                console.error('RedisMemoryTier: Connection ended');
            });

            // Connect to Redis with timeout
            console.error('RedisMemoryTier: Attempting to connect to Redis...');
            try {
                // Use Promise.race to implement a connection timeout
                await Promise.race([
                    this.client.connect(),
                    new Promise((_, reject) =>
                        setTimeout(() => reject(new Error('Redis connection timeout after 30s')), 30000)
                    )
                ]);
                console.error('RedisMemoryTier: Successfully connected to Redis');
                
                // Test connection with PING
                const pingResult = await this.client.ping();
                console.error(`RedisMemoryTier: Redis PING result: ${pingResult}`);
            } catch (connectError) {
                console.error('RedisMemoryTier: Failed to connect to Redis:', connectError);
                throw connectError;
            }
            
            // Store configuration in Redis
            await this.client.hSet(`${this.options.prefix}config`, {
                coherence_minimum: this.options.coherenceThresholds.minimum,
                coherence_mtm_promotion: this.options.coherenceThresholds.mtm_promotion,
                coherence_ltm_promotion: this.options.coherenceThresholds.ltm_promotion,
                coherence_demotion: this.options.coherenceThresholds.demotion,
                stability_mtm_promotion: this.options.stabilityThresholds.mtm_promotion,
                stability_ltm_promotion: this.options.stabilityThresholds.ltm_promotion,
                generations_mtm_promotion: this.options.generationRequirements.mtm_promotion,
                generations_ltm_promotion: this.options.generationRequirements.ltm_promotion,
                fragmentation_threshold: this.options.fragmentationThreshold
            });
            
            // Initialize pool size counters
            await this.client.hSet(`${this.keyPrefixes.pool}size`, {
                STM: 0,
                MTM: 0,
                LTM: 0
            });
            
            // Initialize fragmentation tracking
            await this.client.hSet(this.keyPrefixes.fragmentation, {
                STM: 0,
                MTM: 0,
                LTM: 0
            });
            
            this.initialized = true;
            console.log('Redis Memory Tier initialized successfully');
        } catch (error) {
            console.error('Failed to initialize Redis Memory Tier:', error);
            throw error;
        }
    }

    async close() {
        if (this.client) {
            await this.client.quit();
            this.initialized = false;
            console.log('Redis Memory Tier connection closed');
        }
    }

    sep_getPatternKey(patternId) {
        return `${this.keyPrefixes.pattern}${patternId}`;
    }

    sep_getMetadataKey(patternId) {
        return `${this.keyPrefixes.metadata}${patternId}`;
    }

    sep_determineMemoryTier(coherence, stability, generations) {
        // LTM requirements: high coherence, stability, and sufficient generations
        if (coherence >= this.options.coherenceThresholds.ltm_promotion &&
            stability >= this.options.stabilityThresholds.ltm_promotion &&
            generations >= this.options.generationRequirements.ltm_promotion) {
            return 'LTM';
        }
        
        // MTM requirements: medium coherence, stability, and generations
        if (coherence >= this.options.coherenceThresholds.mtm_promotion &&
            stability >= this.options.stabilityThresholds.mtm_promotion &&
            generations >= this.options.generationRequirements.mtm_promotion) {
            return 'MTM';
        }
        
        // Default to STM
        return 'STM';
    }

    async storePattern(patternId, patternData) {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            // Detect content type if not provided
            const contentType = patternData.contentType || 
                this.contentTypeHandler.detectContentType(patternData.data);
            
            // Get appropriate handler
            const handler = this.contentTypeHandler.getHandler(contentType);
            
            // Calculate content-specific coherence if not provided
            const contentCoherence = patternData.coherence || 
                handler.calculateCoherence(patternData.data);

            // Ensure patternData has required fields
            const metadata = {
                id: patternId,
                contentType,
                coherence: contentCoherence,
                stability: patternData.stability || 0.5,
                generations: patternData.generations || 1,
                mutation_rate: patternData.mutation_rate || 0.0,
                size: patternData.data ? JSON.stringify(patternData.data).length : 0,
                created_at: patternData.created_at || new Date().toISOString(),
                last_updated: new Date().toISOString(),
                access_count: patternData.access_count || 0
            };

            // Determine memory tier
            const tier = patternData.tier ||
                this.sep_determineMemoryTier(metadata.coherence, metadata.stability, metadata.generations);
            metadata.tier = tier;

            // Prepare pattern data for storage using content type handler
            let dataToStore = handler.serialize(patternData.data);
            
            // Compress data for LTM tier
            if (tier === 'LTM' && dataToStore) {
                try {
                    const compressedData = await gzipAsync(Buffer.from(dataToStore));
                    dataToStore = compressedData.toString('base64');
                    metadata.compressed = true;
                    metadata.original_size = dataToStore.length;
                    metadata.compressed_size = dataToStore.length;
                } catch (compressionError) {
                    console.error(`Failed to compress pattern ${patternId}:`, compressionError);
                    metadata.compressed = false;
                }
            } else {
                metadata.compressed = false;
            }

            // Start a transaction
            const multi = this.client.multi();
            
            // Store pattern data
            if (dataToStore) {
                multi.set(this.sep_getPatternKey(patternId), dataToStore);
            }
            
            // Store pattern metadata - ensure all values are strings
            const serializedMetadata = Object.fromEntries(
                Object.entries(metadata).map(([key, value]) => [
                    key,
                    typeof value === 'boolean' ? value.toString() : String(value)
                ])
            );
            multi.hSet(this.sep_getMetadataKey(patternId), serializedMetadata);
            
            // Add to appropriate tier sorted set with coherence as score
            multi.zAdd(this.keyPrefixes.tier[tier], {
                score: metadata.coherence,
                value: patternId
            });
            
            // Set TTL based on tier
            if (this.ttlSettings[tier] !== -1) {
                multi.expire(this.sep_getPatternKey(patternId), this.ttlSettings[tier]);
                multi.expire(this.sep_getMetadataKey(patternId), this.ttlSettings[tier]);
            }
            
            // Update pool size counter
            multi.hIncrBy(`${this.keyPrefixes.pool}size`, tier, 1);
            
            // Execute transaction
            await multi.exec();
            
            // Check if pool size exceeds maximum and trigger cleanup if needed
            await this.sep_checkPoolSize(tier);
            
            return metadata;
        } catch (error) {
            console.error(`Failed to store pattern ${patternId}:`, error);
            throw error;
        }
    }

    async getPattern(patternId) {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            // Get pattern metadata
            const metadataKey = this.sep_getMetadataKey(patternId);
            const metadata = await this.client.hGetAll(metadataKey);
            
            if (Object.keys(metadata).length === 0) {
                return null;
            }
            
            // Convert numeric fields
            if (metadata.coherence) metadata.coherence = parseFloat(metadata.coherence);
            if (metadata.stability) metadata.stability = parseFloat(metadata.stability);
            if (metadata.generations) metadata.generations = parseInt(metadata.generations, 10);
            if (metadata.mutation_rate) metadata.mutation_rate = parseFloat(metadata.mutation_rate);
            if (metadata.access_count) metadata.access_count = parseInt(metadata.access_count, 10);
            if (metadata.size) metadata.size = parseInt(metadata.size, 10);
            if (metadata.original_size) metadata.original_size = parseInt(metadata.original_size, 10);
            if (metadata.compressed_size) metadata.compressed_size = parseInt(metadata.compressed_size, 10);
            metadata.compressed = metadata.compressed === 'true';
            
            // Get pattern data
            const patternKey = this.sep_getPatternKey(patternId);
            const rawData = await this.client.get(patternKey);
            
            let data = null;
            if (rawData) {
                // Get content type handler
                const handler = this.contentTypeHandler.getHandler(metadata.contentType);
                
                // Decompress data if it's compressed (LTM tier)
                if (metadata.compressed) {
                    try {
                        const compressedData = Buffer.from(rawData, 'base64');
                        const decompressedData = await gunzipAsync(compressedData);
                        data = handler.deserialize(decompressedData.toString());
                    } catch (decompressionError) {
                        console.error(`Failed to decompress pattern ${patternId}:`, decompressionError);
                        data = null;
                    }
                } else {
                    try {
                        data = handler.deserialize(rawData);
                    } catch (parseError) {
                        console.error(`Failed to parse pattern data ${patternId}:`, parseError);
                        data = null;
                    }
                }
            }
            
            // Update access count and last accessed timestamp
            await this.client.hSet(metadataKey, {
                access_count: (metadata.access_count || 0) + 1,
                last_accessed: new Date().toISOString()
            });
            
            // Reset TTL if the pattern is in STM or MTM
            if (metadata.tier !== 'LTM') {
                await this.client.expire(patternKey, this.ttlSettings[metadata.tier]);
                await this.client.expire(metadataKey, this.ttlSettings[metadata.tier]);
            }
            
            return {
                metadata,
                data
            };
        } catch (error) {
            console.error(`Failed to get pattern ${patternId}:`, error);
            throw error;
        }
    }

    async sep_checkPoolSize(tier) {
        try {
            // Get current pool size
            const poolSizes = await this.client.hGetAll(`${this.keyPrefixes.pool}size`);
            const currentSize = parseInt(poolSizes[tier] || '0', 10);
            const maxSize = this.options.poolSizes[tier.toLowerCase()];
            
            // If pool size exceeds maximum, trigger cleanup
            if (currentSize > maxSize) {
                console.log(`${tier} pool size (${currentSize}) exceeds maximum (${maxSize}). Triggering cleanup...`);
                await this.sep_cleanupTier(tier, currentSize - maxSize);
            }
            
            // Check fragmentation
            await this.sep_checkFragmentation(tier);
        } catch (error) {
            console.error(`Failed to check pool size for ${tier}:`, error);
        }
    }

    async sep_updateFragmentation(tier) {
        try {
            // Get total patterns in tier
            const totalPatterns = await this.client.zCard(this.keyPrefixes.tier[tier]);
            
            // Get pool size
            const poolSizes = await this.client.hGetAll(`${this.keyPrefixes.pool}size`);
            const poolSize = parseInt(poolSizes[tier] || '0', 10);
            
            // Calculate fragmentation
            const fragmentation = totalPatterns > 0 ? 1 - (poolSize / totalPatterns) : 0;
            
            // Update fragmentation counter
            await this.client.hSet(this.keyPrefixes.fragmentation, tier, fragmentation.toString());
            
            console.log(`${tier} fragmentation: ${(fragmentation * 100).toFixed(2)}%`);
        } catch (error) {
            console.error(`Failed to update fragmentation for ${tier}:`, error);
        }
    }

    async sep_checkFragmentation(tier) {
        try {
            // Get fragmentation
            const fragmentationData = await this.client.hGetAll(this.keyPrefixes.fragmentation);
            const fragmentation = parseFloat(fragmentationData[tier] || '0');
            
            // If fragmentation exceeds threshold, trigger defragmentation
            if (fragmentation > this.options.fragmentationThreshold) {
                console.log(`${tier} fragmentation (${(fragmentation * 100).toFixed(2)}%) exceeds threshold (${(this.options.fragmentationThreshold * 100).toFixed(2)}%). Triggering defragmentation...`);
                await this.sep_defragmentTier(tier);
            }
        } catch (error) {
            console.error(`Failed to check fragmentation for ${tier}:`, error);
        }
    }

    async sep_defragmentTier(tier) {
        try {
            // Get all patterns in tier
            const patterns = await this.client.zRange(this.keyPrefixes.tier[tier], 0, -1);
            
            // Count valid patterns
            let validCount = 0;
            
            // Check each pattern
            for (const patternId of patterns) {
                const metadataKey = this.sep_getMetadataKey(patternId);
                const patternKey = this.sep_getPatternKey(patternId);
                
                // Check if pattern data and metadata exist
                const [metadataExists, dataExists] = await Promise.all([
                    this.client.exists(metadataKey),
                    this.client.exists(patternKey)
                ]);
                
                if (metadataExists && dataExists) {
                    validCount++;
                } else {
                    // Remove invalid pattern from tier
                    await this.client.zRem(this.keyPrefixes.tier[tier], patternId);
                    
                    // Clean up any remaining keys
                    if (metadataExists) {
                        await this.client.del(metadataKey);
                    }
                    
                    if (dataExists) {
                        await this.client.del(patternKey);
                    }
                    
                    console.log(`Removed invalid pattern ${patternId} from ${tier}`);
                }
            }
            
            // Update pool size counter
            await this.client.hSet(`${this.keyPrefixes.pool}size`, tier, validCount.toString());
            
            // Update fragmentation counter
            await this.sep_updateFragmentation(tier);
            
            console.log(`${tier} defragmentation complete. Valid patterns: ${validCount}`);
        } catch (error) {
            console.error(`Failed to defragment ${tier}:`, error);
        }
    }

    async sep_cleanupTier(tier, count) {
        try {
            // Get the least coherent patterns
            const patterns = await this.client.zRange(this.keyPrefixes.tier[tier], 0, count - 1);
            
            console.log(`Cleaning up ${patterns.length} patterns from ${tier}`);
            
            // Delete each pattern
            for (const patternId of patterns) {
                await this.deletePattern(patternId);
            }
            
            console.log(`${tier} cleanup complete. Removed ${patterns.length} patterns`);
        } catch (error) {
            console.error(`Failed to clean up ${tier}:`, error);
        }
    }

    async deletePattern(patternId) {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            const metadataKey = this.sep_getMetadataKey(patternId);
            const patternKey = this.sep_getPatternKey(patternId);
            
            // Get current metadata to find its tier
            const metadata = await this.client.hGetAll(metadataKey);
            if (Object.keys(metadata).length === 0) {
                return false;
            }
            
            // Start a transaction
            const multi = this.client.multi();
            
            // Remove from tier sorted set
            multi.zRem(this.keyPrefixes.tier[metadata.tier], patternId);
            
            // Delete pattern data and metadata
            multi.del(patternKey);
            multi.del(metadataKey);
            
            // Decrement pool size counter
            multi.hIncrBy(`${this.keyPrefixes.pool}size`, metadata.tier, -1);
            
            // Execute transaction
            await multi.exec();
            
            // Update fragmentation counter
            await this.sep_updateFragmentation(metadata.tier);
            
            return true;
        } catch (error) {
            console.error(`Failed to delete pattern ${patternId}:`, error);
            throw error;
        }
    }

    /**
     * Update multiple patterns at once
     * @param {Array} patterns Array of patterns to update
     */
    async updatePatterns(patterns) {
        if (!this.initialized) {
            await this.initialize();
        }

        for (const pattern of patterns) {
            await this.storePattern(pattern.id, pattern);
        }
    }

    /**
     * Get all patterns from all memory tiers
     * @returns {Promise<Array>} Array of all patterns with their metadata
     */
    async getAllPatterns() {
        if (!this.initialized) {
            await this.initialize();
        }

        const patterns = [];
        
        // Get patterns from each tier
        for (const tier of ['STM', 'MTM', 'LTM']) {
            const patternIds = await this.client.zRange(this.keyPrefixes.tier[tier], 0, -1);
            
            // Get each pattern's data
            for (const patternId of patternIds) {
                const pattern = await this.getPattern(patternId);
                if (pattern) {
                    patterns.push(pattern);
                }
            }
        }

        return patterns;
    }

    /**
     * Query patterns based on coherence threshold and optional tier
     * @param {Object} options Query options
     * @param {number} options.minCoherence Minimum coherence threshold
     * @param {string} [options.tier] Optional memory tier to query (STM, MTM, or LTM)
     * @param {number} [options.limit] Maximum number of patterns to return
     * @returns {Promise<Array>} Array of matching patterns with their metadata
     */
    async queryPatterns({ minCoherence = 0.1, tier = null, limit = 100 } = {}) {
        if (!this.initialized) {
            await this.initialize();
        }

        const patterns = [];
        const tiers = tier ? [tier] : ['STM', 'MTM', 'LTM'];
        
        for (const currentTier of tiers) {
            // Get patterns with coherence >= minCoherence
            const patternIds = await this.client.zRangeByScore(
                this.keyPrefixes.tier[currentTier],
                minCoherence,
                '+inf',
                {
                    LIMIT: {
                        offset: 0,
                        count: limit - patterns.length
                    }
                }
            );
            
            // Get each pattern's data
            for (const patternId of patternIds) {
                if (patterns.length >= limit) break;
                
                const pattern = await this.getPattern(patternId);
                if (pattern) {
                    patterns.push(pattern);
                }
            }
            
            if (patterns.length >= limit) break;
        }

        return patterns;
    }
}

/**
 * High-level interface for managing pattern storage and tier transitions.
 * Implements the core memory tier management functionality specified in the
 * SEP CUDA MCP refactoring plan.
 */
class MemoryTierManager {
    constructor(memoryTierOrOptions) {
        if (memoryTierOrOptions instanceof RedisMemoryTier) {
            this.memoryTier = memoryTierOrOptions;
        } else {
            this.memoryTier = new RedisMemoryTier(memoryTierOrOptions || {});
        }
    }

    async initialize() {
        if (!this.memoryTier) {
            throw new Error('Memory tier not provided');
        }
        await this.memoryTier.initialize();
    }

    async managePattern(pattern) {
        const currentTier = await this.getCurrentTier(pattern);
        const metrics = await this.calculateTierMetrics(pattern);

        if (this.shouldPromote(metrics)) {
            await this.promotePattern(pattern);
        } else if (this.shouldDemote(metrics)) {
            await this.demotePattern(pattern);
        }
    }

    async getCurrentTier(pattern) {
        const result = await this.memoryTier.getPattern(pattern.id);
        return result?.metadata?.tier || 'STM';
    }

    async calculateTierMetrics(pattern) {
        try {
            // Get content type handler for coherence calculation
            const handler = this.memoryTier.contentTypeHandler.getHandler(pattern.contentType);
            const contentCoherence = pattern.coherence || handler.calculateCoherence(pattern.data);
            
            return {
                coherence: contentCoherence,
                stability: pattern.stability || 0.5,
                generations: pattern.generations || 1
            };
        } catch (error) {
            return {
                coherence: 0.5,
                stability: 0.5,
                generations: 1
            };
        }
    }

    shouldPromote(metrics) {
        return metrics.coherence >= 0.7 && metrics.stability >= 0.6;
    }

    shouldDemote(metrics) {
        return metrics.coherence < 0.3 || metrics.stability < 0.3;
    }

    async promotePattern(pattern) {
        await this.memoryTier.storePattern(pattern.id, {
            ...pattern,
            tier: 'MTM'
        });
    }

    async demotePattern(pattern) {
        await this.memoryTier.storePattern(pattern.id, {
            ...pattern,
            tier: 'STM'
        });
    }

    async getAllPatterns() {
        return this.memoryTier.getAllPatterns();
    }

    async cleanup() {
        if (this.memoryTier) {
            await this.memoryTier.close();
        }
    }
}

// Export both classes
export { RedisMemoryTier, MemoryTierManager };
export default RedisMemoryTier;