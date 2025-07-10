/**
 * SEP-MCP Integration Main Script
 * Brings together SEP MCP Controller, Redis Memory Tier, and MCP servers
 */

import { SEPMCPController } from './sep_mcp_controller.js';
import ObsidianAdapter from './obsidian_adapter.js';
import { fileURLToPath } from 'url';

/**
 * Main integration class that provides a unified interface to the SEP-MCP system
 */
class SEPMCPIntegration {
    constructor(config = {}) {
        this.config = {
            valkey: {
                host: config.valkey?.host || '127.0.0.1',
                port: config.valkey?.port || 6379,
                socket: config.valkey?.socket || '/run/valkey/sep-manifold.sock',
                prefix: config.valkey?.prefix || 'sep:mcp:'
            },
            coherenceThresholds: {
                minimum: config.coherenceThresholds?.minimum || 0.1,
                mtm_promotion: config.coherenceThresholds?.mtm_promotion || 0.7,
                ltm_promotion: config.coherenceThresholds?.ltm_promotion || 0.9,
                demotion: config.coherenceThresholds?.demotion || 0.3
            },
            logging: {
                level: config.logging?.level || 'info',
                prefix: config.logging?.prefix || '[SEP-MCP]'
            }
        };
        
        this.controller = null;
        this.initialized = false;
    }
    
    /**
     * Initialize the integration system
     */
    async initialize() {
        if (this.initialized) {
            return;
        }

        try {
            // Create and initialize SEP MCP Controller
            this.controller = new SEPMCPController({
                valkeyHost: this.config.valkey.host,
                valkeyPort: this.config.valkey.port,
                valkeySocket: this.config.valkey.socket,
                valkeyPrefix: this.config.valkey.prefix,
                coherenceThresholds: this.config.coherenceThresholds
            });

            await this.controller.initialize();
            
            this.initialized = true;
            this.sep_log('info', 'Integration system initialized successfully');
        } catch (error) {
            this.sep_log('error', 'Failed to initialize integration system:', error);
            throw error;
        }
    }
    
    /**
     * Register an MCP server with the system
     */
    async registerServer(serverName, serverInstance, adapterClass) {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            const adapter = await this.controller.registerServer(
                serverName,
                serverInstance,
                adapterClass
            );
            
            this.sep_log('info', `Server ${serverName} registered successfully`);
            return adapter;
        } catch (error) {
            this.sep_log('error', `Failed to register server ${serverName}:`, error);
            throw error;
        }
    }
    
    /**
     * Execute a request through a registered server
     */
    async executeRequest(serverName, toolName, args) {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            const result = await this.controller.routeRequest(serverName, toolName, args);
            this.sep_log('debug', `Request ${serverName}:${toolName} executed successfully`);
            return result;
        } catch (error) {
            this.sep_log('error', `Failed to execute request ${serverName}:${toolName}:`, error);
            throw error;
        }
    }
    
    /**
     * Get capabilities of a registered server
     */
    async getServerCapabilities(serverName) {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            return await this.controller.getServerCapabilities(serverName);
        } catch (error) {
            this.sep_log('error', `Failed to get capabilities for server ${serverName}:`, error);
            throw error;
        }
    }
    
    /**
     * List all registered servers
     */
    async listServers() {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            return await this.controller.getAllServers();
        } catch (error) {
            this.sep_log('error', 'Failed to list servers:', error);
            throw error;
        }
    }

    /**
     * Deregister a server from the system
     */
    async deregisterServer(serverName) {
        if (!this.initialized) {
            await this.initialize();
        }

        try {
            await this.controller.deregisterServer(serverName);
            this.sep_log('info', `Server ${serverName} deregistered successfully`);
        } catch (error) {
            this.sep_log('error', `Failed to deregister server ${serverName}:`, error);
            throw error;
        }
    }
    
    /**
     * Shut down the integration system
     */
    async close() {
        if (this.controller) {
            await this.controller.close();
        }
        this.initialized = false;
        this.sep_log('info', 'Integration system shut down successfully');
    }
    
    /**
     * Internal logging helper
     */
    sep_log(level, ...args) {
        if (this.config.logging.level === 'none') return;
        
        const levels = ['error', 'warn', 'info', 'debug'];
        if (levels.indexOf(level) <= levels.indexOf(this.config.logging.level)) {
            console[level](`${this.config.logging.prefix}`, ...args);
        }
    }
}

/**
 * CLI interface for testing the integration
 */
async function main() {
    try {
        // Create integration instance
        const integration = new SEPMCPIntegration({
            logging: { level: 'info' }
        });
        
        // Initialize the system
        await integration.initialize();
        
        // Example: Register MCP-Obsidian server
        const obsidianServer = {
            serverConfig: {
                metadata: {
                    name: 'obsidian-server',
                    version: '1.0.0'
                },
                capabilities: {
                    tools: [
                        {
                            name: 'search_vault',
                            description: 'Search Obsidian vault',
                            inputSchema: {
                                type: 'object',
                                properties: {
                                    query: { type: 'string' }
                                },
                                required: ['query']
                            }
                        },
                        {
                            name: 'get_note',
                            description: 'Get Obsidian note by path',
                            inputSchema: {
                                type: 'object',
                                properties: {
                                    path: { type: 'string' }
                                },
                                required: ['path']
                            }
                        }
                    ]
                }
            }
        };
        
        await integration.registerServer('obsidian', obsidianServer, ObsidianAdapter);
        
        // List registered servers
        const servers = await integration.listServers();
        console.log('Registered servers:', servers);
        
        // Get server capabilities
        const capabilities = await integration.getServerCapabilities('obsidian');
        console.log('Obsidian server capabilities:', capabilities);
        
        // Example: Execute a search request
        const searchResult = await integration.executeRequest('obsidian', 'search_vault', {
            query: 'test'
        });
        console.log('Search result:', searchResult);
        
        // Clean shutdown
        await integration.close();
    } catch (error) {
        console.error('Integration test failed:', error);
        process.exit(1);
    }
}

// Run main if this script is executed directly
if (process.argv[1] === fileURLToPath(import.meta.url)) {
    main();
}

export { SEPMCPIntegration };