#!/usr/bin/env node

/**
 * SEP Engine Codebase Search MCP Server
 * 
 * This script starts the codebase search MCP server that provides semantic search
 * capabilities for the SEP Engine codebase.
 */

// Check if the dist directory exists, if not, build the project
const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

const distDir = path.join(__dirname, 'dist');
const indexFile = path.join(distDir, 'index.js');

// If the dist directory doesn't exist or the index.js file doesn't exist, build the project
if (!fs.existsSync(distDir) || !fs.existsSync(indexFile)) {
  console.log('Building codebase search server...');
  try {
    execSync('npm run build', { cwd: __dirname, stdio: 'inherit' });
  } catch (error) {
    console.error('Failed to build codebase search server:', error.message);
    process.exit(1);
  }
}

// Start the server
console.log('Starting codebase search server...');
try {
  // Import and run the server
  const { startServer } = require('./dist/index');
  startServer();
} catch (error) {
  console.error('Failed to start codebase search server:', error.message);
  process.exit(1);
}