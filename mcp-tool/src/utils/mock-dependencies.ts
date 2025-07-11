/**
 * Mock implementations for external dependencies
 * 
 * This file provides mock implementations for external dependencies that are not available
 * in the current environment. This allows the code to compile without errors.
 */

// Mock implementation for fs-extra
export const fs = {
  readFile: (path: string, encoding: string): Promise<string> => {
    console.log(`Mock readFile: ${path}`);
    return Promise.resolve(`Mock content for ${path}`);
  },
  readFileSync: (path: string, encoding: string): string => {
    console.log(`Mock readFileSync: ${path}`);
    return `Mock content for ${path}`;
  },
  existsSync: (path: string): boolean => {
    console.log(`Mock existsSync: ${path}`);
    return true;
  }
};

// Mock implementation for fast-glob
export const glob = (patterns: string | string[], options: any): Promise<string[]> => {
  console.log(`Mock glob: ${patterns}`);
  return Promise.resolve(['mock-file-1.ts', 'mock-file-2.cpp', 'mock-file-3.md']);
};

// Mock implementation for minimatch
export const minimatch = (path: string, pattern: string): boolean => {
  console.log(`Mock minimatch: ${path} against ${pattern}`);
  return true;
};