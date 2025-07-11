/**
 * Codebase Search Tool
 * 
 * MCP tool for searching the codebase semantically.
 */

import * as path from 'path';
import { SearchService } from '../services/search-service';

// Define the tool interface for TypeScript
interface MCPTool {
  name: string;
  description: string;
  inputSchema: any;
  execute(params: any): Promise<any>;
}

/**
 * Tool for searching the codebase
 */
export class CodebaseSearchTool implements MCPTool {
  public name: string = 'codebase_search';
  public description: string = 'Search the codebase for relevant files based on a query';
  
  public inputSchema = {
    type: 'object',
    properties: {
      query: {
        type: 'string',
        description: 'The search query to find relevant code'
      },
      path: {
        type: 'string',
        description: 'Optional path to limit the search to a specific directory'
      }
    },
    required: ['query']
  };

  private searchService: SearchService;

  constructor(searchService: SearchService) {
    this.searchService = searchService;
  }

  /**
   * Execute the codebase search
   */
  public async execute(params: { query: string; path?: string }): Promise<any> {
    try {
      // Ensure the search service is initialized
      if (!this.searchService) {
        return {
          error: 'Search service not initialized'
        };
      }

      // Search the codebase
      const results = await this.searchService.search(params.query, params.path);

      // Format the results
      return {
        results: results.map(result => ({
          filePath: result.filePath,
          score: result.score,
          snippet: result.snippet
        }))
      };
    } catch (error: any) {
      console.error('Error executing codebase search:', error);
      return {
        error: error.message || 'Unknown error'
      };
    }
  }
}