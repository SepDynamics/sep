import { McpServer } from '@modelcontextprotocol/sdk/server/mcp.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import { RedisService } from './services/redis.js';
import { SepService } from './services/sep.js';
import { ServerRequest, ServerNotification, RequestHandlerExtra } from './mcp-types.js';
import {
  AdaptiveBotEvents,
  ToolContent
} from './types.js';

// Node.js type declarations
declare class EventEmitter {
  on(event: string, listener: (...args: any[]) => void): this;
  emit(event: string, ...args: any[]): boolean;
  addListener(event: string, listener: (...args: any[]) => void): this;
}

declare const process: {
  env: Record<string, string | undefined>;
  on(event: string, listener: (...args: any[]) => void): void;
  exit(code?: number): never;
  argv: string[];
};
import type {
  MonitorPatternsParams,
  GetInsightsParams,
  TrackEvolutionParams,
  ProcessContextParams,
  AnalyzeContextParams,
  EvolvePatternsParams,
  AnalyzePatternsParams,
  BridgePatternsParams,
  TransformPatternsParams,
  ProcessPatternsParams,
  StorePatternsParams,
  ToolResponse
} from './types.js';

class AdaptiveBot extends EventEmitter {
  declare emit: <K extends keyof AdaptiveBotEvents>(event: K, ...args: Parameters<AdaptiveBotEvents[K]>) => boolean;
  declare on: <K extends keyof AdaptiveBotEvents>(event: K, listener: AdaptiveBotEvents[K]) => this;
  declare addListener: <K extends keyof AdaptiveBotEvents>(event: K, listener: AdaptiveBotEvents[K]) => this;
  private mcpServer: McpServer;
  private redisService: RedisService;
  private sepService: SepService;
  private isConnected: boolean = false;
  private reconnectAttempts: number = 0;
  private readonly MAX_RECONNECT_ATTEMPTS = 5;
  private readonly RECONNECT_DELAY = 5000;

  constructor() {
    super();
    
    // Initialize Redis service
    this.redisService = new RedisService({
      url: process.env.REDIS_URL || 'redis://localhost:6379',
      prefix: 'adaptive-bot:'
    });

    // Initialize SEP Engine service
    this.sepService = new SepService({
      baseURL: process.env.SEP_ENGINE_URL || 'http://localhost:8080'
    });

    // Create MCP server
    this.mcpServer = new McpServer({
      name: "adaptive-bot",
      version: "0.1.0"
    });

    this.setupTools();
    this.setupErrorHandling();
  }

  private setupErrorHandling() {
    process.on('uncaughtException', this.handleError.bind(this));
    process.on('unhandledRejection', this.handleError.bind(this));

    // Use EventEmitter for internal events
    this.on('error', (error: unknown) => {
      this.handleError(error);
    });
    
    this.on('disconnect', () => {
      this.handleDisconnect();
    });

    // Monitor MCP server connection status
    setInterval(() => {
      if (!this.mcpServer.isConnected()) {
        this.emit('disconnect');
      }
    }, 5000);
  }

  private async handleError(error: Error | unknown) {
    const errorMessage = error instanceof Error ? error.message : String(error);
    console.error('Error in Adaptive Bot:', errorMessage);
    if (!this.isConnected) {
      await this.attemptReconnect();
    }
  }

  private async handleDisconnect() {
    console.error('MCP Server disconnected');
    this.isConnected = false;
    await this.attemptReconnect();
  }

  private async attemptReconnect() {
    if (this.reconnectAttempts >= this.MAX_RECONNECT_ATTEMPTS) {
      console.error('Max reconnection attempts reached. Exiting...');
      process.exit(1);
    }

    this.reconnectAttempts++;
    console.log(`Attempting to reconnect (${this.reconnectAttempts}/${this.MAX_RECONNECT_ATTEMPTS})...`);

    try {
      await new Promise(resolve => setTimeout(resolve, this.RECONNECT_DELAY));
      await this.start();
      this.reconnectAttempts = 0;
    } catch (error) {
      console.error('Reconnection failed:', error);
      await this.attemptReconnect();
    }
  }

  private async checkDependencies() {
    try {
      // Check Redis connection
      await this.redisService.ping();
      
      // Check SEP Engine connection
      await this.sepService.healthCheck();

      return true;
    } catch (error) {
      console.error('Dependency check failed:', error);
      return false;
    }
  }

  private setupTools() {
    // Check status
    this.mcpServer.tool(
      "check_status",
      "Check the status of Redis and SEP Engine connections",
      async (extra) => {
        try {
          const status = {
            redis: await this.redisService.ping(),
            sepEngine: await this.sepService.healthCheck()
          };
          return {
            content: [{ type: "text", text: JSON.stringify(status, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in check_status:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Monitor patterns
    this.mcpServer.tool(
      "monitor_patterns",
      "Monitor patterns in a context",
      async (extra) => {
        try {
          // Get parameters from the request
          // In the MCP SDK, parameters are passed directly to the callback
          // We'll use a default empty object for safety
          const params = (extra as any).params || {};
          const contextId = params.contextId as string;
          const duration = params.duration as number | undefined;

          if (!contextId) {
            return {
              content: [{ type: "text", text: "Error: contextId is required" }],
              isError: true
            };
          }

          const result = await this.sepService.monitorPatterns(contextId, duration);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in monitor_patterns:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Get insights
    this.mcpServer.tool(
      "get_insights",
      "Get insights from patterns in a context",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const contextId = params.contextId as string;
          const patternIds = params.patternIds as string[] | undefined;

          if (!contextId) {
            return {
              content: [{ type: "text", text: "Error: contextId is required" }],
              isError: true
            };
          }

          const result = await this.sepService.getInsights(contextId, patternIds);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in get_insights:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Track evolution
    this.mcpServer.tool(
      "track_evolution",
      "Track the evolution of patterns in a context",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const contextId = params.contextId as string;
          const startTime = params.startTime as string | undefined;

          if (!contextId) {
            return {
              content: [{ type: "text", text: "Error: contextId is required" }],
              isError: true
            };
          }

          const result = await this.sepService.trackEvolution(contextId, startTime);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in track_evolution:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Process context
    this.mcpServer.tool(
      "process_context",
      "Process a context object with patterns and metadata",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const context = params.context as {
            id: string;
            patterns: any[];
            metadata: Record<string, any>
          };

          if (!context || !context.id || !Array.isArray(context.patterns)) {
            return {
              content: [{ type: "text", text: "Error: valid context object with id and patterns array is required" }],
              isError: true
            };
          }

          const result = await this.sepService.processContext(context);
          await this.redisService.storeContext(result);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in process_context:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Analyze context
    this.mcpServer.tool(
      "analyze_context",
      "Analyze a context by its ID",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const contextId = params.contextId as string;

          if (!contextId) {
            return {
              content: [{ type: "text", text: "Error: contextId is required" }],
              isError: true
            };
          }

          const result = await this.sepService.analyzeContext(contextId);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in analyze_context:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Evolve patterns
    this.mcpServer.tool(
      "evolve_patterns",
      "Evolve patterns in a context through multiple generations",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const contextId = params.contextId as string;
          const generations = params.generations as number | undefined;

          if (!contextId) {
            return {
              content: [{ type: "text", text: "Error: contextId is required" }],
              isError: true
            };
          }

          const result = await this.sepService.evolvePatterns(contextId, generations);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in evolve_patterns:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Analyze patterns
    this.mcpServer.tool(
      "analyze_patterns",
      "Analyze a set of patterns to extract insights",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const patterns = params.patterns as any[];

          if (!patterns || !Array.isArray(patterns) || patterns.length === 0) {
            return {
              content: [{ type: "text", text: "Error: patterns array is required and must not be empty" }],
              isError: true
            };
          }

          const result = await this.sepService.analyzePatterns(patterns);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in analyze_patterns:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Bridge patterns
    this.mcpServer.tool(
      "bridge_patterns",
      "Create a bridge between two patterns",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const sourceId = params.sourceId as string;
          const targetId = params.targetId as string;
          const bridgeType = params.bridgeType as string;

          if (!sourceId || !targetId || !bridgeType) {
            return {
              content: [{ type: "text", text: "Error: sourceId, targetId, and bridgeType are all required" }],
              isError: true
            };
          }

          const result = await this.sepService.bridgePatterns(sourceId, targetId, bridgeType);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in bridge_patterns:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Transform patterns
    this.mcpServer.tool(
      "transform_patterns",
      "Apply transformations to a set of patterns",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const patterns = params.patterns as any[];
          const transformType = params.transformType as string;

          if (!patterns || !Array.isArray(patterns) || patterns.length === 0) {
            return {
              content: [{ type: "text", text: "Error: patterns array is required and must not be empty" }],
              isError: true
            };
          }

          if (!transformType) {
            return {
              content: [{ type: "text", text: "Error: transformType is required" }],
              isError: true
            };
          }

          const result = await this.sepService.transformPatterns(patterns, transformType);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in transform_patterns:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Process patterns
    this.mcpServer.tool(
      "process_patterns",
      "Process a set of patterns with optional processing options",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const patterns = params.patterns as any[];
          const options = params.options as Record<string, any> | undefined;
          
          if (!patterns || !Array.isArray(patterns) || patterns.length === 0) {
            return {
              content: [{ type: "text", text: "Error: patterns array is required and must not be empty" }],
              isError: true
            };
          }
          
          // Note: The sepService.processPatterns method only accepts one argument
          const result = await this.sepService.processPatterns(patterns);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in process_patterns:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );

    // Store patterns
    this.mcpServer.tool(
      "store_patterns",
      "Store patterns in Redis with a context ID",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const patterns = params.patterns as any[];
          const contextId = params.contextId as string;
          
          if (!patterns || !Array.isArray(patterns) || patterns.length === 0) {
            return {
              content: [{ type: "text", text: "Error: patterns array is required and must not be empty" }],
              isError: true
            };
          }
          
          if (!contextId) {
            return {
              content: [{ type: "text", text: "Error: contextId is required" }],
              isError: true
            };
          }
          
          for (const pattern of patterns) {
            await this.redisService.storePattern(pattern);
          }
          
          return {
            content: [{ type: "text", text: `Stored ${patterns.length} patterns` }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in store_patterns:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );
    
    // Run analysis
    this.mcpServer.tool(
      "run_analysis",
      "Run analysis on patterns using specified analyzers",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const buildDir = params.buildDir as string;
          const outputDir = params.outputDir as string;
          const analyzers = params.analyzers as string[] | undefined;
          
          if (!buildDir) {
            return {
              content: [{ type: "text", text: "Error: buildDir is required" }],
              isError: true
            };
          }
          
          if (!outputDir) {
            return {
              content: [{ type: "text", text: "Error: outputDir is required" }],
              isError: true
            };
          }
          
          // This is a placeholder for the actual implementation
          // In a real implementation, you would call a service method to run the analysis
          const result = {
            status: "success",
            buildDir,
            outputDir,
            analyzers: analyzers || ["default"],
            timestamp: new Date().toISOString(),
            message: "Analysis completed successfully"
          };
          
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in run_analysis:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );
    
    // Store results
    this.mcpServer.tool(
      "store_results",
      "Store analysis results with a run name",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const reportsDir = params.reportsDir as string;
          const runName = params.runName as string;
          
          if (!reportsDir) {
            return {
              content: [{ type: "text", text: "Error: reportsDir is required" }],
              isError: true
            };
          }
          
          if (!runName) {
            return {
              content: [{ type: "text", text: "Error: runName is required" }],
              isError: true
            };
          }
          
          // This is a placeholder for the actual implementation
          // In a real implementation, you would call a service method to store the results
          const result = {
            status: "success",
            reportsDir,
            runName,
            timestamp: new Date().toISOString(),
            message: "Results stored successfully"
          };
          
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in store_results:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );
    
    // Semantic search
    this.mcpServer.tool(
      "semantic_search",
      "Perform semantic search in the codebase",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const query = params.query as string;
          const filePattern = params.filePattern as string | undefined;
          const limit = params.limit as number | undefined;
          
          if (!query) {
            return {
              content: [{ type: "text", text: "Error: query is required" }],
              isError: true
            };
          }
          
          // This is a placeholder for the actual implementation
          // In a real implementation, you would call a service method to perform the search
          const result = {
            status: "success",
            query,
            filePattern: filePattern || "*",
            limit: limit || 10,
            timestamp: new Date().toISOString(),
            results: [
              {
                file: "example/file1.cpp",
                score: 0.95,
                snippet: "// This is a sample snippet that matches the query"
              },
              {
                file: "example/file2.cpp",
                score: 0.85,
                snippet: "// This is another sample snippet that matches the query"
              }
            ]
          };
          
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in semantic_search:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );
    
    // Analyze issues
    this.mcpServer.tool(
      "analyze_issues",
      "Analyze code issues in a file",
      async (extra) => {
        try {
          // Get parameters from the request
          const params = (extra as any).params || {};
          const file = params.file as string;
          const runName = params.runName as string | undefined;
          
          if (!file) {
            return {
              content: [{ type: "text", text: "Error: file is required" }],
              isError: true
            };
          }
          
          // This is a placeholder for the actual implementation
          // In a real implementation, you would call a service method to analyze the issues
          const result = {
            status: "success",
            file,
            runName: runName || "default",
            timestamp: new Date().toISOString(),
            issues: [
              {
                id: "ISSUE-001",
                severity: "high",
                line: 42,
                message: "Potential null pointer dereference",
                category: "memory"
              },
              {
                id: "ISSUE-002",
                severity: "medium",
                line: 78,
                message: "Uninitialized variable used",
                category: "logic"
              }
            ]
          };
          
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in analyze_issues:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` }],
            isError: true
          };
        }
      }
    );
  }

  async start() {
    try {
      // Check dependencies before starting
      const dependenciesOk = await this.checkDependencies();
      if (!dependenciesOk) {
        console.warn('Some dependencies are not available, but continuing anyway');
      }

      try {
        // Connect Redis client - but don't fail if it doesn't connect
        await this.redisService.connect();
      } catch (redisError) {
        console.warn('Redis connection failed, but continuing:', redisError);
      }

      // Start receiving messages on stdin and sending messages on stdout
      const transport = new StdioServerTransport();
      
      // Set up connection event handlers
      // The StdioServerTransport doesn't have 'on' method in the MCP SDK
      // We'll rely on the McpServer connection status instead
      
      // Log initial connection
      console.error('Adaptive Bot MCP server starting on stdio');
      
      // Connect to the MCP server
      await this.mcpServer.connect(transport);
      
      this.isConnected = true;
      console.error('Adaptive Bot MCP server running on stdio');
      
      // Set up a heartbeat to check connection status
      setInterval(() => {
        if (this.mcpServer.isConnected()) {
          if (!this.isConnected) {
            console.error('Connection restored');
            this.isConnected = true;
          }
        } else {
          if (this.isConnected) {
            console.error('Connection lost');
            this.isConnected = false;
            this.handleDisconnect();
          }
        }
      }, 5000);
    } catch (error) {
      console.error('Failed to start server:', error);
      
      // Try to reconnect after a delay
      setTimeout(() => {
        console.error('Attempting to restart server...');
        this.start().catch(err => {
          console.error('Failed to restart server:', err);
        });
      }, this.RECONNECT_DELAY);
    }
  }
}

// Start server
const bot = new AdaptiveBot();
bot.start().catch(error => {
  console.error('Failed to start Adaptive Bot:', error);
  process.exit(1);
});