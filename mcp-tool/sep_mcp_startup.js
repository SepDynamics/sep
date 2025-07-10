#!/usr/bin/env node
/**
 * SEP MCP Coordination Server Startup Script
 * 
 * This script implements the four-phase process for starting the SEP MCP Coordination Server:
 * 1. Start the server using /workspace/adaptive-bot/sep_mcp_server.js
 * 2. Integrate Codeex with the MCP runtime
 * 3. Activate the AutonomousBrain component
 * 4. Make the system persistent
 */

import { spawn } from 'child_process';
import { join } from 'path';
import { existsSync, mkdirSync, createWriteStream } from 'fs';
import { createClient } from 'redis';

// Configuration
const config = {
    server: {
        path: join(process.cwd(), 'sep_mcp_server.js'),
        args: [],
        cwd: process.cwd() // Explicitly set the current working directory
    },
    redis: {
        host: process.env.SEP_REDIS_HOST || '127.0.0.1',
        port: parseInt(process.env.SEP_REDIS_PORT) || 6379
    },
    codeex: {
        enabled: true,
        integrationPath: join(process.cwd(), 'codeex_integration.js')
    },
    autonomousBrain: {
        enabled: true,
        activationDelay: 5000 // ms
    },
    persistence: {
        enabled: true,
        dataDir: join(process.cwd(), 'data'),
        checkpointInterval: 60000 // ms
    },
    logDir: join(process.cwd(), 'logs')
};

// Ensure log directory exists
if (!existsSync(config.logDir)) {
    mkdirSync(config.logDir, { recursive: true });
}

// Ensure data directory exists if persistence is enabled
if (config.persistence.enabled && !existsSync(config.persistence.dataDir)) {
    mkdirSync(config.persistence.dataDir, { recursive: true });
}

// Create log streams
const serverLogPath = join(config.logDir, 'sep_server.log');
const serverLogStream = createWriteStream(serverLogPath, { flags: 'a' });

// Log helper
function log(message) {
    const timestamp = new Date().toISOString();
    const formattedMessage = `[${timestamp}] ${message}`;
    console.log(formattedMessage);
    serverLogStream.write(formattedMessage + '\n');
}

// Phase 1: Start the server
async function startServer() {
    log('Phase 1: Starting SEP MCP Coordination Server');
    
    // Set environment variables
    process.env.SEP_REDIS_ENABLED = 'true';
    
    // Start the server process
    const serverProcess = spawn('node', [config.server.path, ...config.server.args], {
        stdio: ['pipe', 'pipe', 'pipe'],
        detached: true,
        cwd: config.server.cwd // Use the explicitly set working directory
    });
    
    // Handle server output
    serverProcess.stdout.on('data', (data) => {
        const output = data.toString().trim();
        log(`Server stdout: ${output}`);
    });
    
    serverProcess.stderr.on('data', (data) => {
        const output = data.toString().trim();
        log(`Server stderr: ${output}`);
    });
    
    // Handle server exit
    serverProcess.on('exit', (code, signal) => {
        log(`Server process exited with code ${code} and signal ${signal}`);
        if (code !== 0) {
            log('Server process terminated unexpectedly. Restarting...');
            startServer();
        }
    });
    
    // Wait for server to initialize
    return new Promise((resolve) => {
        setTimeout(() => {
            log('Server initialization period completed');
            resolve(serverProcess);
        }, 5000);
    });
}

// Phase 2: Integrate Codeex with MCP runtime
async function integrateCodeex(serverProcess) {
    if (!config.codeex.enabled) {
        log('Phase 2: Codeex integration disabled, skipping');
        return serverProcess;
    }
    
    log('Phase 2: Integrating Codeex with MCP runtime');
    
    try {
        // Check if Codeex integration file exists
        if (!existsSync(config.codeex.integrationPath)) {
            throw new Error('Codeex integration module not found');
        }

        log('Loading Codeex integration module');
        const codeexIntegration = await import(config.codeex.integrationPath);

        log('Initializing Codeex integration');
        await codeexIntegration.initialize({
            serverProcess,
            redisConfig: config.redis
        });

        log('Codeex integration completed successfully');
    } catch (error) {
        log(`Codeex integration error: ${error.message}`);
    }
    
    return serverProcess;
}

// Phase 3: Activate AutonomousBrain component
async function activateAutonomousBrain(serverProcess) {
    if (!config.autonomousBrain.enabled) {
        log('Phase 3: AutonomousBrain activation disabled, skipping');
        return serverProcess;
    }
    
    log('Phase 3: Activating AutonomousBrain component');
    
    // Wait for specified delay
    await new Promise(resolve => setTimeout(resolve, config.autonomousBrain.activationDelay));
    
    try {
        // Connect to Redis to send activation signal
        const redisClient = createClient({
            socket: {
                host: config.redis.host,
                port: config.redis.port
            }
        });
        
        await redisClient.connect();
        
        // Send activation signal
        await redisClient.set('sep:autonomousBrain:active', 'true');
        await redisClient.publish('sep:events', JSON.stringify({
            type: 'autonomousBrain:activate',
            timestamp: new Date().toISOString()
        }));
        
        log('AutonomousBrain activation signal sent');
        
        // Cleanup
        await redisClient.quit();
    } catch (error) {
        log(`AutonomousBrain activation error: ${error.message}`);
    }
    
    return serverProcess;
}

// Phase 4: Make the system persistent
async function makePersistent(serverProcess) {
    if (!config.persistence.enabled) {
        log('Phase 4: Persistence disabled, skipping');
        return serverProcess;
    }
    
    log('Phase 4: Making the system persistent');
    
    // Set up checkpoint interval
    const checkpointInterval = setInterval(async () => {
        log('Creating system checkpoint');
        
        try {
            // Connect to Redis
            const redisClient = createClient({
                socket: {
                    host: config.redis.host,
                    port: config.redis.port
                }
            });
            
            await redisClient.connect();
            
            // Trigger checkpoint creation
            await redisClient.publish('sep:events', JSON.stringify({
                type: 'system:checkpoint',
                timestamp: new Date().toISOString(),
                path: join(config.persistence.dataDir, `checkpoint-${Date.now()}.json`)
            }));
            
            log('Checkpoint signal sent');
            
            // Cleanup
            await redisClient.quit();
        } catch (error) {
            log(`Checkpoint error: ${error.message}`);
        }
    }, config.persistence.checkpointInterval);
    
    // Handle process termination
    process.on('SIGINT', async () => {
        log('Received SIGINT signal, creating final checkpoint before exit');
        
        clearInterval(checkpointInterval);
        
        try {
            // Connect to Redis
            const redisClient = createClient({
                socket: {
                    host: config.redis.host,
                    port: config.redis.port
                }
            });
            
            await redisClient.connect();
            
            // Trigger final checkpoint
            await redisClient.publish('sep:events', JSON.stringify({
                type: 'system:finalCheckpoint',
                timestamp: new Date().toISOString(),
                path: join(config.persistence.dataDir, `final-checkpoint-${Date.now()}.json`)
            }));
            
            log('Final checkpoint signal sent');
            
            // Cleanup
            await redisClient.quit();
        } catch (error) {
            log(`Final checkpoint error: ${error.message}`);
        }
        
        // Kill server process
        if (serverProcess) {
            serverProcess.kill();
        }
        
        process.exit(0);
    });
    
    return serverProcess;
}

// Main function
async function main() {
    log('Starting SEP MCP Coordination Server with four-phase process');
    
    try {
        // Execute the four phases sequentially
        const serverProcess = await startServer();
        await integrateCodeex(serverProcess);
        await activateAutonomousBrain(serverProcess);
        await makePersistent(serverProcess);
        
        log('All phases completed successfully. Server is running.');
    } catch (error) {
        log(`Error during startup: ${error.message}`);
        process.exit(1);
    }
}

// Run the main function
main().catch(error => {
    log(`Unhandled error: ${error.message}`);
    process.exit(1);
});