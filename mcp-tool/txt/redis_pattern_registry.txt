/**
 * Redis-based Pattern Registry for SEP MCP Server
 * 
 * Implements a persistent pattern registry using Redis as the storage backend.
 * Provides the same API as the in-memory Map-based implementation while adding
 * persistence, atomic operations, and TTL-based memory tier management.
 */

import { createClient } from 'redis';

/**
 * RedisPatternRegistry class
 * 
 * Provides a Redis-based implementation of the Pattern Registry with support for:
 * - Pattern storage, retrieval, updating, and deletion
 * - Pattern evolution and mutation tracking
 * - Memory tier transitions (STM → MTM → LTM) based on coherence scores
 * - TTL settings based on memory tier (STM: 1h, MTM: 7d, LTM: no expiry)
 */
class RedisPatternRegistry {
  /**
   * Constructor
   * @param {Object} options - Configuration options
   * @param {string} options.host - Redis server host (default: '127.0.0.1')
   * @param {number} options.port - Redis server port (default: 6379)
   * @param {string} options.prefix - Key prefix for Redis keys (default: 'sep:pattern:')
   * @param {Object} options.coherenceThresholds - Coherence thresholds for tier transitions
   * @param {number} options.coherenceThresholds.minimum - Minimum coherence score (default: 0.1)
   * @param {number} options.coherenceThresholds.mtm_promotion - Threshold for STM→MTM promotion (default: 0.7)
   * @param {number} options.coherenceThresholds.ltm_promotion - Threshold for MTM→LTM promotion (default: 0.9)
   * @param {number} options.coherenceThresholds.demotion - Threshold for tier demotion (default: 0.3)
   * @param {number} options.coherenceThresholds.relationship_minimum - Minimum relationship strength (default: 0.6)
   */
  constructor(options = {}) {
    this.options = {
      host: options.host || '127.0.0.1',
      port: options.port || 6379,
      prefix: options.prefix || 'sep:pattern:',
      coherenceThresholds: {
        minimum: options.coherenceThresholds?.minimum || 0.1,
        mtm_promotion: options.coherenceThresholds?.mtm_promotion || 0.7,
        ltm_promotion: options.coherenceThresholds?.ltm_promotion || 0.9,
        demotion: options.coherenceThresholds?.demotion || 0.3,
        relationship_minimum: options.coherenceThresholds?.relationship_minimum || 0.6
      }
    };

    // TTL settings for different memory tiers (in seconds)
    this.ttlSettings = {
      STM: 60 * 60,      // 1 hour
      MTM: 7 * 24 * 60 * 60, // 7 days
      LTM: -1            // No expiry
    };

    // Redis client instance
    this.client = null;
    
    // Key prefixes for different pattern types
    this.keyPrefixes = {
      pattern: `${this.options.prefix}data:`,
      evolution: `${this.options.prefix}evolution:`,
      mutation: `${this.options.prefix}mutation:`,
      tier: {
        STM: `${this.options.prefix}tier:stm`,
        MTM: `${this.options.prefix}tier:mtm`,
        LTM: `${this.options.prefix}tier:ltm`
      },
      coherence: `${this.options.prefix}coherence`,
      stability: `${this.options.prefix}stability`,
      usage: `${this.options.prefix}usage`
    };
    
    // Initialization state
    this.initialized = false;
  }

  /**
   * Initialize the Redis connection and set up the pattern registry
   * @returns {Promise<void>}
   */
  async initialize() {
    if (this.initialized) {
      return;
    }

    try {
      console.error('RedisPatternRegistry: Initializing with options:', {
        host: this.options.host,
        port: this.options.port,
        socket: process.env.SEP_REDIS_SOCKET,
        prefix: this.options.prefix
      });
      
      // Create Redis client with improved error handling and retry logic
      const socketPath = process.env.SEP_REDIS_SOCKET;
      console.error('RedisPatternRegistry: Using connection:', socketPath ? `Unix socket: ${socketPath}` : `TCP: ${this.options.host}:${this.options.port}`);
      
      try {
        const redisOptions = {
          socket: socketPath ? {
            path: socketPath,
            connectTimeout: 10000, // Increased timeout to 10 seconds
            reconnectStrategy: (retries) => {
              console.error(`RedisPatternRegistry: Retry attempt ${retries}`);
              if (retries > 5) {
                console.error('RedisPatternRegistry: Max retries reached');
                return new Error('Max retries reached');
              }
              return Math.min(retries * 100, 3000); // Exponential backoff with max 3s
            }
          } : {
            host: this.options.host,
            port: this.options.port,
            connectTimeout: 10000, // Increased timeout to 10 seconds
            reconnectStrategy: (retries) => {
              console.error(`RedisPatternRegistry: Retry attempt ${retries}`);
              if (retries > 5) {
                console.error('RedisPatternRegistry: Max retries reached');
                return new Error('Max retries reached');
              }
              return Math.min(retries * 100, 3000); // Exponential backoff with max 3s
            }
          }
        };
        
        console.error('RedisPatternRegistry: Creating Redis client with options:', JSON.stringify(redisOptions));
        this.client = createClient(redisOptions);
        console.error('RedisPatternRegistry: Redis client created successfully');
      } catch (clientError) {
        console.error('RedisPatternRegistry: Failed to create Redis client:', clientError);
        throw clientError;
      }

      // Set up event handlers with improved logging
      this.client.on('error', (err) => {
        console.error('RedisPatternRegistry error:', {
          error: err.message,
          code: err.code,
          command: err.command,
          stack: err.stack
        });
      });
      
      // Add connection status logging
      this.client.on('connect', () => {
        console.error('RedisPatternRegistry: Connecting...');
      });

      this.client.on('ready', () => {
        console.error('RedisPatternRegistry: Connection ready');
      });

      this.client.on('reconnecting', () => {
        console.error('RedisPatternRegistry: Reconnecting...');
      });

      this.client.on('end', () => {
        console.error('RedisPatternRegistry: Connection ended');
      });

      // Connect to Redis with timeout
      console.error('RedisPatternRegistry: Attempting to connect to Redis...');
      try {
        // Use Promise.race to implement a connection timeout
        await Promise.race([
          this.client.connect(),
          new Promise((_, reject) =>
            setTimeout(() => reject(new Error('Redis connection timeout after 15s')), 15000)
          )
        ]);
        console.error('RedisPatternRegistry: Successfully connected to Redis');
        
        // Test connection with PING
        const pingResult = await this.client.ping();
        console.error(`RedisPatternRegistry: Redis PING result: ${pingResult}`);
      } catch (connectError) {
        console.error('RedisPatternRegistry: Failed to connect to Redis:', connectError);
        throw connectError;
      }
      
      // Store coherence thresholds in Redis
      await this.client.hSet(`${this.options.prefix}config:thresholds`, this.options.coherenceThresholds);
      
      this.initialized = true;
      console.log('Redis Pattern Registry initialized successfully');
    } catch (error) {
      console.error('Failed to initialize Redis Pattern Registry:', error);
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
      console.log('Redis Pattern Registry connection closed');
    }
  }

  /**
   * Get pattern key
   * @param {string} patternId - Pattern ID
   * @returns {string} Redis key for the pattern
   * @private
   */
  sep_getPatternKey(patternId) {
    return `${this.keyPrefixes.pattern}${patternId}`;
  }

  /**
   * Get evolution key
   * @param {string} patternId - Pattern ID
   * @returns {string} Redis key for the pattern evolution
   * @private
   */
  sep_getEvolutionKey(patternId) {
    return `${this.keyPrefixes.evolution}${patternId}`;
  }

  /**
   * Get mutation key
   * @param {string} patternId - Pattern ID
   * @returns {string} Redis key for the pattern mutation
   * @private
   */
  sep_getMutationKey(patternId) {
    return `${this.keyPrefixes.mutation}${patternId}`;
  }

  /**
   * Determine the memory tier based on coherence and stability scores
   * @param {number} coherence - Coherence score (0.0-1.0)
   * @param {number} stability - Stability score (0.0-1.0)
   * @param {number} generations - Number of generations
   * @returns {string} Memory tier (STM, MTM, or LTM)
   * @private
   */
  sep_determineMemoryTier(coherence, stability, generations = 0) {
    // LTM requirements: high coherence, stability, and sufficient generations
    if (coherence >= this.options.coherenceThresholds.ltm_promotion && 
        stability >= 0.85 && 
        generations >= 100) {
      return 'LTM';
    }
    
    // MTM requirements: medium coherence and stability
    if (coherence >= this.options.coherenceThresholds.mtm_promotion) {
      return 'MTM';
    }
    
    // Default to STM
    return 'STM';
  }
/**
   * Store a pattern in the registry
   * @param {string} patternId - Pattern ID
   * @param {Object} patternData - Pattern data
   * @returns {Promise<void>}
   */
  async storePattern(patternId, patternData) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Ensure patternData has required fields
      const pattern = {
        ...patternData,
        id: patternId,
        coherence: patternData.coherence || 0.5,
        stability: patternData.stability || 0.5,
        usage_count: patternData.usage_count || 0,
        last_used: patternData.last_used || new Date().toISOString(),
        created_at: patternData.created_at || new Date().toISOString(),
        generations: patternData.generations || 1
      };

      // Determine memory tier
      const tier = patternData.tier || 
        this.sep_determineMemoryTier(pattern.coherence, pattern.stability, pattern.generations);
      pattern.tier = tier;

      // Store pattern data as hash
      await this.client.hSet(this.sep_getPatternKey(patternId), pattern);
      
      // Add to appropriate tier sorted set with coherence as score
      await this.client.zAdd(this.keyPrefixes.tier[tier], {
        score: pattern.coherence,
        value: patternId
      });
      
      // Add to coherence and stability sorted sets
      await this.client.zAdd(this.keyPrefixes.coherence, {
        score: pattern.coherence,
        value: patternId
      });
      
      await this.client.zAdd(this.keyPrefixes.stability, {
        score: pattern.stability,
        value: patternId
      });
      
      // Add to usage sorted set
      await this.client.zAdd(this.keyPrefixes.usage, {
        score: pattern.usage_count,
        value: patternId
      });
      
      // Set TTL based on tier
      await this.sep_setTierTTL(patternId, tier);
      
      return pattern;
    } catch (error) {
      console.error(`Failed to store pattern ${patternId}:`, error);
      throw error;
    }
  }

  /**
   * Get a pattern from the registry
   * @param {string} patternId - Pattern ID
   * @returns {Promise<Object|null>} Pattern data or null if not found
   */
  async getPattern(patternId) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const patternKey = this.sep_getPatternKey(patternId);
      const pattern = await this.client.hGetAll(patternKey);
      
      if (Object.keys(pattern).length === 0) {
        return null;
      }
      
      // Convert numeric fields
      if (pattern.coherence) pattern.coherence = parseFloat(pattern.coherence);
      if (pattern.stability) pattern.stability = parseFloat(pattern.stability);
      if (pattern.usage_count) pattern.usage_count = parseInt(pattern.usage_count, 10);
      if (pattern.generations) pattern.generations = parseInt(pattern.generations, 10);
      
      // Update usage count and last_used
      await this.client.hSet(patternKey, {
        usage_count: (pattern.usage_count || 0) + 1,
        last_used: new Date().toISOString()
      });
      
      // Update usage sorted set
      await this.client.zIncrBy(this.keyPrefixes.usage, 1, patternId);
      
      return pattern;
    } catch (error) {
      console.error(`Failed to get pattern ${patternId}:`, error);
      throw error;
    }
  }

  /**
   * Update a pattern in the registry
   * @param {string} patternId - Pattern ID
   * @param {Object} updates - Pattern updates
   * @returns {Promise<Object|null>} Updated pattern data or null if not found
   */
  async updatePattern(patternId, updates) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const patternKey = this.sep_getPatternKey(patternId);
      
      // Check if pattern exists
      const exists = await this.client.exists(patternKey);
      if (!exists) {
        return null;
      }
      
      // Get current pattern data
      const currentPattern = await this.client.hGetAll(patternKey);
      
      // Convert numeric fields
      if (currentPattern.coherence) currentPattern.coherence = parseFloat(currentPattern.coherence);
      if (currentPattern.stability) currentPattern.stability = parseFloat(currentPattern.stability);
      if (currentPattern.usage_count) currentPattern.usage_count = parseInt(currentPattern.usage_count, 10);
      if (currentPattern.generations) currentPattern.generations = parseInt(currentPattern.generations, 10);
      
      // Merge updates with current pattern
      const updatedPattern = {
        ...currentPattern,
        ...updates,
        last_updated: new Date().toISOString()
      };
      
      // Check if coherence or stability changed
      const coherenceChanged = updates.coherence !== undefined && 
        updates.coherence !== currentPattern.coherence;
      
      const stabilityChanged = updates.stability !== undefined && 
        updates.stability !== currentPattern.stability;
      
      // Update pattern data
      await this.client.hSet(patternKey, updatedPattern);
      
      // Update sorted sets if scores changed
      if (coherenceChanged) {
        await this.client.zAdd(this.keyPrefixes.coherence, {
          score: updatedPattern.coherence,
          value: patternId
        });
      }
      
      if (stabilityChanged) {
        await this.client.zAdd(this.keyPrefixes.stability, {
          score: updatedPattern.stability,
          value: patternId
        });
      }
      
      // Check if tier transition is needed
      if (coherenceChanged || stabilityChanged) {
        await this.sep_checkTierTransition(patternId, updatedPattern);
      }
      
      return updatedPattern;
    } catch (error) {
      console.error(`Failed to update pattern ${patternId}:`, error);
      throw error;
    }
  }

  /**
   * Delete a pattern from the registry
   * @param {string} patternId - Pattern ID
   * @returns {Promise<boolean>} True if pattern was deleted, false if not found
   */
  async deletePattern(patternId) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const patternKey = this.sep_getPatternKey(patternId);
      const evolutionKey = this.sep_getEvolutionKey(patternId);
      const mutationKey = this.sep_getMutationKey(patternId);
      
      // Get current pattern to find its tier
      const pattern = await this.client.hGetAll(patternKey);
      if (Object.keys(pattern).length === 0) {
        return false;
      }
      
      // Remove from all sorted sets
      await this.client.zRem(this.keyPrefixes.tier[pattern.tier], patternId);
      await this.client.zRem(this.keyPrefixes.coherence, patternId);
      await this.client.zRem(this.keyPrefixes.stability, patternId);
      await this.client.zRem(this.keyPrefixes.usage, patternId);
      
      // Delete pattern data, evolution, and mutation
      await this.client.del(patternKey);
      await this.client.del(evolutionKey);
      await this.client.del(mutationKey);
      
      return true;
    } catch (error) {
      console.error(`Failed to delete pattern ${patternId}:`, error);
      throw error;
    }
  }

  /**
   * Check if a pattern needs to be promoted or demoted
   * @param {string} patternId - Pattern ID
   * @param {Object} pattern - Pattern data
   * @returns {Promise<void>}
   * @private
   */
  async sep_checkTierTransition(patternId, pattern) {
    const currentTier = pattern.tier || 'STM';
    const coherence = parseFloat(pattern.coherence);
    const stability = parseFloat(pattern.stability);
    const generations = parseInt(pattern.generations || 1, 10);
    
    // Determine new tier
    const newTier = this.sep_determineMemoryTier(coherence, stability, generations);
    
    // If tier changed, update tier membership
    if (newTier !== currentTier) {
      // Start a transaction
      const multi = this.client.multi();
      
      // Remove from old tier
      multi.zRem(this.keyPrefixes.tier[currentTier], patternId);
      
      // Add to new tier with coherence as score
      multi.zAdd(this.keyPrefixes.tier[newTier], {
        score: coherence,
        value: patternId
      });
      
      // Update tier in pattern data
      multi.hSet(this.sep_getPatternKey(patternId), 'tier', newTier);
      
      // Set TTL based on new tier
      if (this.ttlSettings[newTier] === -1) {
        multi.persist(this.sep_getPatternKey(patternId));
        multi.persist(this.sep_getEvolutionKey(patternId));
        multi.persist(this.sep_getMutationKey(patternId));
      } else {
        multi.expire(this.sep_getPatternKey(patternId), this.ttlSettings[newTier]);
        multi.expire(this.sep_getEvolutionKey(patternId), this.ttlSettings[newTier]);
        multi.expire(this.sep_getMutationKey(patternId), this.ttlSettings[newTier]);
      }
      
      // Execute transaction
      await multi.exec();
      
      console.log(`Pattern ${patternId} transitioned from ${currentTier} to ${newTier}`);
      
      // Record tier transition in evolution history
      await this.recordEvolution(patternId, {
        timestamp: new Date().toISOString(),
        event: 'tier_transition',
        from: currentTier,
        to: newTier,
        coherence,
        stability
      });
    }
  }

  /**
   * Set TTL for a pattern based on its memory tier
   * @param {string} patternId - Pattern ID
   * @param {string} tier - Memory tier (STM, MTM, or LTM)
   * @returns {Promise<void>}
   * @private
   */
  async sep_setTierTTL(patternId, tier) {
    const ttl = this.ttlSettings[tier];
    const patternKey = this.sep_getPatternKey(patternId);
    const evolutionKey = this.sep_getEvolutionKey(patternId);
    const mutationKey = this.sep_getMutationKey(patternId);
    
    // If TTL is -1, remove expiry
    if (ttl === -1) {
      await this.client.persist(patternKey);
      await this.client.persist(evolutionKey);
      await this.client.persist(mutationKey);
    } else {
      await this.client.expire(patternKey, ttl);
      await this.client.expire(evolutionKey, ttl);
      await this.client.expire(mutationKey, ttl);
    }
  }

  /**
   * Record pattern evolution
   * @param {string} patternId - Pattern ID
   * @param {Object} evolutionData - Evolution data
   * @returns {Promise<void>}
   */
  /**
   * Analyze relationships between patterns
   * @param {Array<Object>} patterns - Array of patterns to analyze
   * @returns {Promise<Object>} Relationship analysis results
   */
  async analyzeRelationships(patterns) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const relationships = {};
      
      // Analyze relationships between each pair of patterns
      for (let i = 0; i < patterns.length; i++) {
        for (let j = i + 1; j < patterns.length; j++) {
          const pattern1 = patterns[i];
          const pattern2 = patterns[j];
          
          // Calculate relationship strength based on coherence scores
          const strength = Math.min(
            pattern1.coherence * pattern2.coherence,
            this.options.coherenceThresholds.relationship_minimum
          );
          
          if (strength >= this.options.coherenceThresholds.relationship_minimum) {
            const relationshipId = `${pattern1.id}:${pattern2.id}`;
            relationships[relationshipId] = {
              patterns: [pattern1.id, pattern2.id],
              strength,
              timestamp: new Date().toISOString()
            };
          }
        }
      }
      
      return relationships;
    } catch (error) {
      console.error('Failed to analyze pattern relationships:', error);
      throw error;
    }
  }

  /**
   * Update pattern relationships in the registry
   * @param {Object} relationships - Pattern relationship data
   * @returns {Promise<void>}
   */
  async updateRelationships(relationships) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const multi = this.client.multi();
      
      // Store each relationship
      for (const [relationshipId, data] of Object.entries(relationships)) {
        const key = `${this.options.prefix}relationship:${relationshipId}`;
        multi.hSet(key, {
          ...data,
          strength: data.strength.toString(),
          patterns: JSON.stringify(data.patterns)
        });
        
        // Set TTL based on strength (stronger relationships last longer)
        const ttl = Math.floor(data.strength * this.ttlSettings.MTM);
        if (ttl > 0) {
          multi.expire(key, ttl);
        }
      }
      
      await multi.exec();
    } catch (error) {
      console.error('Failed to update pattern relationships:', error);
      throw error;
    }
  }

  async recordEvolution(patternId, evolutionData) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const evolutionKey = this.sep_getEvolutionKey(patternId);
      const timestamp = evolutionData.timestamp || new Date().toISOString();
      
      // Add evolution record to sorted set with timestamp as score
      await this.client.zAdd(evolutionKey, {
        score: new Date(timestamp).getTime(),
        value: JSON.stringify(evolutionData)
      });
      
      // Increment generations count
      await this.client.hIncrBy(this.sep_getPatternKey(patternId), 'generations', 1);
      
      // Update pattern with latest evolution data if it contains coherence/stability
      if (evolutionData.coherence !== undefined || evolutionData.stability !== undefined) {
        const updates = {};
        
        if (evolutionData.coherence !== undefined) {
          updates.coherence = evolutionData.coherence;
        }
        
        if (evolutionData.stability !== undefined) {
          updates.stability = evolutionData.stability;
        }
        
        if (Object.keys(updates).length > 0) {
          await this.updatePattern(patternId, updates);
        }
      }
    } catch (error) {
      console.error(`Failed to record evolution for pattern ${patternId}:`, error);
      throw error;
    }
  }

  /**
   * Get pattern evolution history
   * @param {string} patternId - Pattern ID
   * @param {number} limit - Maximum number of evolution records to return
   * @returns {Promise<Array>} Evolution history
   */
  async getEvolutionHistory(patternId, limit = 100) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const evolutionKey = this.sep_getEvolutionKey(patternId);
      
      // Get evolution records sorted by timestamp (newest first)
      const records = await this.client.zRange(evolutionKey, 0, limit - 1, {
        REV: true
      });
      
      // Parse JSON records
      return records.map(record => JSON.parse(record));
    } catch (error) {
      console.error(`Failed to get evolution history for pattern ${patternId}:`, error);
      throw error;
    }
  }

  /**
   * Record pattern mutation
   * @param {string} patternId - Pattern ID
   * @param {Object} mutationData - Mutation data
   * @returns {Promise<void>}
   */
  async recordMutation(patternId, mutationData) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const mutationKey = this.sep_getMutationKey(patternId);
      const timestamp = mutationData.timestamp || new Date().toISOString();
      
      // Add mutation record to sorted set with timestamp as score
      await this.client.zAdd(mutationKey, {
        score: new Date(timestamp).getTime(),
        value: JSON.stringify(mutationData)
      });
      
      // Record in evolution history as well
      await this.recordEvolution(patternId, {
        ...mutationData,
        event: 'mutation'
      });
    } catch (error) {
      console.error(`Failed to record mutation for pattern ${patternId}:`, error);
      throw error;
    }
  }

  /**
   * Get pattern mutation history
   * @param {string} patternId - Pattern ID
   * @param {number} limit - Maximum number of mutation records to return
   * @returns {Promise<Array>} Mutation history
   */
  async getMutationHistory(patternId, limit = 100) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const mutationKey = this.sep_getMutationKey(patternId);
      
      // Get mutation records sorted by timestamp (newest first)
      const records = await this.client.zRange(mutationKey, 0, limit - 1, {
        REV: true
      });
      
      // Parse JSON records
      return records.map(record => JSON.parse(record));
    } catch (error) {
      console.error(`Failed to get mutation history for pattern ${patternId}:`, error);
      throw error;
    }
  }
}

// Export both named and default exports
export { RedisPatternRegistry };
export default RedisPatternRegistry;