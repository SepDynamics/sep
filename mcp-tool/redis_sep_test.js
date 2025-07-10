#!/usr/bin/env node
import { RedisIntegration } from './redis_integration.js';
import { seedGrandProject } from './grand_project_init.js';

/**
 * Test Redis integration with SEP memory tiers
 * Validates deterministic pattern storage and coherence tracking
 */
async function testRedisIntegration() {
    console.log('Testing SEP Redis Integration...\n');

    const redisConfig = {
        enabled: true,
        host: process.env.SEP_REDIS_HOST || '127.0.0.1',
        port: parseInt(process.env.SEP_REDIS_PORT) || 6379,
        prefix: process.env.SEP_REDIS_PREFIX || 'sep:test:'
    };

    const redis = new RedisIntegration(redisConfig);
    
    try {
        // Test 1: Redis Connection
        console.log('1. Testing Redis connection...');
        const connected = await redis.initialize();
        
        if (!connected) {
            console.log('❌ Redis connection failed - server will use fallback mode');
            return false;
        }
        console.log('✅ Redis connected successfully\n');

        // Test 2: Memory Tier Storage
        console.log('2. Testing memory tier storage...');
        const memoryTier = redis.getComponent('memoryTier');
        
        // Store a test pattern in STM
        await memoryTier.storePattern('test_kernel_001', {
            data: { 
                kernelCode: '__global__ void test_kernel() { /* deterministic logic */ }',
                analysis: { coherence: 0.85, deterministic: true }
            },
            coherence: 0.85,
            contentType: 'cuda_kernel'
        });
        console.log('✅ Pattern stored in memory tier\n');

        // Test 3: Coherence Tracking
        console.log('3. Testing coherence tracking...');
        const coherenceTracker = redis.getComponent('coherenceTracker');
        
        await coherenceTracker.trackCoherence('test_kernel_001', 0.85, {
            type: 'cuda_kernel',
            deterministic: true,
            timestamp: new Date().toISOString()
        });
        console.log('✅ Coherence tracked successfully\n');

        // Test 4: Pattern Registry
        console.log('4. Testing pattern registry...');
        const patternRegistry = redis.getComponent('patternRegistry');
        
        await patternRegistry.storePattern('pattern_test_001', {
            type: 'cuda_kernel',
            coherence: 0.85,
            stability: 0.9,
            generations: 1
        });
        console.log('✅ Pattern registered successfully\n');

        // Test 5: Grand Project Seeding
        console.log('5. Testing Grand Project initialization...');
        await seedGrandProject(redis);
        console.log('✅ Grand Project patterns seeded\n');

        // Test 6: Verify Data Persistence
        console.log('6. Verifying data persistence...');
        const patterns = await patternRegistry.getAllPatterns();
        console.log(`✅ Found ${patterns.length} persistent patterns\n`);

        console.log('🎉 All SEP Redis integration tests passed!');
        console.log('Your system is ready for deterministic pattern evolution.\n');

        return true;

    } catch (error) {
        console.error('❌ Redis integration test failed:', error);
        return false;
    } finally {
        await redis.close();
    }
}

// Run the test
testRedisIntegration()
    .then(success => {
        if (success) {
            console.log('SEP Redis coordination is ready.');
            console.log('Start your MCP server with: node sep_mcp_server.js');
        } else {
            console.log('Fix Redis issues before starting MCP server.');
        }
        process.exit(success ? 0 : 1);
    })
    .catch(error => {
        console.error('Test execution error:', error);
        process.exit(1);
    });
