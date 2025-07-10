import { McpServer } from '@modelcontextprotocol/sdk/server/mcp.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import { EventEmitter } from 'events';
import { z } from 'zod';
import { RedisService } from './src/services/redis.js';
import { SepService } from './src/services/sep.js';

export class CoordinatedHelper extends EventEmitter {
  private mcp: McpServer;
  private redis: RedisService;
  private sep: SepService;
  private connected = false;

  constructor() {
    super();
    this.redis = new RedisService({
      url: process.env.REDIS_URL || 'redis://localhost:6379',
      prefix: 'coordinated:'
    });
    this.sep = new SepService({ baseURL: process.env.SEP_ENGINE_URL || 'http://localhost:8080' });
    this.mcp = new McpServer({ name: 'coordinated-helper', version: '0.0.1' });
    this.registerTools();
  }

  private registerTools() {
    this.mcp.tool(
      'coordinated_ping',
      {},
      async () => ({ content: [{ type: 'text', text: 'pong' }] })
    );

    this.mcp.tool(
      'store_context',
      {
        context: z.object({
          id: z.string(),
          patterns: z.array(z.any()),
          metadata: z.record(z.any())
        })
      },
      async (args) => {
        await this.redis.storeContext(args.context);
        return { content: [{ type: 'text', text: 'context stored' }] };
      }
    );
  }

  async start() {
    await this.redis.connect();
    const transport = new StdioServerTransport();
    await this.mcp.connect(transport);
    this.connected = true;
    console.error('Coordinated helper running on stdio');
  }
}

if (process.argv[1] === new URL(import.meta.url).pathname) {
  const helper = new CoordinatedHelper();
  helper.start().catch((err) => {
    console.error('Failed to start coordinated helper:', err);
    process.exit(1);
  });
}
