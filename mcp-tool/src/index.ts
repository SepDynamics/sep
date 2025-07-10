import { McpServer } from '@modelcontextprotocol/sdk/server/mcp.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import { z } from 'zod';
import { EventEmitter } from 'events';
import { RedisService } from './services/redis.js';
import { SepService } from './services/sep.js';
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

    // Use EventEmitter for MCP server events
    this.on('error', this.handleError.bind(this));
    this.on('disconnect', this.handleDisconnect.bind(this));

    // Monitor MCP server connection status
    setInterval(() => {
      if (!this.mcpServer.isConnected()) {
        this.emit('disconnect');
      }
    }, 5000);
  }

  private async handleError(error: Error) {
    console.error('Error in Adaptive Bot:', error);
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
    // Monitor patterns
    this.mcpServer.tool(
      "monitor_patterns",
      z.object({
        contextId: z.string().describe("Context ID to monitor"),
        duration: z.number().optional().describe("Monitoring duration in seconds")
      }),
      async (args: MonitorPatternsParams, extra: RequestHandlerExtra<ServerRequest, ServerNotification>): Promise<ToolResponse> => {
        try {
          const result = await this.sepService.monitorPatterns(args.contextId, args.duration);
          return {
            content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
          };
        } catch (error) {
          const errorMessage = error instanceof Error ? error.message : String(error);
          console.error('Error in monitor_patterns:', errorMessage);
          return {
            content: [{ type: "text", text: `Error: ${errorMessage}` } as ToolContent],
            isError: true
          };
        }
      }
    );

    // Get insights
    this.mcpServer.tool(
      "get_insights",
      {
        contextId: z.string().describe("Context ID to analyze"),
        patternIds: z.array(z.string()).optional().describe("Specific pattern IDs to analyze")
      },
      async (args, extra) => {
        const result = await this.sepService.getInsights(args.contextId, args.patternIds);
        return {
          content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
        };
      }
    );

    // Track evolution
    this.mcpServer.tool(
      "track_evolution",
      {
        contextId: z.string().describe("Context ID to track"),
        startTime: z.string().optional().describe("Start time for evolution tracking")
      },
      async (args, extra) => {
        const result = await this.sepService.trackEvolution(args.contextId, args.startTime);
        return {
          content: [{ type: "text", text: JSON.stringify(result, null, 2) } as ToolContent]
        };
      }
    );

    // Process context
    this.mcpServer.tool(
      "process_context",
      {
        context: z.object({
          id: z.string(),
          patterns: z.array(z.any()),
          metadata: z.record(z.any())
        }).describe("Context object to process")
      },
      async (args, extra) => {
        const result = await this.sepService.processContext(args.context);
        await this.redisService.storeContext(result);
        return {
          content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
        };
      }
    );

    // Analyze context
    this.mcpServer.tool(
      "analyze_context",
      {
        contextId: z.string().describe("Context ID to analyze")
      },
      async (args, extra) => {
        const result = await this.sepService.analyzeContext(args.contextId);
        return {
          content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
        };
      }
    );

    // Evolve patterns
    this.mcpServer.tool(
      "evolve_patterns",
      {
        contextId: z.string().describe("Context ID containing patterns to evolve"),
        generations: z.number().optional().describe("Number of generations to evolve")
      },
      async (args, extra) => {
        const result = await this.sepService.evolvePatterns(args.contextId, args.generations);
        return {
          content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
        };
      }
    );

    // Analyze patterns
    this.mcpServer.tool(
      "analyze_patterns",
      {
        patterns: z.array(z.any()).describe("Patterns to analyze")
      },
      async (args, extra) => {
        const result = await this.sepService.analyzePatterns(args.patterns);
        return {
          content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
        };
      }
    );

    // Bridge patterns
    this.mcpServer.tool(
      "bridge_patterns",
      {
        sourceId: z.string().describe("Source pattern ID"),
        targetId: z.string().describe("Target pattern ID"),
        bridgeType: z.string().describe("Type of bridge to create")
      },
      async (args, extra) => {
        const result = await this.sepService.bridgePatterns(args.sourceId, args.targetId, args.bridgeType);
        return {
          content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
        };
      }
    );

    // Transform patterns
    this.mcpServer.tool(
      "transform_patterns",
      {
        patterns: z.array(z.any()).describe("Patterns to transform"),
        transformType: z.string().describe("Type of transformation to apply")
      },
      async (args, extra) => {
        const result = await this.sepService.transformPatterns(args.patterns, args.transformType);
        return {
          content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
        };
      }
    );

    // Process patterns
    this.mcpServer.tool(
      "process_patterns",
      {
        patterns: z.array(z.any()).describe("Patterns to process"),
        options: z.record(z.any()).optional().describe("Processing options")
      },
      async (args, extra) => {
        const result = await this.sepService.processPatterns(args.patterns);
        return {
          content: [{ type: "text", text: JSON.stringify(result, null, 2) }]
        };
      }
    );

    // Store patterns
    this.mcpServer.tool(
      "store_patterns",
      {
        patterns: z.array(z.any()).describe("Patterns to store"),
        contextId: z.string().describe("Context ID to store patterns in")
      },
      async (args, extra) => {
        for (const pattern of args.patterns) {
          await this.redisService.storePattern(pattern);
        }
        return {
          content: [{ type: "text", text: `Stored ${args.patterns.length} patterns` }]
        };
      }
    );
  }

  async start() {
    try {
      // Check dependencies before starting
      const dependenciesOk = await this.checkDependencies();
      if (!dependenciesOk) {
        throw new Error('Required dependencies are not available');
      }

      // Connect Redis client
      await this.redisService.connect();

      // Start receiving messages on stdin and sending messages on stdout
      const transport = new StdioServerTransport();
      await this.mcpServer.connect(transport);
      
      this.isConnected = true;
      console.error('Adaptive Bot MCP server running on stdio');
    } catch (error) {
      console.error('Failed to start server:', error);
      throw error;
    }
  }
}

// Start server
const bot = new AdaptiveBot();
bot.start().catch(error => {
  console.error('Failed to start Adaptive Bot:', error);
  process.exit(1);
});