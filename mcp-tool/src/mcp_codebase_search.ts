import { McpServer } from '@modelcontextprotocol/sdk/server/mcp.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import { z } from 'zod';
import { searchCodebase } from './codebase_search.js';

// Define the return type of searchCodebase for TypeScript
type SearchResult = {
  file: string;
  matches: { lineNumber: number; line: string }[];
  relevance: number;
};

type SearchResults = {
  results: SearchResult[];
  error?: string;
};

/**
 * MCP server that provides codebase search functionality
 */
export class CodebaseSearchMcp {
  private mcp: McpServer;

  constructor() {
    this.mcp = new McpServer({
      name: 'codebase-search',
      version: '1.0.0',
      description: 'Provides semantic search capabilities for the SEP Engine codebase'
    });
    this.registerTools();
  }

  private registerTools() {
    // Register the search tool
    this.mcp.tool(
      'search',
      {
        query: z.string().describe('The search query to find relevant code'),
        path: z.string().optional().describe('Optional path to limit search to a specific directory')
      },
      async (args) => {
        const directory = args.path || '/sep';
        const results = await searchCodebase(args.query, directory) as SearchResults;
        
        // Format results for display
        let content = `# Search Results for "${args.query}"\n\n`;
        
        if (results.error) {
          content += `Error: ${results.error}\n`;
        } else if (!results.results || results.results.length === 0) {
          content += 'No results found.\n';
        } else {
          content += `Found ${results.results.length} matching files:\n\n`;
          
          results.results.forEach((result, index) => {
            if (result) {
              content += `## ${index + 1}. ${result.file}\n\n`;
              content += `Relevance score: ${result.relevance}\n\n`;
              
              if (result.matches && result.matches.length > 0) {
                content += '```\n';
                result.matches.slice(0, 10).forEach(match => {
                  content += `${match.lineNumber}: ${match.line}\n`;
                });
                
                if (result.matches.length > 10) {
                  content += `... and ${result.matches.length - 10} more matches\n`;
                }
                
                content += '```\n\n';
              }
            }
          });
        }
        
        return { content: [{ type: 'text', text: content }] };
      }
    );

    // Register a tool to get file content
    this.mcp.tool(
      'get_file',
      {
        path: z.string().describe('The path to the file to retrieve'),
        lineRange: z.string().optional().describe('Optional line range in format "start-end"')
      },
      async (args) => {
        try {
          const fs = await import('fs');
          const path = await import('path');
          
          const filePath = path.resolve('/sep', args.path);
          const content = fs.readFileSync(filePath, 'utf-8');
          const lines = content.split('\n');
          
          let result = '';
          let lineStart = 0;
          let lineEnd = lines.length;
          
          // Parse line range if provided
          if (args.lineRange) {
            const [start, end] = args.lineRange.split('-').map(n => parseInt(n, 10));
            if (!isNaN(start)) lineStart = Math.max(0, start - 1);
            if (!isNaN(end)) lineEnd = Math.min(lines.length, end);
          }
          
          // Extract the requested lines
          const selectedLines = lines.slice(lineStart, lineEnd);
          
          // Format the output with line numbers
          result = selectedLines.map((line, i) =>
            `${lineStart + i + 1}: ${line}`
          ).join('\n');
          
          return {
            content: [
              { type: 'text', text: `File: ${args.path}\n\n\`\`\`\n${result}\n\`\`\`` }
            ]
          };
        } catch (error: any) {
          return {
            content: [
              { type: 'text', text: `Error retrieving file: ${error.message || String(error)}` }
            ]
          };
        }
      }
    );
  }

  async start() {
    const transport = new StdioServerTransport();
    await this.mcp.connect(transport);
    console.error('Codebase search MCP server running on stdio');
  }
}

// Start the server if this file is run directly
if (process.argv[1] === new URL(import.meta.url).pathname) {
  const server = new CodebaseSearchMcp();
  server.start().catch((err) => {
    console.error('Failed to start codebase search MCP server:', err);
    process.exit(1);
  });
}