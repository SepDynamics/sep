import fetch from 'node-fetch';

export default class SepClient {
  constructor(config = {}) {
    this.baseUrl = config.baseUrl || 'http://localhost:8080';
    this.timeout = config.timeout || 5000;
    this.retries = config.retries || 3;
  }

  async connect() {
    try {
      const response = await fetch(`${this.baseUrl}/status`);
      if (!response.ok) throw new Error('Connection failed');
      return true;
    } catch (error) {
      console.error('SEP Engine connection error:', error);
      return false;
    }
  }

  async processPattern(pattern) {
    const url = `${this.baseUrl}/api/pattern`;
    const options = {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(pattern),
      timeout: this.timeout
    };

    return this.retryRequest(() => fetch(url, options));
  }

  async getMemoryState(tier) {
    const url = `${this.baseUrl}/api/memory/${tier}`;
    return this.retryRequest(() => fetch(url));
  }

  async evolvePatterns(patterns) {
    const url = `${this.baseUrl}/api/evolve`;
    const options = {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({ patterns }),
      timeout: this.timeout
    };

    return this.retryRequest(() => fetch(url, options));
  }

  async retryRequest(requestFn) {
    let lastError;

    for (let i = 0; i < this.retries; i++) {
      try {
        const response = await requestFn();
        if (!response.ok) throw new Error(`HTTP ${response.status}`);
        return await response.json();
      } catch (error) {
        console.error(`Attempt ${i + 1} failed:`, error);
        lastError = error;
        await new Promise(resolve => setTimeout(resolve, 1000 * (i + 1)));
      }
    }

    throw new Error(`Failed after ${this.retries} attempts: ${lastError}`);
  }

  async sync() {
    const url = `${this.baseUrl}/api/sync`;
    return this.retryRequest(() => fetch(url, { method: 'POST' }));
  }
}