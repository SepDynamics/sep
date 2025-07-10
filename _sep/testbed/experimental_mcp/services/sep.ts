import axios, { AxiosInstance } from 'axios';

export class SepService {
  private client: AxiosInstance;

  constructor(config: { baseURL: string }) {
    this.client = axios.create({
      baseURL: config.baseURL,
      timeout: 5000
    });
  }

  async healthCheck(): Promise<boolean> {
    try {
      const response = await this.client.get('/health');
      return response.status === 200;
    } catch (error) {
      return false;
    }
  }

  async monitorPatterns(contextId: string, duration?: number): Promise<any> {
    const response = await this.client.post('/patterns/monitor', {
      contextId,
      duration
    });
    return response.data;
  }

  async getInsights(contextId: string, patternIds?: string[]): Promise<any> {
    const response = await this.client.post('/patterns/insights', {
      contextId,
      patternIds
    });
    return response.data;
  }

  async trackEvolution(contextId: string, startTime?: string): Promise<any> {
    const response = await this.client.post('/patterns/evolution', {
      contextId,
      startTime
    });
    return response.data;
  }

  async processContext(context: any): Promise<any> {
    const response = await this.client.post('/context/process', context);
    return response.data;
  }

  async analyzeContext(contextId: string): Promise<any> {
    const response = await this.client.get(`/context/${contextId}/analyze`);
    return response.data;
  }

  async evolvePatterns(contextId: string, generations?: number): Promise<any> {
    const response = await this.client.post('/patterns/evolve', {
      contextId,
      generations
    });
    return response.data;
  }

  async analyzePatterns(patterns: any[]): Promise<any> {
    const response = await this.client.post('/patterns/analyze', { patterns });
    return response.data;
  }

  async bridgePatterns(sourceId: string, targetId: string, bridgeType: string): Promise<any> {
    const response = await this.client.post('/patterns/bridge', {
      sourceId,
      targetId,
      bridgeType
    });
    return response.data;
  }

  async transformPatterns(patterns: any[], transformType: string): Promise<any> {
    const response = await this.client.post('/patterns/transform', {
      patterns,
      transformType
    });
    return response.data;
  }

  async processPatterns(patterns: any[]): Promise<any> {
    const response = await this.client.post('/patterns/process', { patterns });
    return response.data;
  }
}