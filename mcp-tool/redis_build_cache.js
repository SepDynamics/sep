/**
 * Redis-based Build Cache for SEP MCP Server
 * 
 * Implements a persistent build cache using Redis as the storage backend.
 * Provides the same API as the in-memory Map-based implementation while adding
 * persistence, TTL-based expiration, and coherence-based cache management.
 */

import { createClient } from 'redis';
import { createHash } from 'crypto';

/**
 * RedisBuildCache class
 * 
 * Provides a Redis-based implementation of the Build Cache with support for:
 * - Deterministic cache key generation using SHA-256 hashing
 * - Cache entry storage, retrieval, and invalidation
 * - TTL-based expiration based on coherence scores
 * - Integration with RedisCoherenceTracker and RedisMemoryTier
 * - Memory tier transitions (STM → MTM → LTM) for cache entries
 */
export class RedisBuildCache {
  /**
   * Constructor
   * @param {Object} options - Configuration options
   * @param {string} options.host - Redis server host (default: '127.0.0.1')
   * @param {number} options.port - Redis server port (default: 6379)
   * @param {string} options.prefix - Key prefix for Redis keys (default: 'sep:build:')
   * @param {Object} options.coherenceThresholds - Coherence thresholds for tier transitions
   * @param {number} options.coherenceThresholds.minimum - Minimum coherence score (default: 0.1)
   * @param {number} options.coherenceThresholds.mtm_promotion - Threshold for STM→MTM promotion (default: 0.7)
   * @param {number} options.coherenceThresholds.ltm_promotion - Threshold for MTM→LTM promotion (default: 0.9)
   * @param {Object} options.ttlSettings - TTL settings for different memory tiers (in seconds)
   * @param {number} options.ttlSettings.STM - TTL for STM tier (default: 1 hour)
   * @param {number} options.ttlSettings.MTM - TTL for MTM tier (default: 7 days)
   * @param {number} options.ttlSettings.LTM - TTL for LTM tier (default: no expiry)
   * @param {Object} options.coherenceTracker - RedisCoherenceTracker instance (optional)
   * @param {Object} options.memoryTier - RedisMemoryTier instance (optional)
   */
  constructor(options = {}) {
    this.options = {
      host: options.host || '127.0.0.1',
      port: options.port || 6379,
      prefix: options.prefix || 'sep:build:',
      coherenceThresholds: {
        minimum: options.coherenceThresholds?.minimum || 0.1,
        mtm_promotion: options.coherenceThresholds?.mtm_promotion || 0.7,
        ltm_promotion: options.coherenceThresholds?.ltm_promotion || 0.9,
        demotion: options.coherenceThresholds?.demotion || 0.3
      },
      ttlSettings: {
        STM: options.ttlSettings?.STM || 60 * 60,         // 1 hour
        MTM: options.ttlSettings?.MTM || 7 * 24 * 60 * 60, // 7 days
        LTM: options.ttlSettings?.LTM || -1               // No expiry
      }
    };

    // External components for integration
    this.coherenceTracker = options.coherenceTracker || null;
    this.memoryTier = options.memoryTier || null;

    // Redis client instance
    this.client = null;
    
    // Key prefixes for different cache data types
    this.keyPrefixes = {
      cache: `${this.options.prefix}data:`,
      metadata: `${this.options.prefix}metadata:`,
      tier: {
        STM: `${this.options.prefix}tier:stm`,
        MTM: `${this.options.prefix}tier:mtm`,
        LTM: `${this.options.prefix}tier:ltm`
      },
      coherence: `${this.options.prefix}coherence`,
      stats: `${this.options.prefix}stats`
    };
    
    // Initialization state
    this.initialized = false;
  }

  /**
   * Initialize the Redis connection and set up the build cache
   * @returns {Promise<void>}
   */
  async initialize() {
    if (this.initialized) {
      return;
    }

    try {
      // Create Redis client
      this.client = createClient({
        url: `redis://${this.options.host}:${this.options.port}`
      });

      // Set up event handlers
      this.client.on('error', (err) => {
        console.error('Redis Build Cache error:', err);
      });

      // Connect to Redis
      await this.client.connect();
      
      // Store configuration in Redis
      await this.client.hSet(`${this.options.prefix}config`, {
        coherence_minimum: this.options.coherenceThresholds.minimum,
        coherence_mtm_promotion: this.options.coherenceThresholds.mtm_promotion,
        coherence_ltm_promotion: this.options.coherenceThresholds.ltm_promotion,
        coherence_demotion: this.options.coherenceThresholds.demotion,
        ttl_stm: this.options.ttlSettings.STM,
        ttl_mtm: this.options.ttlSettings.MTM,
        ttl_ltm: this.options.ttlSettings.LTM
      });
      
      // Initialize stats
      await this.client.hSet(this.keyPrefixes.stats, {
        total_hits: 0,
        total_misses: 0,
        total_stores: 0,
        total_invalidations: 0
      });
      
      this.initialized = true;
      console.log('Redis Build Cache initialized successfully');
    } catch (error) {
      console.error('Failed to initialize Redis Build Cache:', error);
      throw error;
    }
  }

  /**
   * Close the Redis connection
   * @returns {Promise<void>}
   */
  async close() {
    if (this.client) {
      await this.client.quit();
      this.initialized = false;
      console.log('Redis Build Cache connection closed');
    }
  }

  /**
   * Generate a deterministic cache key using SHA-256 hashing
   * @param {Object} params - Build parameters
   * @returns {string} Deterministic cache key
   * @private
   */
  sep_generateCacheKey(params) {
    // Sort keys to ensure deterministic order
    const sortedParams = {};
    Object.keys(params).sort().forEach(key => {
      sortedParams[key] = params[key];
    });
    
    // Create a deterministic string representation
    const paramString = JSON.stringify(sortedParams);
    
    // Generate SHA-256 hash
    return createHash('sha256').update(paramString).digest('hex');
  }

  /**
   * Get cache data key
   * @param {string} cacheKey - Cache key
   * @returns {string} Redis key for the cache data
   * @private
   */
  sep_getCacheDataKey(cacheKey) {
    return `${this.keyPrefixes.cache}${cacheKey}`;
  }

  /**
   * Get metadata key
   * @param {string} cacheKey - Cache key
   * @returns {string} Redis key for the cache metadata
   * @private
   */
  sep_getMetadataKey(cacheKey) {
    return `${this.keyPrefixes.metadata}${cacheKey}`;
  }

  /**
   * Determine the memory tier based on coherence score
   * @param {number} coherence - Coherence score (0.0-1.0)
   * @param {number} accessCount - Number of times the cache entry has been accessed
   * @param {number} ageInHours - Age of the cache entry in hours
   * @returns {string} Memory tier (STM, MTM, or LTM)
   * @private
   */
  sep_determineMemoryTier(coherence, accessCount = 0, ageInHours = 0) {
    // LTM requirements: high coherence, frequent access, and sufficient age
    if (coherence >= this.options.coherenceThresholds.ltm_promotion &&
        accessCount >= 10 &&
        ageInHours >= 24) {
      return 'LTM';
    }
    
    // MTM requirements: medium coherence and some access history
    if (coherence >= this.options.coherenceThresholds.mtm_promotion &&
        accessCount >= 3) {
      return 'MTM';
    }
    
    // Default to STM
    return 'STM';
  }

  /**
   * Calculate TTL based on coherence score and memory tier
   * @param {number} coherence - Coherence score (0.0-1.0)
   * @param {string} tier - Memory tier (STM, MTM, or LTM)
   * @returns {number} TTL in seconds
   * @private
   */
  sep_calculateTTL(coherence, tier) {
    const baseTTL = this.options.ttlSettings[tier];
    
    // No expiry for LTM tier
    if (tier === 'LTM' || baseTTL === -1) {
      return -1;
    }
    
    // For STM and MTM, adjust TTL based on coherence
    // Higher coherence = longer TTL (up to 2x the base TTL)
    const coherenceMultiplier = 1 + coherence;
    return Math.floor(baseTTL * coherenceMultiplier);
  }
/**
   * Store a build result in the cache
   * @param {Object} params - Build parameters
   * @param {Object} result - Build result
   * @param {Object} options - Storage options
   * @param {number} options.coherence - Coherence score (0.0-1.0)
   * @param {string} options.tier - Memory tier (STM, MTM, or LTM)
   * @returns {Promise<Object>} Cache metadata
   */
  async store(params, result, options = {}) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Generate cache key
      const cacheKey = this.sep_generateCacheKey(params);
      
      // Get coherence score from options, coherence tracker, or default
      let coherence = options.coherence;
      if (coherence === undefined && this.coherenceTracker) {
        try {
          const componentId = `build:${cacheKey}`;
          const coherenceData = await this.coherenceTracker.getCoherence(componentId);
          coherence = coherenceData ? coherenceData.score : 0.5;
        } catch (error) {
          console.warn(`Failed to get coherence from tracker for ${cacheKey}:`, error);
          coherence = 0.5;
        }
      } else if (coherence === undefined) {
        coherence = 0.5;
      }
      
      // Ensure coherence is within valid range
      coherence = Math.max(0, Math.min(1, coherence));
      
      // Prepare metadata
      const timestamp = new Date().toISOString();
      const metadata = {
        key: cacheKey,
        params: JSON.stringify(params),
        coherence,
        created_at: timestamp,
        last_accessed: timestamp,
        access_count: 0,
        size: JSON.stringify(result).length
      };
      
      // Determine memory tier
      const tier = options.tier || this.sep_determineMemoryTier(coherence);
      metadata.tier = tier;
      
      // Calculate TTL
      const ttl = this.sep_calculateTTL(coherence, tier);
      metadata.ttl = ttl;
      
      // Start a transaction
      const multi = this.client.multi();
      
      // Store cache data
      multi.set(this.sep_getCacheDataKey(cacheKey), JSON.stringify(result));
      
      // Store metadata
      multi.hSet(this.sep_getMetadataKey(cacheKey), metadata);
      
      // Add to appropriate tier sorted set with coherence as score
      multi.zAdd(this.keyPrefixes.tier[tier], {
        score: coherence,
        value: cacheKey
      });
      
      // Add to coherence sorted set
      multi.zAdd(this.keyPrefixes.coherence, {
        score: coherence,
        value: cacheKey
      });
      
      // Set TTL if not LTM
      if (ttl !== -1) {
        multi.expire(this.sep_getCacheDataKey(cacheKey), ttl);
        multi.expire(this.sep_getMetadataKey(cacheKey), ttl);
      }
      
      // Update stats
      multi.hIncrBy(this.keyPrefixes.stats, 'total_stores', 1);
      
      // Execute transaction
      await multi.exec();
      
      // Track coherence if coherence tracker is available
      if (this.coherenceTracker) {
        try {
          const componentId = `build:${cacheKey}`;
          await this.coherenceTracker.trackCoherence(componentId, coherence, {
            cacheKey,
            tier,
            timestamp
          });
        } catch (error) {
          console.warn(`Failed to track coherence for ${cacheKey}:`, error);
        }
      }
      
      return metadata;
    } catch (error) {
      console.error('Failed to store in build cache:', error);
      throw error;
    }
  }

  /**
   * Get a build result from the cache
   * @param {Object} params - Build parameters
   * @returns {Promise<Object|null>} Build result or null if not found
   */
  async get(params) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Generate cache key
      const cacheKey = this.sep_generateCacheKey(params);
      
      // Get cache data
      const cacheDataKey = this.sep_getCacheDataKey(cacheKey);
      const cachedResult = await this.client.get(cacheDataKey);
      
      // Update stats based on hit/miss
      if (cachedResult) {
        await this.client.hIncrBy(this.keyPrefixes.stats, 'total_hits', 1);
      } else {
        await this.client.hIncrBy(this.keyPrefixes.stats, 'total_misses', 1);
        return null;
      }
      
      // Get metadata
      const metadataKey = this.sep_getMetadataKey(cacheKey);
      const metadata = await this.client.hGetAll(metadataKey);
      
      if (Object.keys(metadata).length === 0) {
        // Metadata missing but data exists - inconsistent state
        // Clean up orphaned data
        await this.client.del(cacheDataKey);
        return null;
      }
      
      // Parse result
      let result;
      try {
        result = JSON.parse(cachedResult);
      } catch (parseError) {
        console.error(`Failed to parse cached result for ${cacheKey}:`, parseError);
        
        // Clean up corrupted data
        await this.invalidate(params);
        return null;
      }
      
      // Update access metadata
      const timestamp = new Date().toISOString();
      const accessCount = parseInt(metadata.access_count || '0', 10) + 1;
      
      await this.client.hSet(metadataKey, {
        last_accessed: timestamp,
        access_count: accessCount
      });
      
      // Check if tier promotion is needed based on access count and coherence
      const currentTier = metadata.tier || 'STM';
      const coherence = parseFloat(metadata.coherence || '0.5');
      const createdAt = new Date(metadata.created_at || timestamp);
      const ageInHours = (new Date() - createdAt) / (1000 * 60 * 60);
      
      const newTier = this.sep_determineMemoryTier(coherence, accessCount, ageInHours);
      
      if (newTier !== currentTier) {
        await this.sep_promoteCacheEntry(cacheKey, currentTier, newTier, coherence);
      }
      
      // Reset TTL if not in LTM
      if (currentTier !== 'LTM') {
        const ttl = this.sep_calculateTTL(coherence, currentTier);
        if (ttl !== -1) {
          await this.client.expire(cacheDataKey, ttl);
          await this.client.expire(metadataKey, ttl);
        }
      }
      
      return result;
    } catch (error) {
      console.error('Failed to get from build cache:', error);
      throw error;
    }
  }

  /**
   * Check if a build result exists in the cache
   * @param {Object} params - Build parameters
   * @returns {Promise<boolean>} True if the build result exists in the cache
   */
  async has(params) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Generate cache key
      const cacheKey = this.sep_generateCacheKey(params);
      
      // Check if cache data exists
      const exists = await this.client.exists(this.sep_getCacheDataKey(cacheKey));
      return exists === 1;
    } catch (error) {
      console.error('Failed to check build cache:', error);
      throw error;
    }
  }

  /**
   * Invalidate a build result in the cache
   * @param {Object} params - Build parameters
   * @returns {Promise<boolean>} True if the build result was invalidated
   */
  async invalidate(params) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Generate cache key
      const cacheKey = this.sep_generateCacheKey(params);
      
      // Get metadata to find the tier
      const metadataKey = this.sep_getMetadataKey(cacheKey);
      const metadata = await this.client.hGetAll(metadataKey);
      
      if (Object.keys(metadata).length === 0) {
        return false;
      }
      
      const tier = metadata.tier || 'STM';
      
      // Start a transaction
      const multi = this.client.multi();
      
      // Remove from tier sorted set
      multi.zRem(this.keyPrefixes.tier[tier], cacheKey);
      
      // Remove from coherence sorted set
      multi.zRem(this.keyPrefixes.coherence, cacheKey);
      
      // Delete cache data and metadata
      multi.del(this.sep_getCacheDataKey(cacheKey));
      multi.del(metadataKey);
      
      // Update stats
      multi.hIncrBy(this.keyPrefixes.stats, 'total_invalidations', 1);
      
      // Execute transaction
      await multi.exec();
      
      return true;
    } catch (error) {
      console.error('Failed to invalidate build cache:', error);
      throw error;
    }
  }
/**
   * Promote a cache entry to a higher memory tier
   * @param {string} cacheKey - Cache key
   * @param {string} currentTier - Current memory tier
   * @param {string} newTier - New memory tier
   * @param {number} coherence - Coherence score
   * @returns {Promise<void>}
   * @private
   */
  async sep_promoteCacheEntry(cacheKey, currentTier, newTier, coherence) {
    try {
      // Start a transaction
      const multi = this.client.multi();
      
      // Remove from old tier
      multi.zRem(this.keyPrefixes.tier[currentTier], cacheKey);
      
      // Add to new tier with coherence as score
      multi.zAdd(this.keyPrefixes.tier[newTier], {
        score: coherence,
        value: cacheKey
      });
      
      // Update tier in metadata
      multi.hSet(this.sep_getMetadataKey(cacheKey), 'tier', newTier);
      
      // Update TTL based on new tier
      const ttl = this.sep_calculateTTL(coherence, newTier);
      multi.hSet(this.sep_getMetadataKey(cacheKey), 'ttl', ttl);
      
      if (ttl === -1) {
        // Remove expiry for LTM
        multi.persist(this.sep_getCacheDataKey(cacheKey));
        multi.persist(this.sep_getMetadataKey(cacheKey));
      } else {
        // Set new TTL for STM/MTM
        multi.expire(this.sep_getCacheDataKey(cacheKey), ttl);
        multi.expire(this.sep_getMetadataKey(cacheKey), ttl);
      }
      
      // Execute transaction
      await multi.exec();
      
      console.log(`Cache entry ${cacheKey} promoted from ${currentTier} to ${newTier}`);
    } catch (error) {
      console.error(`Failed to promote cache entry ${cacheKey}:`, error);
      throw error;
    }
  }

  /**
   * Clear all entries from the cache
   * @returns {Promise<number>} Number of entries cleared
   */
  async clear() {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Get all cache keys from all tiers
      const stmKeys = await this.client.zRange(this.keyPrefixes.tier.STM, 0, -1);
      const mtmKeys = await this.client.zRange(this.keyPrefixes.tier.MTM, 0, -1);
      const ltmKeys = await this.client.zRange(this.keyPrefixes.tier.LTM, 0, -1);
      
      const allKeys = [...stmKeys, ...mtmKeys, ...ltmKeys];
      
      if (allKeys.length === 0) {
        return 0;
      }
      
      // Start a transaction
      const multi = this.client.multi();
      
      // Delete all cache data and metadata
      for (const key of allKeys) {
        multi.del(this.sep_getCacheDataKey(key));
        multi.del(this.sep_getMetadataKey(key));
      }
      
      // Clear tier sorted sets
      multi.del(this.keyPrefixes.tier.STM);
      multi.del(this.keyPrefixes.tier.MTM);
      multi.del(this.keyPrefixes.tier.LTM);
      
      // Clear coherence sorted set
      multi.del(this.keyPrefixes.coherence);
      
      // Reset stats
      multi.hSet(this.keyPrefixes.stats, {
        total_hits: 0,
        total_misses: 0,
        total_stores: 0,
        total_invalidations: 0
      });
      
      // Execute transaction
      await multi.exec();
      
      return allKeys.length;
    } catch (error) {
      console.error('Failed to clear build cache:', error);
      throw error;
    }
  }

  /**
   * Get cache statistics
   * @returns {Promise<Object>} Cache statistics
   */
  async getStats() {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Get stats
      const stats = await this.client.hGetAll(this.keyPrefixes.stats);
      
      // Convert string values to numbers
      Object.keys(stats).forEach(key => {
        stats[key] = parseInt(stats[key], 10);
      });
      
      // Get counts from each tier
      const [stmCount, mtmCount, ltmCount] = await Promise.all([
        this.client.zCard(this.keyPrefixes.tier.STM),
        this.client.zCard(this.keyPrefixes.tier.MTM),
        this.client.zCard(this.keyPrefixes.tier.LTM)
      ]);
      
      // Calculate hit rate
      const totalRequests = stats.total_hits + stats.total_misses;
      const hitRate = totalRequests > 0 ? stats.total_hits / totalRequests : 0;
      
      return {
        hits: stats.total_hits,
        misses: stats.total_misses,
        stores: stats.total_stores,
        invalidations: stats.total_invalidations,
        hitRate,
        tierCounts: {
          STM: stmCount,
          MTM: mtmCount,
          LTM: ltmCount
        },
        totalEntries: stmCount + mtmCount + ltmCount
      };
    } catch (error) {
      console.error('Failed to get build cache stats:', error);
      throw error;
    }
  }

  /**
   * Get all cache entries from a specific tier
   * @param {string} tier - Memory tier (STM, MTM, or LTM)
   * @param {number} limit - Maximum number of entries to retrieve
   * @param {number} offset - Offset for pagination
   * @returns {Promise<Array>} Cache entries
   */
  async getEntriesFromTier(tier, limit = 100, offset = 0) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Validate tier
      if (!this.keyPrefixes.tier[tier]) {
        throw new Error(`Invalid tier: ${tier}`);
      }
      
      // Get cache keys from tier
      const keys = await this.client.zRange(this.keyPrefixes.tier[tier], offset, offset + limit - 1);
      
      if (keys.length === 0) {
        return [];
      }
      
      // Get metadata for each key
      const entries = [];
      
      for (const key of keys) {
        const metadata = await this.client.hGetAll(this.sep_getMetadataKey(key));
        
        if (Object.keys(metadata).length > 0) {
          // Convert numeric fields
          if (metadata.coherence) metadata.coherence = parseFloat(metadata.coherence);
          if (metadata.access_count) metadata.access_count = parseInt(metadata.access_count, 10);
          if (metadata.size) metadata.size = parseInt(metadata.size, 10);
          if (metadata.ttl) metadata.ttl = parseInt(metadata.ttl, 10);
          
          // Parse params
          try {
            metadata.params = JSON.parse(metadata.params || '{}');
          } catch (error) {
            metadata.params = {};
          }
          
          entries.push(metadata);
        }
      }
      
      return entries;
    } catch (error) {
      console.error(`Failed to get entries from tier ${tier}:`, error);
      throw error;
    }
  }

  /**
   * Get cache entries by coherence score range
   * @param {number} minCoherence - Minimum coherence score
   * @param {number} maxCoherence - Maximum coherence score
   * @param {number} limit - Maximum number of entries to retrieve
   * @returns {Promise<Array>} Cache entries
   */
  async getEntriesByCoherence(minCoherence, maxCoherence, limit = 100) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Get cache keys by coherence score range
      const keys = await this.client.zRangeByScore(this.keyPrefixes.coherence, minCoherence, maxCoherence, {
        LIMIT: {
          offset: 0,
          count: limit
        }
      });
      
      if (keys.length === 0) {
        return [];
      }
      
      // Get metadata for each key
      const entries = [];
      
      for (const key of keys) {
        const metadata = await this.client.hGetAll(this.sep_getMetadataKey(key));
        
        if (Object.keys(metadata).length > 0) {
          // Convert numeric fields
          if (metadata.coherence) metadata.coherence = parseFloat(metadata.coherence);
          if (metadata.access_count) metadata.access_count = parseInt(metadata.access_count, 10);
          if (metadata.size) metadata.size = parseInt(metadata.size, 10);
          if (metadata.ttl) metadata.ttl = parseInt(metadata.ttl, 10);
          
          // Parse params
          try {
            metadata.params = JSON.parse(metadata.params || '{}');
          } catch (error) {
            metadata.params = {};
          }
          
          entries.push(metadata);
        }
      }
      
      return entries;
    } catch (error) {
      console.error('Failed to get entries by coherence:', error);
      throw error;
    }
  }

  /**
   * Cleanup expired or low-coherence entries
   * @param {number} coherenceThreshold - Minimum coherence score to keep (default: minimum threshold)
   * @returns {Promise<number>} Number of entries cleaned up
   */
  async cleanup(coherenceThreshold = null) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Use provided threshold or default minimum
      const threshold = coherenceThreshold !== null ? 
        coherenceThreshold : this.options.coherenceThresholds.minimum;
      
      // Get low-coherence entries
      const lowCoherenceKeys = await this.client.zRangeByScore(this.keyPrefixes.coherence, 0, threshold);
      
      if (lowCoherenceKeys.length === 0) {
        return 0;
      }
      
      // Start a transaction
      const multi = this.client.multi();
      
      // Delete low-coherence entries
      for (const key of lowCoherenceKeys) {
        // Get metadata to find the tier
        const metadata = await this.client.hGetAll(this.sep_getMetadataKey(key));
        const tier = metadata.tier || 'STM';
        
        // Remove from tier sorted set
        multi.zRem(this.keyPrefixes.tier[tier], key);
        
        // Remove from coherence sorted set
        multi.zRem(this.keyPrefixes.coherence, key);
        
        // Delete cache data and metadata
        multi.del(this.sep_getCacheDataKey(key));
        multi.del(this.sep_getMetadataKey(key));
      }
      
      // Execute transaction
      await multi.exec();
      
      return lowCoherenceKeys.length;
    } catch (error) {
      console.error('Failed to cleanup build cache:', error);
      throw error;
    }
  }
}