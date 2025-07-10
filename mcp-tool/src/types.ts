import { McpServer } from '@modelcontextprotocol/sdk/server/mcp.js';

export interface Pattern {
  id: string;
  [key: string]: any;
}

export interface Context {
  id: string;
  patterns: Pattern[];
  metadata: Record<string, any>;
}

// Extend McpServer to include connection status
declare module '@modelcontextprotocol/sdk/server/mcp.js' {
  interface McpServer {
    isConnected(): boolean;
  }
}

export interface MonitorPatternsParams {
  contextId: string;
  duration?: number;
}

export interface GetInsightsParams {
  contextId: string;
  patternIds?: string[];
}

export interface TrackEvolutionParams {
  contextId: string;
  startTime?: string;
}

export interface ProcessContextParams {
  context: Context;
}

export interface AnalyzeContextParams {
  contextId: string;
}

export interface EvolvePatternsParams {
  contextId: string;
  generations?: number;
}

export interface AnalyzePatternsParams {
  patterns: Pattern[];
}

export interface BridgePatternsParams {
  sourceId: string;
  targetId: string;
  bridgeType: string;
}

export interface TransformPatternsParams {
  patterns: Pattern[];
  transformType: string;
}

export interface ProcessPatternsParams {
  patterns: Pattern[];
}

export interface StorePatternsParams {
  patterns: Pattern[];
  contextId: string;
}

export interface AdaptiveBotEvents {
  error: (error: Error | unknown) => void;
  disconnect: () => void;
}

export interface ToolContent {
  [key: string]: unknown;
  type: "text";
  text: string;
  _meta?: Record<string, unknown>;
}

export interface ToolResponse {
  [key: string]: unknown;
  content: ToolContent[];
  _meta?: Record<string, unknown>;
  isError?: boolean;
}