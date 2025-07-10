// Define simple types that match what the SDK expects
export interface ServerRequest {
  id: string;
  params?: Record<string, any>;
  [key: string]: any;
}

export interface ServerNotification {
  type: string;
  [key: string]: any;
}

export interface RequestHandlerExtra<TRequest, TNotification> {
  request: TRequest;
  requestId: string;
  sendNotification: (notification: TNotification) => Promise<void>;
  [key: string]: any;
}

// Define the tool content types
export interface ToolContent {
  type: string;
  text: string;
  [key: string]: unknown;
}

export interface ToolResponse {
  content: ToolContent[];
  isError?: boolean;
  [key: string]: unknown;
}