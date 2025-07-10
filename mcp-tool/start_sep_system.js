#!/usr/bin/env node

/**
 * SEP System Startup Script
 * 
 * This script provides a command-line interface for initializing the SEP system
 * using the bring-up controller.
 */

const sepController = require('./sep_bring_up_controller');
const sepBringUp = require('./sep_bring_up_sequence');
const sepVisualizer = require('./sep_visualization_updater');

// Command-line arguments
const args = process.argv.slice(2);
const options = {
  help: args.includes('--help') || args.includes('-h'),
  verbose: args.includes('--verbose') || args.includes('-v'),
  noRetry: args.includes('--no-retry'),
  noVis: args.includes('--no-visualization'),
  component: null,
  maxRetries: 3,
  timeout: 30000 // 30 seconds
};

// Parse component-specific argument
const componentArg = args.find(arg => arg.startsWith('--component='));
if (componentArg) {
  options.component = componentArg.split('=')[1];
}

// Parse max retries argument
const maxRetriesArg = args.find(arg => arg.startsWith('--max-retries='));
if (maxRetriesArg) {
  options.maxRetries = parseInt(maxRetriesArg.split('=')[1], 10);
}

// Parse timeout argument
const timeoutArg = args.find(arg => arg.startsWith('--timeout='));
if (timeoutArg) {
  options.timeout = parseInt(timeoutArg.split('=')[1], 10);
}

/**
 * Display help information
 */
function showHelp() {
  console.log(`
SEP System Startup Script

Usage: node start_sep_system.js [options]

Options:
  --help, -h                 Show this help message
  --verbose, -v              Enable verbose logging
  --no-retry                 Disable automatic retry of failed components
  --no-visualization         Disable visualization updates
  --component=<id>           Initialize only the specified component
  --max-retries=<number>     Maximum retry attempts (default: 3)
  --timeout=<ms>             Timeout in milliseconds (default: 30000)

Examples:
  node start_sep_system.js                     Initialize the entire SEP system
  node start_sep_system.js --verbose           Initialize with verbose logging
  node start_sep_system.js --component=cuda_integration  Initialize only CUDA integration
  `);
}

/**
 * Format duration in milliseconds to a human-readable string
 * @param {number} ms - Duration in milliseconds
 * @returns {string} Formatted duration
 */
function formatDuration(ms) {
  const seconds = Math.floor(ms / 1000);
  const minutes = Math.floor(seconds / 60);
  const remainingSeconds = seconds % 60;
  return `${minutes}m ${remainingSeconds}s`;
}

/**
 * Log a message with timestamp
 * @param {string} message - Message to log
 * @param {string} level - Log level
 */
function log(message, level = 'info') {
  const timestamp = new Date().toISOString();
  const prefix = level === 'error' ? '❌ ERROR' : level === 'warn' ? '⚠️ WARN' : '📋 INFO';
  console.log(`[${timestamp}] ${prefix}: ${message}`);
}

/**
 * Log a component status change
 * @param {string} componentId - Component ID
 * @param {string} status - New status
 * @param {string} details - Status details
 */
function logComponentStatus(componentId, status, details = '') {
  const component = sepBringUp.components.find(c => c.id === componentId);
  const name = component ? component.name : componentId;
  const statusIcon = sepVisualizer.STATUS_ICONS[status.toLowerCase()];
  const statusColor = sepVisualizer.STATUS_COLORS[status.toLowerCase()];
  
  let message = `Component '${name}' (${componentId}) status: ${statusIcon} ${status}`;
  if (details) {
    message += ` - ${details}`;
  }
  
  log(message);
}

/**
 * Initialize a single component
 * @param {string} componentId - Component ID
 */
async function initializeSingleComponent(componentId) {
  log(`Initializing component: ${componentId}`);
  
  try {
    const result = await sepController.initializeComponent(componentId, {
      timeoutMs: options.timeout,
      updateVisualization: !options.noVis,
      throwOnError: true
    });
    
    log(`Component '${componentId}' initialized successfully`);
    return true;
  } catch (error) {
    log(`Failed to initialize component '${componentId}': ${error.message}`, 'error');
    return false;
  }
}

/**
 * Start the bring-up process for the entire SEP system
 */
async function startFullBringUp() {
  log('Starting SEP system bring-up process');
  
  // Register event listeners for verbose logging
  if (options.verbose) {
    sepController.events.on('component:start', ({ componentId }) => {
      log(`Starting initialization of component '${componentId}'`);
    });
    
    sepController.events.on('component:initializing', ({ componentId }) => {
      logComponentStatus(componentId, 'INITIALIZING');
    });
    
    sepController.events.on('component:ready', ({ componentId }) => {
      logComponentStatus(componentId, 'READY');
    });
    
    sepController.events.on('component:error', ({ componentId, error }) => {
      logComponentStatus(componentId, 'ERROR', error.message);
    });
    
    sepController.events.on('retry:attempt', ({ componentId, retryCount }) => {
      log(`Retrying component '${componentId}' (attempt ${retryCount}/${options.maxRetries})`, 'warn');
    });
    
    sepController.events.on('retry:max-exceeded', ({ componentId }) => {
      log(`Maximum retry attempts exceeded for component '${componentId}'`, 'error');
    });
    
    sepController.events.on('bring-up:complete', ({ progress }) => {
      log(`Bring-up process complete: ${progress.ready}/${progress.total} components ready (${progress.percentComplete}%)`);
    });
    
    sepController.events.on('bring-up:error', ({ error, progress }) => {
      log(`Bring-up process failed: ${error.message}`, 'error');
      log(`Progress at failure: ${progress.ready}/${progress.total} components ready (${progress.percentComplete}%)`);
    });
  }
  
  try {
    // Start bring-up process
    const result = await sepController.startBringUp({
      maxRetries: options.maxRetries,
      autoRetry: !options.noRetry,
      timeoutMs: options.timeout
    });
    
    if (result.success) {
      log(`SEP system bring-up completed successfully in ${formatDuration(result.duration)}`);
      log(`${result.progress.ready}/${result.progress.total} components ready (${result.progress.percentComplete}%)`);
      
      if (result.progress.error > 0 || result.progress.degraded > 0) {
        log(`Warning: ${result.progress.error} components have errors, ${result.progress.degraded} are degraded`, 'warn');
      }
      
      return true;
    } else {
      log(`SEP system bring-up failed: ${result.error.message}`, 'error');
      log(`Progress at failure: ${result.progress.ready}/${result.progress.total} components ready (${result.progress.percentComplete}%)`);
      return false;
    }
  } catch (error) {
    log(`Unexpected error during bring-up process: ${error.message}`, 'error');
    return false;
  }
}

/**
 * Main function
 */
async function main() {
  // Show help if requested
  if (options.help) {
    showHelp();
    return;
  }
  
  // Initialize a single component if specified
  if (options.component) {
    const success = await initializeSingleComponent(options.component);
    process.exit(success ? 0 : 1);
    return;
  }
  
  // Start full bring-up process
  const success = await startFullBringUp();
  process.exit(success ? 0 : 1);
}

// Run the main function
main().catch(error => {
  log(`Unhandled error: ${error.message}`, 'error');
  process.exit(1);
});