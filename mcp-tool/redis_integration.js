import { createClient } from 'redis';

class RedisIntegration {
  constructor() {
    this.client = null;
    this.fallbackMap = new Map();
    this.isConnected = false;
    this.retryAttempts = 0;
    this.maxRetries = 3;
  }

  async connect(config = {}) {
    const defaultConfig = {
      url: 'redis://localhost:6379',
      retryStrategy: (times) => Math.min(times * 100, 3000),
    };

    try {
      this.client = createClient({ ...defaultConfig, ...config });
      
      this.client.on('error', (err) => {
        console.error('Redis Error:', err);
        this.isConnected = false;
        this.fallbackToMemory();
      });

      this.client.on('connect', () => {
        console.log('Redis connected');
        this.isConnected = true;
        this.retryAttempts = 0;
      });

      await this.client.connect();
    } catch (err) {
      console.error('Redis Connection Error:', err);
      this.fallbackToMemory();
    }
  }

  async set(key, value, options = {}) {
    try {
      if (this.isConnected) {
        return await this.client.set(key, JSON.stringify(value), options);
      }
      return this.fallbackMap.set(key, value);
    } catch (err) {
      console.error('Redis Set Error:', err);
      return this.fallbackMap.set(key, value);
    }
  }

  async get(key) {
    try {
      if (this.isConnected) {
        const value = await this.client.get(key);
        return value ? JSON.parse(value) : null;
      }
      return this.fallbackMap.get(key);
    } catch (err) {
      console.error('Redis Get Error:', err);
      return this.fallbackMap.get(key);
    }
  }

  async delete(key) {
    try {
      if (this.isConnected) {
        return await this.client.del(key);
      }
      return this.fallbackMap.delete(key);
    } catch (err) {
      console.error('Redis Delete Error:', err);
      return this.fallbackMap.delete(key);
    }
  }

  fallbackToMemory() {
    console.warn('Falling back to in-memory storage');
    if (this.retryAttempts < this.maxRetries) {
      this.retryAttempts++;
      setTimeout(() => this.connect(), 1000 * this.retryAttempts);
    }
  }

  async disconnect() {
    if (this.isConnected) {
      await this.client.quit();
      this.isConnected = false;
    }
    this.fallbackMap.clear();
  }
}

export default new RedisIntegration();