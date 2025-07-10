#!/usr/bin/env node
/**
 * SEP MCP Coordination Server Diagnostic Tool
 * 
 * This script helps diagnose issues with the SEP MCP Coordination Server
 * by testing each component individually and providing detailed logging.
 */

import { createClient } from 'redis';
import { RedisIntegration } from './redis_integration.js';
import { RedisMemoryTier } from './redis_memory_tier.js';
import SEPCudaAssistantMCP from './sep_cuda_mcp.js';
import { ManifoldContextInjector } from './sep_context_injector.js';
import CoordinatedSEPServer from './sep_mcp_server.js';

// Configuration
const config = {
    redis: {
        enabled: process.env.SEP_REDIS_ENABLED === 'true',
        host: process.env.SEP_REDIS_HOST || '127.0.0.1',
        port: parseInt(process.env.SEP_REDIS_PORT) || 6379,
        socket: process.env.SEP_REDIS_SOCKET || null,
        prefix: process.env.SEP_REDIS_PREFIX || 'sep:'
    },
    timeout: {
        redis: 5000,
        cuda: 8000,
        component: 10000
    }
};

// Diagnostic functions
async function testRedisConnection() {
    console.log('\n=== Testing Redis Connection ===');
    console.log(`Configuration: ${JSON.stringify(config.redis, null, 2)}`);
    
    try {
        // Create Redis client with timeout
        console.log('Creating Redis client...');
        const client = createClient({
            socket: config.redis.socket ? {
                path: config.redis.socket,
                connectTimeout: config.timeout.redis
            } : {
                host: config.redis.host,
                port: config.redis.port,
                connectTimeout: config.timeout.redis
            }
        });
        
        // Add event handlers
        client.on('error', (err) => {
            console.error('Redis error:', err);
        });
        
        client.on('connect', () => {
            console.log('Redis connecting...');
        });
        
        client.on('ready', () => {
            console.log('Redis connection ready');
        });
        
        // Connect with timeout
        console.log(`Connecting to Redis with ${config.timeout.redis}ms timeout...`);
        const startTime = Date.now();
        
        try {
            await Promise.race([
                client.connect(),
                new Promise((_, reject) => 
                    setTimeout(() => reject(new Error('Redis connection timeout')), 
                    config.timeout.redis)
                )
            ]);
            
            console.log(`✅ Redis connection successful (${Date.now() - startTime}ms)`);
            
            // Test basic operations
            console.log('Testing Redis operations...');
            await client.set('sep:diagnostic:test', 'success');
            const result = await client.get('sep:diagnostic:test');
            console.log(`Redis test result: ${result}`);
            
            // Cleanup
            await client.del('sep:diagnostic:test');
            await client.quit();
            
            return true;
        } catch (error) {
            console.error(`❌ Redis connection failed (${Date.now() - startTime}ms):`, error.message);
            return false;
        }
    } catch (error) {
        console.error('❌ Redis client creation failed:', error.message);
        return false;
    }
}

async function testRedisIntegration() {
    console.log('\n=== Testing Redis Integration ===');
    
    try {
        // Create RedisIntegration instance
        console.log('Creating RedisIntegration instance...');
        const redisIntegration = new RedisIntegration(config.redis);
        
        // Initialize with timeout
        console.log(`Initializing RedisIntegration with ${config.timeout.component}ms timeout...`);
        const startTime = Date.now();
        
        try {
            const success = await Promise.race([
                redisIntegration.initialize(),
                new Promise((_, reject) => 
                    setTimeout(() => reject(new Error('RedisIntegration initialization timeout')), 
                    config.timeout.component)
                )
            ]);
            
            if (success) {
                console.log(`✅ RedisIntegration initialization successful (${Date.now() - startTime}ms)`);
                
                // Test component access
                console.log('Testing component access...');
                const memoryTier = redisIntegration.getComponent('memoryTier');
                console.log('Memory tier component:', memoryTier ? '✅ Available' : '❌ Unavailable');
                
                const coherenceTracker = redisIntegration.getComponent('coherenceTracker');
                console.log('Coherence tracker component:', coherenceTracker ? '✅ Available' : '❌ Unavailable');
                
                // Cleanup
                await redisIntegration.close();
                
                return true;
            } else {
                console.error(`❌ RedisIntegration initialization failed (${Date.now() - startTime}ms)`);
                return false;
            }
        } catch (error) {
            console.error(`❌ RedisIntegration initialization failed (${Date.now() - startTime}ms):`, error.message);
            return false;
        }
    } catch (error) {
        console.error('❌ RedisIntegration creation failed:', error.message);
        return false;
    }
}

async function testCudaAssistant() {
    console.log('\n=== Testing CUDA Assistant ===');
    
    try {
        // Create SEPCudaAssistantMCP instance
        console.log('Creating SEPCudaAssistantMCP instance...');
        const cudaAssistant = new SEPCudaAssistantMCP({
            ...config.redis,
            coherenceThresholds: {
                codeComplexity: 0.7,
                memoryAlignment: 0.9,
                kernelOccupancy: 0.8,
                patternStability: 0.85
            }
        });
        
        // Initialize with timeout
        console.log(`Initializing CUDA Assistant with ${config.timeout.cuda}ms timeout...`);
        const startTime = Date.now();
        
        try {
            const success = await Promise.race([
                cudaAssistant.initialize(),
                new Promise((_, reject) => 
                    setTimeout(() => reject(new Error('CUDA Assistant initialization timeout')), 
                    config.timeout.cuda)
                )
            ]);
            
            if (success) {
                console.log(`✅ CUDA Assistant initialization successful (${Date.now() - startTime}ms)`);
                
                // Test kernel analysis
                console.log('Testing kernel analysis...');
                const result = await cudaAssistant.analyzeKernel({
                    kernelCode: `
                        __global__ void testKernel(float* data, int size) {
                            const int idx = blockIdx.x * blockDim.x + threadIdx.x;
                            if (idx < size) {
                                data[idx] = data[idx] * 2.0f;
                            }
                        }
                    `
                });
                
                console.log('Kernel analysis result:', result ? '✅ Success' : '❌ Failed');
                console.log(`Coherence score: ${result?.coherenceScore || 'N/A'}`);
                
                // Cleanup
                await cudaAssistant.cleanup();
                
                return true;
            } else {
                console.error(`❌ CUDA Assistant initialization failed (${Date.now() - startTime}ms)`);
                return false;
            }
        } catch (error) {
            console.error(`❌ CUDA Assistant initialization failed (${Date.now() - startTime}ms):`, error.message);
            return false;
        }
    } catch (error) {
        console.error('❌ CUDA Assistant creation failed:', error.message);
        return false;
    }
}

async function testContextInjector() {
    console.log('\n=== Testing Context Injector ===');
    
    try {
        // Create ManifoldContextInjector instance
        console.log('Creating ManifoldContextInjector instance...');
        const contextInjector = new ManifoldContextInjector({
            host: config.redis.host,
            port: config.redis.port,
            prefix: `${config.redis.prefix}manifold:context:`
        });
        
        // Test context injection
        console.log('Testing context injection...');
        const startTime = Date.now();
        
        try {
            const result = await Promise.race([
                contextInjector.injectContext({
                    request: { type: 'test', data: 'diagnostic' },
                    response: { status: 'ok', result: 'test_result' }
                }),
                new Promise((_, reject) => 
                    setTimeout(() => reject(new Error('Context injection timeout')), 
                    config.timeout.component)
                )
            ]);
            
            if (result && !result.error) {
                console.log(`✅ Context injection successful (${Date.now() - startTime}ms)`);
                console.log(`Pattern ID: ${result.patternId}`);
                console.log(`Coherence: ${result.coherence}`);
                console.log(`Tier: ${result.tier}`);
                
                return true;
            } else {
                console.error(`❌ Context injection failed (${Date.now() - startTime}ms):`, result?.error || 'Unknown error');
                return false;
            }
        } catch (error) {
            console.error(`❌ Context injection failed (${Date.now() - startTime}ms):`, error.message);
            return false;
        }
    } catch (error) {
        console.error('❌ Context Injector creation failed:', error.message);
        return false;
    }
}

async function testFullServer() {
    console.log('\n=== Testing Full Server ===');
    
    try {
        // Create CoordinatedSEPServer instance
        console.log('Creating CoordinatedSEPServer instance...');
        const server = new CoordinatedSEPServer();
        
        // Initialize with timeout
        console.log(`Initializing server with ${config.timeout.component}ms timeout...`);
        const startTime = Date.now();
        
        try {
            const success = await Promise.race([
                server.initialize(),
                new Promise((_, reject) => 
                    setTimeout(() => reject(new Error('Server initialization timeout')), 
                    config.timeout.component)
                )
            ]);
            
            if (success) {
                console.log(`✅ Server initialization successful (${Date.now() - startTime}ms)`);
                
                // Test kernel analysis
                console.log('Testing server kernel analysis...');
                const result = await server.analyzeKernel({
                    kernelCode: `
                        __global__ void testKernel(float* data, int size) {
                            const int idx = blockIdx.x * blockDim.x + threadIdx.x;
                            if (idx < size) {
                                data[idx] = data[idx] * 2.0f;
                            }
                        }
                    `
                });
                
                console.log('Kernel analysis result:', result ? '✅ Success' : '❌ Failed');
                console.log(`Coherence score: ${result?.coherenceScore || 'N/A'}`);
                
                // Cleanup
                await server.shutdown();
                
                return true;
            } else {
                console.error(`❌ Server initialization failed (${Date.now() - startTime}ms)`);
                return false;
            }
        } catch (error) {
            console.error(`❌ Server initialization failed (${Date.now() - startTime}ms):`, error.message);
            return false;
        }
    } catch (error) {
        console.error('❌ Server creation failed:', error.message);
        return false;
    }
}

// Main diagnostic function
async function runDiagnostics() {
    console.log('=== SEP MCP Coordination Server Diagnostics ===');
    console.log('Environment:');
    console.log(`- Node.js: ${process.version}`);
    console.log(`- Platform: ${process.platform}`);
    console.log(`- Redis Enabled: ${config.redis.enabled}`);
    console.log(`- Redis Host: ${config.redis.host}`);
    console.log(`- Redis Port: ${config.redis.port}`);
    console.log(`- Redis Socket: ${config.redis.socket || 'N/A'}`);
    console.log(`- Redis Prefix: ${config.redis.prefix}`);
    
    // Run tests
    const redisConnected = await testRedisConnection();
    const redisIntegrationWorking = await testRedisIntegration();
    const cudaAssistantWorking = await testCudaAssistant();
    const contextInjectorWorking = await testContextInjector();
    const fullServerWorking = await testFullServer();
    
    // Summary
    console.log('\n=== Diagnostic Summary ===');
    console.log(`Redis Connection: ${redisConnected ? '✅ Working' : '❌ Failed'}`);
    console.log(`Redis Integration: ${redisIntegrationWorking ? '✅ Working' : '❌ Failed'}`);
    console.log(`CUDA Assistant: ${cudaAssistantWorking ? '✅ Working' : '❌ Failed'}`);
    console.log(`Context Injector: ${contextInjectorWorking ? '✅ Working' : '❌ Failed'}`);
    console.log(`Full Server: ${fullServerWorking ? '✅ Working' : '❌ Failed'}`);
    
    // Recommendations
    console.log('\n=== Recommendations ===');
    if (!redisConnected) {
        console.log('- Check if Redis is running on the specified host and port');
        console.log('- Verify Redis connection settings in environment variables');
        console.log('- Increase Redis connection timeout if needed');
    }
    
    if (!cudaAssistantWorking) {
        console.log('- Increase CUDA initialization timeout (currently 8000ms)');
        console.log('- Check CUDA dependencies and installation');
    }
    
    if (!contextInjectorWorking) {
        console.log('- Verify Redis connection for context injector');
        console.log('- Check implementation of missing methods in ManifoldContextInjector');
    }
    
    if (!fullServerWorking) {
        console.log('- Address individual component issues first');
        console.log('- Check server initialization sequence');
        console.log('- Increase initialization timeout if needed');
    }
}

// Run diagnostics
runDiagnostics().catch(error => {
    console.error('Diagnostic error:', error);
    process.exit(1);
});