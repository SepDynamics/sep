/**
 * Search Service
 *
 * Provides semantic search capabilities for the codebase.
 */

import * as path from 'path';
import { fs, glob, minimatch } from '../utils/mock-dependencies';

export interface SearchServiceOptions {
  rootDir: string;
  indexingPatterns: string[];
  excludePatterns: string[];
}

export interface SearchResult {
  filePath: string;
  score: number;
  snippet: string;
}

export interface FileContentResult {
  filePath: string;
  content: string;
  lineCount: number;
}

/**
 * Service for searching the codebase
 */
export class SearchService {
  private options: SearchServiceOptions;
  private fileIndex: Map<string, string> = new Map();
  private initialized: boolean = false;

  constructor(options: SearchServiceOptions) {
    this.options = options;
  }

  /**
   * Initialize the search service by indexing the codebase
   */
  public async initialize(): Promise<void> {
    if (this.initialized) {
      return;
    }

    try {
      // Find all files matching the indexing patterns
      const files = await glob(this.options.indexingPatterns, {
        cwd: this.options.rootDir,
        absolute: false,
        onlyFiles: true,
        ignore: this.options.excludePatterns
      });

      // Index each file
      for (const file of files) {
        const filePath = path.join(this.options.rootDir, file);
        try {
          const content = await fs.readFile(filePath, 'utf-8');
          this.fileIndex.set(file, content);
        } catch (error) {
          console.error(`Error indexing file ${file}:`, error);
        }
      }

      this.initialized = true;
      console.log(`Indexed ${this.fileIndex.size} files`);
    } catch (error) {
      console.error('Error initializing search service:', error);
      throw error;
    }
  }

  /**
   * Search the codebase for files matching the query
   */
  public async search(query: string, searchPath?: string, limit: number = 10): Promise<SearchResult[]> {
    if (!this.initialized) {
      await this.initialize();
    }

    // Simple search implementation - in a real implementation, this would use embeddings
    const results: SearchResult[] = [];
    
    // Convert query to lowercase for case-insensitive search
    const lowerQuery = query.toLowerCase();
    
    // Filter files by path if provided
    const filesToSearch = searchPath 
      ? Array.from(this.fileIndex.entries())
          .filter(([filePath]) => filePath.startsWith(searchPath))
      : Array.from(this.fileIndex.entries());
    
    // Search each file
    for (const [filePath, content] of filesToSearch) {
      const lowerContent = content.toLowerCase();
      
      // Check if the content contains the query
      if (lowerContent.includes(lowerQuery)) {
        // Find the position of the match
        const matchIndex = lowerContent.indexOf(lowerQuery);
        
        // Extract a snippet around the match
        const snippetStart = Math.max(0, matchIndex - 50);
        const snippetEnd = Math.min(content.length, matchIndex + query.length + 50);
        const snippet = content.substring(snippetStart, snippetEnd);
        
        // Calculate a simple score based on the number of matches
        const matchCount = (lowerContent.match(new RegExp(lowerQuery, 'g')) || []).length;
        const score = matchCount / content.length;
        
        results.push({
          filePath,
          score,
          snippet: `...${snippet}...`
        });
      }
    }
    
    // Sort results by score (descending)
    results.sort((a, b) => b.score - a.score);
    
    // Return the top results
    return results.slice(0, limit);
  }

  /**
   * Get the content of a file
   */
  public async getFileContent(filePath: string): Promise<FileContentResult | null> {
    try {
      const fullPath = path.join(this.options.rootDir, filePath);
      const content = await fs.readFile(fullPath, 'utf-8');
      const lineCount = content.split('\n').length;
      
      return {
        filePath,
        content,
        lineCount
      };
    } catch (error) {
      console.error(`Error reading file ${filePath}:`, error);
      return null;
    }
  }
}