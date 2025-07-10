import { createClient, RedisClientType } from 'redis';

export class RedisService {
  private client: RedisClientType;
  private prefix: string;

  constructor(config: { url: string; prefix: string }) {
    this.client = createClient({ url: config.url });
    this.prefix = config.prefix;
  }

  async connect(): Promise<void> {
    await this.client.connect();
  }

  async ping(): Promise<string> {
    return await this.client.ping();
  }

  async storePattern(pattern: any): Promise<void> {
    const key = `${this.prefix}pattern:${pattern.id}`;
    await this.client.set(key, JSON.stringify(pattern));
  }

  async storeContext(context: any): Promise<void> {
    const key = `${this.prefix}context:${context.id}`;
    await this.client.set(key, JSON.stringify(context));
  }
}