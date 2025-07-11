/**
 * File Content Tool
 * 
 * MCP tool for retrieving file content.
 */

import * as fs from 'fs';
import * as path from 'path';

// Define the tool interface for TypeScript
interface MCPTool {
  name: string;
  description: string;
  inputSchema: any;
  execute(params: any): Promise<any>;
}

/**
 * Tool for retrieving file content
 */
export class FileContentTool implements MCPTool {
  public name: string = 'file_content';
  public description: string = 'Retrieve the content of a file';
  
  public inputSchema = {
    type: 'object',
    properties: {
      filePath: {
        type: 'string',
        description: 'The path to the file to retrieve'
      },
      lineRange: {
        type: 'array',
        items: {
          type: 'object',
          properties: {
            start: { type: 'number' },
            end: { type: 'number' }
          },
          required: ['start', 'end']
        },
        description: 'Optional line ranges to retrieve (1-based, inclusive)'
      }
    },
    required: ['filePath']
  };

  /**
   * Execute the file content retrieval
   */
  public async execute(params: { 
    filePath: string; 
    lineRange?: Array<{ start: number; end: number }>
  }): Promise<any> {
    try {
      const { filePath, lineRange } = params;
      
      // Resolve the file path
      const fullPath = path.resolve(process.cwd(), filePath);
      
      // Check if the file exists
      if (!fs.existsSync(fullPath)) {
        return {
          error: `File not found: ${filePath}`
        };
      }
      
      // Read the file content
      const content = fs.readFileSync(fullPath, 'utf-8');
      const lines = content.split('\n');
      
      // If line range is specified, extract only those lines
      if (lineRange && lineRange.length > 0) {
        const extractedLines: string[] = [];
        
        for (const range of lineRange) {
          const { start, end } = range;
          
          // Validate the range
          if (start < 1 || end > lines.length || start > end) {
            return {
              error: `Invalid line range: ${start}-${end} (file has ${lines.length} lines)`
            };
          }
          
          // Extract the lines (adjusting for 0-based array indexing)
          for (let i = start - 1; i < end; i++) {
            extractedLines.push(`${i + 1} | ${lines[i]}`);
          }
        }
        
        return {
          content: extractedLines.join('\n'),
          lineCount: lines.length
        };
      }
      
      // Return the full content with line numbers
      return {
        content: lines.map((line, index) => `${index + 1} | ${line}`).join('\n'),
        lineCount: lines.length
      };
    } catch (error: any) {
      console.error('Error retrieving file content:', error);
      return {
        error: error.message || 'Unknown error'
      };
    }
  }
}