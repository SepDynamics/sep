/**
 * SEP Engine Codebase Search MCP Server
 *
 * This is the main entry point for the codebase search MCP server.
 * It provides semantic search capabilities for the SEP Engine codebase.
 */

import { MCPServer } from './mcp-server-mock';
import { CodebaseSearchTool } from './tools/codebase-search';
import { FileContentTool } from './tools/file-content';
import { SearchService } from './services/search-service';

/**
 * Start the MCP server with codebase search capabilities
 */
export function startServer(): void {
  // Create the search service
  const searchService = new SearchService({
    rootDir: process.cwd(),
    indexingPatterns: ['**/*.{cpp,hpp,h,cu,ts,js,json,md}'],
    excludePatterns: ['**/node_modules/**', '**/dist/**', '**/build/**', '**/.git/**']
  });

  // Initialize the search service
  searchService.initialize().then(() => {
    console.log('Search service initialized');
  }).catch((error: Error) => {
    console.error('Failed to initialize search service:', error);
  });

  // Create the MCP server
  const server = new MCPServer({
    name: 'codebase-search',
    description: 'Provides semantic search capabilities for the SEP Engine codebase'
  });

  // Register the tools
  server.registerTool(new CodebaseSearchTool(searchService));
  server.registerTool(new FileContentTool());

  // Start the server
  server.start().then(() => {
    console.log('Codebase search MCP server started');
  }).catch((error: Error) => {
    console.error('Failed to start codebase search MCP server:', error);
  });
}

// If this file is run directly, start the server
if (require.main === module) {
  startServer();
}