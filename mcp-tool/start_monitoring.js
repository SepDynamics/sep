#!/usr/bin/env node

/**
 * SEP Monitoring Dashboard Launcher
 * 
 * This script launches the SEP monitoring dashboard, providing a web-based
 * interface to monitor all SEP components and their performance metrics.
 */

const sepMonitoring = require('./sep_monitoring_dashboard');

// Command-line arguments
const args = process.argv.slice(2);
const options = {
  help: args.includes('--help') || args.includes('-h'),
  httpPort: 8090,
  wsPort: 8091,
  refreshInterval: 5000 // 5 seconds
};

// Parse port arguments
const httpPortArg = args.find(arg => arg.startsWith('--http-port='));
if (httpPortArg) {
  options.httpPort = parseInt(httpPortArg.split('=')[1], 10);
}

const wsPortArg = args.find(arg => arg.startsWith('--ws-port='));
if (wsPortArg) {
  options.wsPort = parseInt(wsPortArg.split('=')[1], 10);
}

// Parse refresh interval argument
const refreshArg = args.find(arg => arg.startsWith('--refresh='));
if (refreshArg) {
  options.refreshInterval = parseInt(refreshArg.split('=')[1], 10);
}

/**
 * Display help information
 */
function showHelp() {
  console.log(`
SEP Monitoring Dashboard Launcher

Usage: node start_monitoring.js [options]

Options:
  --help, -h                 Show this help message
  --http-port=<port>         HTTP port for the dashboard (default: 8090)
  --ws-port=<port>           WebSocket port for real-time updates (default: 8091)
  --refresh=<ms>             Refresh interval in milliseconds (default: 5000)

Examples:
  node start_monitoring.js                     Start with default settings
  node start_monitoring.js --http-port=8080    Start with custom HTTP port
  node start_monitoring.js --refresh=10000     Start with 10-second refresh interval
  `);
}

/**
 * Start the monitoring dashboard
 */
function startMonitoring() {
  console.log('Starting SEP Monitoring Dashboard...');
  
  // Initialize the dashboard
  sepMonitoring.initDashboard({
    httpPort: options.httpPort,
    wsPort: options.wsPort,
    refreshInterval: options.refreshInterval
  });
  
  console.log(`
SEP Monitoring Dashboard is running!

Dashboard URL: http://localhost:${options.httpPort}
WebSocket URL: ws://localhost:${options.wsPort}
Refresh Interval: ${options.refreshInterval}ms

Press Ctrl+C to stop the dashboard.
  `);
}

/**
 * Main function
 */
function main() {
  // Show help if requested
  if (options.help) {
    showHelp();
    return;
  }
  
  // Start the monitoring dashboard
  startMonitoring();
}

// Run the main function
main();