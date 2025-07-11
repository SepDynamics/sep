import fs from 'fs';
import path from 'path';
import { execSync } from 'child_process';

// Simple codebase search utility
async function searchCodebase(query: string, directory: string = '/sep') {
  try {
    // Use ripgrep if available for faster searching
    try {
      const result = execSync(`rg -i -l "${query}" ${directory}`, { encoding: 'utf-8' });
      return processResults(result, query, directory);
    } catch (e) {
      // Fall back to grep if ripgrep is not available
      const result = execSync(`grep -r -i -l "${query}" ${directory}`, { encoding: 'utf-8' });
      return processResults(result, query, directory);
    }
  } catch (error) {
    console.error('Error searching codebase:', error);
    return { results: [], error: String(error) };
  }
}

function processResults(output: string, query: string, directory: string) {
  const files = output.split('\n').filter(Boolean);
  const results = files.map(file => {
    try {
      const content = fs.readFileSync(file, 'utf-8');
      const lines = content.split('\n');
      
      // Find lines that match the query
      const matchingLines: { lineNumber: number; line: string }[] = [];
      lines.forEach((line, index) => {
        if (line.toLowerCase().includes(query.toLowerCase())) {
          matchingLines.push({ lineNumber: index + 1, line });
        }
      });
      
      return {
        file: path.relative(directory, file),
        matches: matchingLines,
        relevance: matchingLines.length
      };
    } catch (e) {
      return null;
    }
  }).filter(Boolean);
  
  // Sort by relevance
  results.sort((a, b) => b!.relevance - a!.relevance);
  
  return { results };
}

// If run directly
if (process.argv[1] === new URL(import.meta.url).pathname) {
  const query = process.argv[2];
  const directory = process.argv[3] || '/sep';
  
  if (!query) {
    console.error('Usage: node codebase_search.js <query> [directory]');
    process.exit(1);
  }
  
  searchCodebase(query, directory)
    .then(results => console.log(JSON.stringify(results, null, 2)))
    .catch(err => {
      console.error('Error:', err);
      process.exit(1);
    });
}

export { searchCodebase };