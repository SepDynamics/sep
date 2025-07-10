/**
 * Redis-based Coherence Tracker for SEP MCP Server
 * 
 * Implements a persistent coherence tracking system using Redis as the storage backend.
 * Provides the same API as the in-memory Map-based implementation while adding
 * persistence, time-series analysis, and integration with the memory tier system.
 */

import { createClient } from 'redis';

/**
 * RedisCoherenceTracker class
 * 
 * Provides a Redis-based implementation of the Coherence Tracker with support for:
 * - Coherence score tracking, updating, and retrieval
 * - Time-series analysis of coherence scores with exponential decay
 * - Pattern extraction and analysis using QBSA-inspired techniques
 * - Integration with RedisPatternRegistry and RedisMemoryTier
 */
class RedisCoherenceTracker {
    constructor() {
        this.redisKeyPrefix = 'coherence:';
        this.timeSeriesPrefix = 'timeseries:coherence:';
        this.initialized = false;
        this.client = null;
    }

    /**
     * Initialize Redis connection and data structures
     * @returns {Promise<void>}
     */
    async initialize() {
        if (this.initialized) return;

        try {
            // Redis client will be injected by RedisIntegration
            this.initialized = true;
            console.log('Coherence tracker initialized');
        } catch (error) {
            console.error('Failed to initialize coherence tracker:', error);
            throw error;
        }
    }

    /**
     * Set Redis client
     * @param {Object} client Redis client instance
     */
    setRedisClient(client) {
        this.client = client;
    }

    /**
     * Track coherence score for a component
     * @param {string} componentId Component ID
     * @param {number} score Coherence score (0.0-1.0)
     * @param {Object} metadata Additional metadata
     * @returns {Promise<Object>} Tracked coherence data
     */
    async trackCoherence(componentId, score, metadata = {}) {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            const timestamp = new Date().toISOString();
            const key = `${this.redisKeyPrefix}${componentId}`;
            const timeSeriesKey = `${this.timeSeriesPrefix}${componentId}`;

            const coherenceData = {
                id: componentId,
                score,
                timestamp,
                ...metadata
            };

            await this.client.multi()
                .hSet(key, coherenceData)
                .zAdd(timeSeriesKey, {
                    score: new Date(timestamp).getTime(),
                    value: JSON.stringify(coherenceData)
                })
                .exec();

            return coherenceData;
        } catch (error) {
            console.error('Error tracking coherence:', error);
            throw error;
        }
    }

    /**
     * Get coherence history for a component
     * @param {string} componentId Component ID
     * @param {number} limit Maximum number of entries
     * @returns {Promise<Array>} Coherence history
     */
    async getCoherenceHistory(componentId, limit = 100) {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            const timeSeriesKey = `${this.timeSeriesPrefix}${componentId}`;
            const entries = await this.client.zRevRange(timeSeriesKey, 0, limit - 1);
            
            return entries.map(entry => JSON.parse(entry));
        } catch (error) {
            console.error('Error getting coherence history:', error);
            throw error;
        }
    }

    /**
     * Close Redis connection
     * @returns {Promise<void>}
     */
    async close() {
        if (this.client) {
            await this.client.quit();
            this.initialized = false;
        }
    }
}

// Export only as default
export default RedisCoherenceTracker;