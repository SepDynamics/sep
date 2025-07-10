/**
 * Redis-based Dependency Graph for SEP MCP Server
 * 
 * Implements a persistent dependency graph using Redis as the storage backend.
 * Provides the same API as the in-memory Map-based implementation while adding
 * persistence, bidirectional dependency tracking, and coherence-based relationship strength.
 */

import { createClient } from 'redis';

/**
 * RedisDependencyGraph class
 * 
 * Provides a Redis-based implementation of the Dependency Graph with support for:
 * - Adding, removing, and querying dependencies
 * - Bidirectional dependency tracking
 * - Dependency strength based on coherence scores
 * - Circular dependency detection
 * - Dependency traversal and impact analysis
 */
export class RedisDependencyGraph {
  /**
   * Constructor
   * @param {Object} options - Configuration options
   * @param {string} options.host - Redis server host (default: '127.0.0.1')
   * @param {number} options.port - Redis server port (default: 6379)
   * @param {string} options.prefix - Key prefix for Redis keys (default: 'sep:dependency:')
   * @param {Object} options.coherenceThresholds - Coherence thresholds for dependency relationships
   * @param {number} options.coherenceThresholds.minimum - Minimum coherence score for dependency (default: 0.1)
   * @param {number} options.coherenceThresholds.strong - Threshold for strong dependency (default: 0.7)
   * @param {number} options.coherenceThresholds.critical - Threshold for critical dependency (default: 0.9)
   * @param {Object} options.coherenceTracker - RedisCoherenceTracker instance (optional)
   * @param {Object} options.patternRegistry - RedisPatternRegistry instance (optional)
   */
  constructor(options = {}) {
    this.options = {
      host: options.host || '127.0.0.1',
      port: options.port || 6379,
      prefix: options.prefix || 'sep:dependency:',
      coherenceThresholds: {
        minimum: options.coherenceThresholds?.minimum || 0.1,
        strong: options.coherenceThresholds?.strong || 0.7,
        critical: options.coherenceThresholds?.critical || 0.9
      }
    };

    // External components for integration
    this.coherenceTracker = options.coherenceTracker || null;
    this.patternRegistry = options.patternRegistry || null;

    // Redis client instance
    this.client = null;
    
    // Key prefixes for different dependency data types
    this.keyPrefixes = {
      // Direct dependencies (component -> dependencies)
      dependencies: `${this.options.prefix}deps:`,
      
      // Reverse dependencies (component -> dependents)
      dependents: `${this.options.prefix}deps_rev:`,
      
      // Dependency metadata (strength, type, timestamp)
      metadata: `${this.options.prefix}meta:`,
      
      // Circular dependency detection
      circular: `${this.options.prefix}circular`,
      
      // Component metadata
      components: `${this.options.prefix}components`,
      
      // Stats
      stats: `${this.options.prefix}stats`
    };
    
    // Initialization state
    this.initialized = false;
  }

  /**
   * Initialize the Redis connection and set up the dependency graph
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
        console.error('Redis Dependency Graph error:', err);
      });

      // Connect to Redis
      await this.client.connect();
      
      // Store configuration in Redis
      await this.client.hSet(`${this.options.prefix}config`, {
        coherence_minimum: this.options.coherenceThresholds.minimum,
        coherence_strong: this.options.coherenceThresholds.strong,
        coherence_critical: this.options.coherenceThresholds.critical
      });
      
      // Initialize stats
      await this.client.hSet(this.keyPrefixes.stats, {
        total_components: 0,
        total_dependencies: 0,
        total_circular_dependencies: 0,
        last_updated: new Date().toISOString()
      });
      
      this.initialized = true;
      console.log('Redis Dependency Graph initialized successfully');
    } catch (error) {
      console.error('Failed to initialize Redis Dependency Graph:', error);
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
      console.log('Redis Dependency Graph connection closed');
    }
  }

  /**
   * Get dependency key
   * @param {string} component - Component name
   * @returns {string} Redis key for the component's dependencies
   * @private
   */
  sep_getDependencyKey(component) {
    return `${this.keyPrefixes.dependencies}${component}`;
  }

  /**
   * Get dependent key
   * @param {string} component - Component name
   * @returns {string} Redis key for the component's dependents
   * @private
   */
  sep_getDependentKey(component) {
    return `${this.keyPrefixes.dependents}${component}`;
  }

  /**
   * Get metadata key for a dependency relationship
   * @param {string} component - Source component
   * @param {string} dependency - Dependency component
   * @returns {string} Redis key for the dependency metadata
   * @private
   */
  sep_getMetadataKey(component, dependency) {
    return `${this.keyPrefixes.metadata}${component}:${dependency}`;
  }

  /**
   * Calculate dependency strength based on coherence scores
   * @param {string} component - Source component
   * @param {string} dependency - Dependency component
   * @returns {Promise<number>} Dependency strength (0.0-1.0)
   * @private
   */
  async sep_calculateDependencyStrength(component, dependency) {
    // If coherence tracker is available, use it to calculate strength
    if (this.coherenceTracker) {
      try {
        const sourceCoherence = await this.coherenceTracker.getCoherence(`component:${component}`);
        const targetCoherence = await this.coherenceTracker.getCoherence(`component:${dependency}`);
        
        if (sourceCoherence && targetCoherence) {
          // Calculate strength based on both components' coherence
          // Higher coherence = stronger dependency
          const avgCoherence = (sourceCoherence.score + targetCoherence.score) / 2;
          
          // Apply relationship strength formula
          // This is a simplified version of the normalized dot product
          return Math.min(1.0, avgCoherence * 1.2);
        }
      } catch (error) {
        console.warn(`Failed to get coherence for dependency ${component} -> ${dependency}:`, error);
      }
    }
    
    // Default strength if coherence tracker is not available
    return 0.5;
  }

  /**
   * Detect circular dependencies
   * @param {string} component - Source component
   * @param {string} dependency - Dependency component
   * @returns {Promise<boolean>} True if adding this dependency would create a circular dependency
   * @private
   */
  async sep_detectCircularDependency(component, dependency) {
    // If component and dependency are the same, it's circular
    if (component === dependency) {
      return true;
    }
    
    // Check if dependency depends on component (direct circular)
    const dependencyDeps = await this.getDependencies(dependency);
    if (dependencyDeps.includes(component)) {
      return true;
    }
    
    // Check for indirect circular dependencies using BFS
    const visited = new Set();
    const queue = [...dependencyDeps];
    
    while (queue.length > 0) {
      const current = queue.shift();
      
      if (current === component) {
        return true;
      }
      
      if (!visited.has(current)) {
        visited.add(current);
        const currentDeps = await this.getDependencies(current);
        queue.push(...currentDeps);
      }
    }
    
    return false;
  }

  /**
   * Add a dependency relationship
   * @param {string} component - Source component
   * @param {string} dependency - Dependency component
   * @param {Object} options - Dependency options
   * @param {string} options.type - Dependency type (default: 'code')
   * @param {number} options.strength - Dependency strength (0.0-1.0, default: calculated)
   * @param {boolean} options.allowCircular - Allow circular dependencies (default: false)
   * @returns {Promise<Object>} Dependency metadata
   */
  async addDependency(component, dependency, options = {}) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Check for circular dependency
      const isCircular = await this.sep_detectCircularDependency(component, dependency);
      if (isCircular && !options.allowCircular) {
        throw new Error(`Circular dependency detected: ${component} -> ${dependency}`);
      }
      
      // Calculate dependency strength if not provided
      let strength = options.strength;
      if (strength === undefined) {
        strength = await this.sep_calculateDependencyStrength(component, dependency);
      }
      
      // Ensure strength is within valid range
      strength = Math.max(0, Math.min(1, strength));
      
      // Prepare metadata
      const timestamp = new Date().toISOString();
      const metadata = {
        source: component,
        target: dependency,
        type: options.type || 'code',
        strength: strength.toString(),
        created_at: timestamp,
        updated_at: timestamp,
        is_circular: isCircular ? '1' : '0'
      };
      
      // Start a transaction
      const multi = this.client.multi();
      
      // Add to dependencies set
      multi.sAdd(this.sep_getDependencyKey(component), dependency);
      
      // Add to dependents set (reverse dependency)
      multi.sAdd(this.sep_getDependentKey(dependency), component);
      
      // Store metadata
      multi.hSet(this.sep_getMetadataKey(component, dependency), metadata);
      
      // Add components to components set if they don't exist
      multi.sAdd(this.keyPrefixes.components, component);
      multi.sAdd(this.keyPrefixes.components, dependency);
      
      // Track circular dependency if detected
      if (isCircular) {
        multi.sAdd(this.keyPrefixes.circular, `${component}:${dependency}`);
        multi.hIncrBy(this.keyPrefixes.stats, 'total_circular_dependencies', 1);
      }
      
      // Update stats
      multi.hIncrBy(this.keyPrefixes.stats, 'total_dependencies', 1);
      multi.hSet(this.keyPrefixes.stats, 'last_updated', timestamp);
      
      // Execute transaction
      await multi.exec();
      
      // Track coherence if coherence tracker is available
      if (this.coherenceTracker) {
        try {
          const relationId = `dependency:${component}:${dependency}`;
          await this.coherenceTracker.trackCoherence(relationId, strength, {
            type: 'dependency',
            source: component,
            target: dependency,
            timestamp
          });
        } catch (error) {
          console.warn(`Failed to track coherence for dependency ${component} -> ${dependency}:`, error);
        }
      }
      
      return metadata;
    } catch (error) {
      console.error(`Failed to add dependency ${component} -> ${dependency}:`, error);
      throw error;
    }
  }

  /**
   * Remove a dependency relationship
   * @param {string} component - Source component
   * @param {string} dependency - Dependency component
   * @returns {Promise<boolean>} True if the dependency was removed
   */
  async removeDependency(component, dependency) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Check if the dependency exists
      const exists = await this.client.sIsMember(this.sep_getDependencyKey(component), dependency);
      if (!exists) {
        return false;
      }
      
      // Get metadata to check if it's circular
      const metadataKey = this.sep_getMetadataKey(component, dependency);
      const metadata = await this.client.hGetAll(metadataKey);
      const isCircular = metadata.is_circular === '1';
      
      // Start a transaction
      const multi = this.client.multi();
      
      // Remove from dependencies set
      multi.sRem(this.sep_getDependencyKey(component), dependency);
      
      // Remove from dependents set (reverse dependency)
      multi.sRem(this.sep_getDependentKey(dependency), component);
      
      // Delete metadata
      multi.del(metadataKey);
      
      // Remove from circular dependencies if needed
      if (isCircular) {
        multi.sRem(this.keyPrefixes.circular, `${component}:${dependency}`);
        multi.hIncrBy(this.keyPrefixes.stats, 'total_circular_dependencies', -1);
      }
      
      // Update stats
      multi.hIncrBy(this.keyPrefixes.stats, 'total_dependencies', -1);
      multi.hSet(this.keyPrefixes.stats, 'last_updated', new Date().toISOString());
      
      // Execute transaction
      await multi.exec();
      
      return true;
    } catch (error) {
      console.error(`Failed to remove dependency ${component} -> ${dependency}:`, error);
      throw error;
    }
  }

  /**
   * Get all dependencies for a component
   * @param {string} component - Component name
   * @returns {Promise<string[]>} Array of dependencies
   */
  async getDependencies(component) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const dependencies = await this.client.sMembers(this.sep_getDependencyKey(component));
      return dependencies;
    } catch (error) {
      console.error(`Failed to get dependencies for ${component}:`, error);
      throw error;
    }
  }

  /**
   * Get all dependents for a component
   * @param {string} component - Component name
   * @returns {Promise<string[]>} Array of dependents
   */
  async getDependents(component) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const dependents = await this.client.sMembers(this.sep_getDependentKey(component));
      return dependents;
    } catch (error) {
      console.error(`Failed to get dependents for ${component}:`, error);
      throw error;
    }
  }

  /**
   * Get metadata for a dependency relationship
   * @param {string} component - Source component
   * @param {string} dependency - Dependency component
   * @returns {Promise<Object|null>} Dependency metadata or null if not found
   */
  async getDependencyMetadata(component, dependency) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const metadata = await this.client.hGetAll(this.sep_getMetadataKey(component, dependency));
      
      if (Object.keys(metadata).length === 0) {
        return null;
      }
      
      // Convert string values to appropriate types
      return {
        ...metadata,
        strength: parseFloat(metadata.strength),
        is_circular: metadata.is_circular === '1'
      };
    } catch (error) {
      console.error(`Failed to get metadata for dependency ${component} -> ${dependency}:`, error);
      throw error;
    }
  }

  /**
   * Update dependency metadata
   * @param {string} component - Source component
   * @param {string} dependency - Dependency component
   * @param {Object} updates - Metadata updates
   * @returns {Promise<Object|null>} Updated metadata or null if not found
   */
  async updateDependencyMetadata(component, dependency, updates) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Check if the dependency exists
      const exists = await this.client.sIsMember(this.sep_getDependencyKey(component), dependency);
      if (!exists) {
        return null;
      }
      
      // Get current metadata
      const metadataKey = this.sep_getMetadataKey(component, dependency);
      const currentMetadata = await this.client.hGetAll(metadataKey);
      
      if (Object.keys(currentMetadata).length === 0) {
        return null;
      }
      
      // Prepare updates
      const updatedMetadata = {
        ...currentMetadata,
        ...updates,
        updated_at: new Date().toISOString()
      };
      
      // Update metadata
      await this.client.hSet(metadataKey, updatedMetadata);
      
      // Convert string values to appropriate types for return value
      return {
        ...updatedMetadata,
        strength: parseFloat(updatedMetadata.strength),
        is_circular: updatedMetadata.is_circular === '1'
      };
    } catch (error) {
      console.error(`Failed to update metadata for dependency ${component} -> ${dependency}:`, error);
      throw error;
    }
  }

  /**
   * Get all components in the dependency graph
   * @returns {Promise<string[]>} Array of component names
   */
  async getAllComponents() {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const components = await this.client.sMembers(this.keyPrefixes.components);
      return components;
    } catch (error) {
      console.error('Failed to get all components:', error);
      throw error;
    }
  }

  /**
   * Get all circular dependencies
   * @returns {Promise<Array<{source: string, target: string}>>} Array of circular dependencies
   */
  async getCircularDependencies() {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const circularKeys = await this.client.sMembers(this.keyPrefixes.circular);
      
      return circularKeys.map(key => {
        const [source, target] = key.split(':');
        return { source, target };
      });
    } catch (error) {
      console.error('Failed to get circular dependencies:', error);
      throw error;
    }
  }

  /**
   * Get transitive dependencies for a component
   * @param {string} component - Component name
   * @param {number} maxDepth - Maximum depth to traverse (default: Infinity)
   * @returns {Promise<Map<string, number>>} Map of dependencies to their distance from the component
   */
  async getTransitiveDependencies(component, maxDepth = Infinity) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const result = new Map();
      const visited = new Set();
      const queue = [{ component, depth: 0 }];
      
      while (queue.length > 0) {
        const { component: current, depth } = queue.shift();
        
        if (visited.has(current) || depth > maxDepth) {
          continue;
        }
        
        visited.add(current);
        
        if (current !== component) {
          result.set(current, depth);
        }
        
        if (depth < maxDepth) {
          const dependencies = await this.getDependencies(current);
          
          for (const dependency of dependencies) {
            if (!visited.has(dependency)) {
              queue.push({ component: dependency, depth: depth + 1 });
            }
          }
        }
      }
      
      return result;
    } catch (error) {
      console.error(`Failed to get transitive dependencies for ${component}:`, error);
      throw error;
    }
  }

  /**
   * Get transitive dependents for a component
   * @param {string} component - Component name
   * @param {number} maxDepth - Maximum depth to traverse (default: Infinity)
   * @returns {Promise<Map<string, number>>} Map of dependents to their distance from the component
   */
  async getTransitiveDependents(component, maxDepth = Infinity) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      const result = new Map();
      const visited = new Set();
      const queue = [{ component, depth: 0 }];
      
      while (queue.length > 0) {
        const { component: current, depth } = queue.shift();
        
        if (visited.has(current) || depth > maxDepth) {
          continue;
        }
        
        visited.add(current);
        
        if (current !== component) {
          result.set(current, depth);
        }
        
        if (depth < maxDepth) {
          const dependents = await this.getDependents(current);
          
          for (const dependent of dependents) {
            if (!visited.has(dependent)) {
              queue.push({ component: dependent, depth: depth + 1 });
            }
          }
        }
      }
      
      return result;
    } catch (error) {
      console.error(`Failed to get transitive dependents for ${component}:`, error);
      throw error;
    }
  }

  /**
   * Perform impact analysis for a component
   * @param {string} component - Component name
   * @returns {Promise<Object>} Impact analysis results
   */
  async analyzeImpact(component) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Get direct dependencies and dependents
      const [dependencies, dependents] = await Promise.all([
        this.getDependencies(component),
        this.getDependents(component)
      ]);
      
      // Get transitive dependencies and dependents
      const [transitiveDependencies, transitiveDependents] = await Promise.all([
        this.getTransitiveDependencies(component),
        this.getTransitiveDependents(component)
      ]);
      
      // Get metadata for direct relationships
      const dependencyMetadata = await Promise.all(
        dependencies.map(dep => this.getDependencyMetadata(component, dep))
      );
      
      const dependentMetadata = await Promise.all(
        dependents.map(dep => this.getDependencyMetadata(dep, component))
      );
      
      // Calculate impact scores
      const impactScore = dependents.length * 0.7 + transitiveDependents.size * 0.3;
      const dependencyScore = dependencies.length * 0.7 + transitiveDependencies.size * 0.3;
      
      // Identify critical paths
      const criticalDependencies = dependencyMetadata
        .filter(meta => meta && meta.strength >= this.options.coherenceThresholds.critical)
        .map(meta => meta.target);
      
      const criticalDependents = dependentMetadata
        .filter(meta => meta && meta.strength >= this.options.coherenceThresholds.critical)
        .map(meta => meta.source);
      
      return {
        component,
        direct_dependencies: dependencies,
        direct_dependents: dependents,
        transitive_dependencies: Array.from(transitiveDependencies.keys()),
        transitive_dependents: Array.from(transitiveDependents.keys()),
        impact_score: impactScore,
        dependency_score: dependencyScore,
        critical_dependencies: criticalDependencies,
        critical_dependents: criticalDependents,
        total_dependencies: dependencies.length + transitiveDependencies.size,
        total_dependents: dependents.length + transitiveDependents.size
      };
    } catch (error) {
      console.error(`Failed to analyze impact for ${component}:`, error);
      throw error;
    }
  }

  /**
   * Build the dependency graph from a set of components and their dependencies
   * @param {Object<string, string[]>} graph - Map of components to their dependencies
   * @returns {Promise<boolean>} True if the graph was built successfully
   */
  async buildDependencyGraph(graph) {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Start a transaction
      const multi = this.client.multi();
      
      // Clear existing data
      const keys = await this.client.keys(`${this.options.prefix}*`);
      if (keys.length > 0) {
        multi.del(...keys);
      }
      
      // Reset stats
      multi.hSet(this.keyPrefixes.stats, {
        total_components: 0,
        total_dependencies: 0,
        total_circular_dependencies: 0,
        last_updated: new Date().toISOString()
      });
      
      // Execute transaction to clear data
      await multi.exec();
      
      // Add components and dependencies
      let totalComponents = 0;
      let totalDependencies = 0;
      
      for (const [component, dependencies] of Object.entries(graph)) {
        totalComponents++;
        
        // Add component to components set
        await this.client.sAdd(this.keyPrefixes.components, component);
        
        // Add dependencies
        for (const dependency of dependencies) {
          try {
            await this.addDependency(component, dependency, { allowCircular: true });
            totalDependencies++;
          } catch (error) {
            console.warn(`Failed to add dependency ${component} -> ${dependency}:`, error);
          }
        }
      }
      
      // Update stats
      await this.client.hSet(this.keyPrefixes.stats, {
        total_components: totalComponents,
        total_dependencies: totalDependencies,
        last_updated: new Date().toISOString()
      });
      
      return true;
    } catch (error) {
      console.error('Failed to build dependency graph:', error);
      return false;
    }
  }

  /**
   * Get statistics about the dependency graph
   * @returns {Promise<Object>} Statistics
   */
  async getStats() {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Get basic stats
      const stats = await this.client.hGetAll(this.keyPrefixes.stats);
      
      // Get component count
      const componentCount = await this.client.sCard(this.keyPrefixes.components);
      
      // Get circular dependency count
      const circularCount = await this.client.sCard(this.keyPrefixes.circular);
      
      // Calculate average dependencies per component
      const avgDependencies = componentCount > 0 
        ? parseInt(stats.total_dependencies || '0', 10) / componentCount 
        : 0;
      
      return {
        total_components: parseInt(stats.total_components || '0', 10),
        total_dependencies: parseInt(stats.total_dependencies || '0', 10),
        total_circular_dependencies: parseInt(stats.total_circular_dependencies || '0', 10),
        avg_dependencies_per_component: avgDependencies,
        last_updated: stats.last_updated || new Date().toISOString()
      };
    } catch (error) {
      console.error('Failed to get dependency graph stats:', error);
      throw error;
    }
  }

  /**
   * Clear the dependency graph
   * @returns {Promise<boolean>} True if the graph was cleared successfully
   */
  async clear() {
    if (!this.initialized) {
      await this.initialize();
    }

    try {
      // Get all keys with the prefix
      const keys = await this.client.keys(`${this.options.prefix}*`);
      
      if (keys.length > 0) {
        // Delete all keys
        await this.client.del(...keys);
      }
      
      // Reset stats
      await this.client.hSet(this.keyPrefixes.stats, {
        total_components: 0,
        total_dependencies: 0,
        total_circular_dependencies: 0,
        last_updated: new Date().toISOString()
      });
      
      return true;
    } catch (error) {
      console.error('Failed to clear dependency graph:', error);
      return false;
    }
  }
}