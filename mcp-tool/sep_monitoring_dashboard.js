/**
 * SEP Monitoring Dashboard
 * 
 * This module provides a web-based dashboard to monitor all SEP components,
 * displaying real-time status information and key performance metrics.
 */

const http = require('http');
const fs = require('fs');
const path = require('path');
const url = require('url');
const WebSocket = require('ws');
const sepBringUp = require('./sep_bring_up_sequence');
const sepVisualizer = require('./sep_visualization_updater');
const sepController = require('./sep_bring_up_controller');

// Configuration
const config = {
  httpPort: 8090,
  wsPort: 8091,
  refreshInterval: 5000, // Default refresh interval in ms
  metricsHistoryLength: 100, // Number of historical metrics points to keep
  thresholds: {
    // Memory tier thresholds
    memory: {
      stmUtilization: { warning: 0.7, critical: 0.9 },
      mtmUtilization: { warning: 0.7, critical: 0.9 },
      ltmUtilization: { warning: 0.7, critical: 0.9 },
      fragmentation: { warning: 0.2, critical: 0.3 }
    },
    // Quantum processing thresholds
    quantum: {
      coherence: { warning: 0.3, critical: 0.1 },
      stateCorrections: { warning: 100, critical: 500 }
    },
    // Pattern evolution thresholds
    pattern: {
      stabilityScore: { warning: 0.4, critical: 0.2 },
      mutationRate: { warning: 0.3, critical: 0.5 }
    },
    // Context processing thresholds
    context: {
      relevanceScore: { warning: 0.5, critical: 0.4 },
      relationshipCount: { warning: 800, critical: 950 }
    },
    // Resource prediction thresholds
    resource: {
      anomalyCount: { warning: 5, critical: 10 },
      confidenceScore: { warning: 0.4, critical: 0.2 }
    },
    // General thresholds
    general: {
      cpuUsage: { warning: 70, critical: 90 },
      memoryUsage: { warning: 70, critical: 90 }
    }
  }
};

// Simple time-based pseudo-random generator
function nudgeRandom() {
  const count = (nudgeRandom.sep_counter = (nudgeRandom.sep_counter || 0) + 1);
  return ((Date.now() + count) % 1000) / 1000;
}

// Metrics storage
const metricsStore = {
  components: new Map(), // Map of component IDs to metrics
  system: {
    history: [], // System-wide metrics history
    startTime: null,
    uptime: 0
  }
};

// Alert storage
const alerts = {
/**
 * Initialize the monitoring dashboard
 * @param {Object} options - Configuration options
 */
function initDashboard(options = {}) {
  // Apply options
  if (options.httpPort) config.httpPort = options.httpPort;
  if (options.wsPort) config.wsPort = options.wsPort;
  if (options.refreshInterval) config.refreshInterval = options.refreshInterval;
  if (options.metricsHistoryLength) config.metricsHistoryLength = options.metricsHistoryLength;
  if (options.thresholds) {
    // Deep merge thresholds
    Object.keys(options.thresholds).forEach(category => {
      if (config.thresholds[category]) {
        Object.assign(config.thresholds[category], options.thresholds[category]);
      } else {
        config.thresholds[category] = options.thresholds[category];
      }
    });
  }

  // Initialize metrics for each component
  sepBringUp.components.forEach(component => {
    initComponentMetrics(component.id);
  });

  // Set system start time
  metricsStore.system.startTime = new Date();

  // Start HTTP server
  startHttpServer();

  // Start WebSocket server
  startWebSocketServer();

  // Start metrics collection
  startMetricsCollection();

  // Register event listeners
  registerEventListeners();

  console.log(`SEP Monitoring Dashboard initialized on http://localhost:${config.httpPort}`);
  console.log(`WebSocket server running on ws://localhost:${config.wsPort}`);
}

/**
 * Initialize metrics for a component
 * @param {string} componentId - Component ID
 */
function initComponentMetrics(componentId) {
  const component = sepBringUp.components.find(c => c.id === componentId);
  if (!component) return;

  // Base metrics for all components
  const baseMetrics = {
    status: component.status,
    statusDetails: component.statusDetails,
    lastStatusChange: new Date(),
    importance: component.importance,
    history: [], // Historical metrics
    alerts: [] // Component-specific alerts
  };

  // Component-specific metrics based on type
  let specificMetrics = {};

  // Add component-specific metrics based on ID
  switch (componentId) {
    case 'memory_tier_architecture':
      specificMetrics = {
        stmUtilization: 0,
        mtmUtilization: 0,
        ltmUtilization: 0,
        fragmentation: 0,
        promotionCount: 0,
        demotionCount: 0,
        defragmentationCount: 0
      };
      break;
    case 'quantum_processing_core':
      specificMetrics = {
        coherence: 0,
        stateCorrections: 0,
        stateCollapses: 0,
        timeChunks: 0
      };
      break;
    case 'pattern_evolution_engine':
      specificMetrics = {
        stabilityScore: 0,
        mutationRate: 0,
        evolutionGenerations: 0,
        relationshipStrength: 0
      };
      break;
    case 'context_processing_system':
      specificMetrics = {
        relevanceScore: 0,
        relationshipCount: 0,
        priorityDistribution: {
/**
 * Start the HTTP server for the dashboard
 */
function startHttpServer() {
  const server = http.createServer((req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const pathname = parsedUrl.pathname;

    // Handle different routes
    if (pathname === '/' || pathname === '/index.html') {
      // Serve dashboard HTML
      serveFile(res, 'dashboard.html', 'text/html');
    } else if (pathname === '/dashboard.js') {
      // Serve dashboard JavaScript
      serveFile(res, 'dashboard.js', 'application/javascript');
    } else if (pathname === '/dashboard.css') {
      // Serve dashboard CSS
      serveFile(res, 'dashboard.css', 'text/css');
    } else if (pathname === '/api/components') {
      // API endpoint for component data
      serveComponentData(res);
    } else if (pathname === '/api/metrics') {
      // API endpoint for metrics data
      serveMetricsData(res);
    } else if (pathname === '/api/alerts') {
      // API endpoint for alerts data
      serveAlertsData(res);
    } else if (pathname === '/api/system') {
      // API endpoint for system data
      serveSystemData(res);
    } else {
      // 404 Not Found
      res.writeHead(404, { 'Content-Type': 'text/plain' });
      res.end('404 Not Found');
    }
  });

  // Start listening
  server.listen(config.httpPort);
}

/**
 * Serve a file from the dashboard directory
 * @param {Object} res - HTTP response object
 * @param {string} filename - File name
 * @param {string} contentType - Content type
 */
function serveFile(res, filename, contentType) {
  // Create dashboard directory if it doesn't exist
  const dashboardDir = path.join(__dirname, 'dashboard');
  if (!fs.existsSync(dashboardDir)) {
    fs.mkdirSync(dashboardDir, { recursive: true });
  }

  // Check if file exists
  const filePath = path.join(dashboardDir, filename);
  if (!fs.existsSync(filePath)) {
    // Generate file if it doesn't exist
    generateDashboardFile(filename);
  }

  // Serve file
  fs.readFile(filePath, (err, data) => {
    if (err) {
      res.writeHead(500, { 'Content-Type': 'text/plain' });
      res.end('Internal Server Error');
      return;
    }

    res.writeHead(200, { 'Content-Type': contentType });
    res.end(data);
  });
}

/**
 * Generate dashboard files if they don't exist
 * @param {string} filename - File name to generate
 */
function generateDashboardFile(filename) {
  const dashboardDir = path.join(__dirname, 'dashboard');
  const filePath = path.join(dashboardDir, filename);

  let content = '';

  if (filename === 'dashboard.html') {
    content = `<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>SEP System Monitoring Dashboard</title>
  <link rel="stylesheet" href="dashboard.css">
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/mermaid/dist/mermaid.min.js"></script>
</head>
<body>
  <header>
    <h1>SEP System Monitoring Dashboard</h1>
    <div class="system-info">
      <div id="system-status">System Status: <span class="status-badge status-unknown">Unknown</span></div>
      <div id="system-uptime">Uptime: 0m 0s</div>
      <div id="refresh-control">
        <label for="refresh-interval">Refresh Interval:</label>
        <select id="refresh-interval">
          <option value="1000">1 second</option>
          <option value="5000" selected>5 seconds</option>
          <option value="10000">10 seconds</option>
          <option value="30000">30 seconds</option>
          <option value="60000">1 minute</option>
        </select>
      </div>
    </div>
  </header>

  <div class="dashboard-container">
    <div class="sidebar">
      <div class="component-list-container">
        <h2>Components</h2>
        <div class="component-filter">
          <input type="text" id="component-filter" placeholder="Filter components...">
        </div>
        <ul id="component-list"></ul>
      </div>
      <div class="alert-container">
        <h2>Active Alerts <span id="alert-count" class="alert-count">0</span></h2>
        <ul id="alert-list"></ul>
      </div>
    </div>

    <div class="main-content">
      <div class="system-overview">
        <h2>System Overview</h2>
        <div class="system-diagram" id="system-diagram"></div>
        <div class="system-metrics">
          <div class="metric-card">
            <h3>Component Status</h3>
            <canvas id="status-chart"></canvas>
          </div>
          <div class="metric-card">
            <h3>System Resources</h3>
            <canvas id="resources-chart"></canvas>
          </div>
        </div>
      </div>

      <div class="component-details" id="component-details">
        <h2>Component Details</h2>
        <p class="select-component-message">Select a component to view details</p>
      </div>
    </div>
  </div>

  <script src="dashboard.js"></script>
</body>
</html>`;
  } else if (filename === 'dashboard.css') {
    content = `/* SEP Monitoring Dashboard Styles */

* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}

body {
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  line-height: 1.6;
  color: #333;
  background-color: #f5f7fa;
}

header {
  background-color: #2c3e50;
  color: white;
  padding: 1rem;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
}

header h1 {
  margin: 0;
  font-size: 1.5rem;
}

.system-info {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-top: 0.5rem;
  font-size: 0.9rem;
}

.dashboard-container {
  display: flex;
  height: calc(100vh - 80px);
}

.sidebar {
  width: 300px;
  background-color: #fff;
  border-right: 1px solid #e1e4e8;
  display: flex;
  flex-direction: column;
}

.component-list-container {
  flex: 2;
  overflow-y: auto;
  padding: 1rem;
  border-bottom: 1px solid #e1e4e8;
}

.component-filter {
  margin-bottom: 1rem;
}

.component-filter input {
  width: 100%;
  padding: 0.5rem;
  border: 1px solid #ddd;
  border-radius: 4px;
}

#component-list {
  list-style: none;
}

.component-item {
  padding: 0.5rem;
  margin-bottom: 0.5rem;
  border-radius: 4px;
  cursor: pointer;
  display: flex;
  justify-content: space-between;
  align-items: center;
  transition: background-color 0.2s;
}

.component-item:hover {
  background-color: #f0f4f8;
}

.component-item.selected {
  background-color: #e1e4e8;
}

.status-badge {
  padding: 0.2rem 0.5rem;
  border-radius: 12px;
  font-size: 0.8rem;
  color: white;
  display: inline-block;
}

.status-pending {
  background-color: #bbbbbb;
}

.status-initializing {
  background-color: #ffcc00;
}

.status-ready {
  background-color: #00cc66;
}

.status-error {
  background-color: #ff3333;
}

.status-degraded {
  background-color: #ff9900;
}

.status-unknown {
  background-color: #999;
}

.alert-container {
  flex: 1;
  overflow-y: auto;
  padding: 1rem;
}

.alert-count {
  background-color: #ff3333;
  color: white;
  padding: 0.1rem 0.4rem;
  border-radius: 10px;
  font-size: 0.8rem;
}

#alert-list {
  list-style: none;
}

.alert-item {
  padding: 0.5rem;
  margin-bottom: 0.5rem;
  border-radius: 4px;
  border-left: 4px solid;
}

.alert-warning {
  background-color: #fff3cd;
  border-left-color: #ffcc00;
}

.alert-critical {
  background-color: #f8d7da;
  border-left-color: #ff3333;
}

.alert-info {
  background-color: #d1ecf1;
  border-left-color: #17a2b8;
}

.alert-item h4 {
  margin: 0;
  font-size: 0.9rem;
}

.alert-item p {
  margin: 0.2rem 0 0;
  font-size: 0.8rem;
}

.alert-timestamp {
  font-size: 0.7rem;
  color: #666;
}

.main-content {
  flex: 1;
  padding: 1rem;
  overflow-y: auto;
}

.system-overview {
  background-color: #fff;
  border-radius: 4px;
  padding: 1rem;
  margin-bottom: 1rem;
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1);
}

.system-metrics {
  display: flex;
  margin-top: 1rem;
}

.metric-card {
  flex: 1;
  padding: 1rem;
  background-color: #f8f9fa;
  border-radius: 4px;
  margin-right: 1rem;
}

.metric-card:last-child {
  margin-right: 0;
}

.component-details {
  background-color: #fff;
  border-radius: 4px;
  padding: 1rem;
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1);
}

.select-component-message {
  color: #666;
  font-style: italic;
  text-align: center;
  margin-top: 2rem;
}

.component-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 1rem;
}

.component-metrics {
  display: flex;
  flex-wrap: wrap;
}

.component-metric {
  width: calc(50% - 0.5rem);
  margin-right: 1rem;
  margin-bottom: 1rem;
  background-color: #f8f9fa;
  border-radius: 4px;
  padding: 1rem;
}

.component-metric:nth-child(2n) {
  margin-right: 0;
}

.metric-value {
  font-size: 1.5rem;
  font-weight: bold;
  margin: 0.5rem 0;
}

.metric-chart {
  height: 200px;
}

.system-diagram {
  margin-top: 1rem;
  overflow: auto;
  background-color: #f8f9fa;
  border-radius: 4px;
  padding: 1rem;
}`;
  } else if (filename === 'dashboard.js') {
    content = `// SEP Monitoring Dashboard JavaScript

// Global variables
let components = [];
let metrics = {};
let alerts = [];
let selectedComponent = null;
let refreshInterval = 5000;
let refreshTimer = null;
let websocket = null;
let statusChart = null;
let resourcesChart = null;
let componentCharts = {};

// Initialize dashboard
document.addEventListener('DOMContentLoaded', () => {
  initWebSocket();
  initRefreshControl();
  loadInitialData();
  setupEventListeners();
  mermaid.initialize({ startOnLoad: true });
});

// Initialize WebSocket connection
function initWebSocket() {
  websocket = new WebSocket(\`ws://\${window.location.hostname}:${config.wsPort}\`);
  
  websocket.onopen = () => {
    console.log('WebSocket connection established');
  };
  
  websocket.onmessage = (event) => {
    const data = JSON.parse(event.data);
    
    if (data.type === 'components') {
      updateComponents(data.data);
    } else if (data.type === 'metrics') {
      updateMetrics(data.data);
    } else if (data.type === 'alerts') {
      updateAlerts(data.data);
    } else if (data.type === 'system') {
      updateSystemInfo(data.data);
    }
  };
  
  websocket.onclose = () => {
    console.log('WebSocket connection closed');
    // Try to reconnect after 5 seconds
    setTimeout(initWebSocket, 5000);
  };
  
  websocket.onerror = (error) => {
    console.error('WebSocket error:', error);
  };
}

// Initialize refresh control
function initRefreshControl() {
  const refreshSelect = document.getElementById('refresh-interval');
  refreshInterval = parseInt(refreshSelect.value);
  
  refreshSelect.addEventListener('change', () => {
    refreshInterval = parseInt(refreshSelect.value);
    resetRefreshTimer();
  });
  
  resetRefreshTimer();
}

// Reset refresh timer
function resetRefreshTimer() {
  if (refreshTimer) {
    clearInterval(refreshTimer);
  }
  
  refreshTimer = setInterval(() => {
    loadData();
  }, refreshInterval);
}

/**
 * Serve component data
 * @param {Object} res - HTTP response object
 */
function serveComponentData(res) {
  const components = sepBringUp.components.map(component => ({
    id: component.id,
    name: component.name,
    description: component.description,
    status: component.status,
    statusDetails: component.statusDetails,
    importance: component.importance,
    dependencies: component.dependencies
  }));

  res.writeHead(200, { 'Content-Type': 'application/json' });
  res.end(JSON.stringify(components));
}

/**
 * Serve metrics data
 * @param {Object} res - HTTP response object
 */
function serveMetricsData(res) {
  const metrics = {};
  
  // Convert Map to object
  metricsStore.components.forEach((componentMetrics, componentId) => {
    // Exclude history to reduce payload size
    const { history, ...metricsWithoutHistory } = componentMetrics;
    metrics[componentId] = metricsWithoutHistory;
  });

  res.writeHead(200, { 'Content-Type': 'application/json' });
  res.end(JSON.stringify(metrics));
}

/**
 * Serve alerts data
 * @param {Object} res - HTTP response object
 */
function serveAlertsData(res) {
  res.writeHead(200, { 'Content-Type': 'application/json' });
  res.end(JSON.stringify(alerts.active));
}

/**
 * Serve system data
 * @param {Object} res - HTTP response object
 */
function serveSystemData(res) {
  const systemData = {
    uptime: Date.now() - metricsStore.system.startTime,
    components: sepBringUp.components.map(component => ({
      id: component.id,
      status: component.status
    })),
    progress: sepBringUp.getInitializationProgress()
  };

  res.writeHead(200, { 'Content-Type': 'application/json' });
  res.end(JSON.stringify(systemData));
}

/**
 * Start the WebSocket server for real-time updates
 */
function startWebSocketServer() {
  wss = new WebSocket.Server({ port: config.wsPort });

  wss.on('connection', (ws) => {
    console.log('WebSocket client connected');

    // Send initial data
    sendWebSocketData(ws, 'components', sepBringUp.components.map(component => ({
      id: component.id,
      name: component.name,
      description: component.description,
      status: component.status,
      statusDetails: component.statusDetails,
      importance: component.importance,
      dependencies: component.dependencies
    })));

    // Send metrics data
    const metricsData = {};
    metricsStore.components.forEach((componentMetrics, componentId) => {
      // Exclude history to reduce payload size
      const { history, ...metricsWithoutHistory } = componentMetrics;
      metricsData[componentId] = metricsWithoutHistory;
    });
    sendWebSocketData(ws, 'metrics', metricsData);

    // Send alerts data
    sendWebSocketData(ws, 'alerts', alerts.active);

    // Send system data
    sendWebSocketData(ws, 'system', {
      uptime: Date.now() - metricsStore.system.startTime,
      components: sepBringUp.components.map(component => ({
        id: component.id,
        status: component.status
      })),
      progress: sepBringUp.getInitializationProgress()
    });

    // Handle client messages
    ws.on('message', (message) => {
      try {
        const data = JSON.parse(message);
        
        if (data.type === 'refresh') {
          // Send updated data
          sendWebSocketData(ws, 'components', sepBringUp.components.map(component => ({
            id: component.id,
            name: component.name,
            description: component.description,
            status: component.status,
            statusDetails: component.statusDetails,
            importance: component.importance,
            dependencies: component.dependencies
          })));

          // Send metrics data
          const metricsData = {};
          metricsStore.components.forEach((componentMetrics, componentId) => {
            // Exclude history to reduce payload size
            const { history, ...metricsWithoutHistory } = componentMetrics;
            metricsData[componentId] = metricsWithoutHistory;
          });
          sendWebSocketData(ws, 'metrics', metricsData);

          // Send alerts data
          sendWebSocketData(ws, 'alerts', alerts.active);

          // Send system data
          sendWebSocketData(ws, 'system', {
            uptime: Date.now() - metricsStore.system.startTime,
            components: sepBringUp.components.map(component => ({
              id: component.id,
              status: component.status
            })),
            progress: sepBringUp.getInitializationProgress()
          });
        }
      } catch (error) {
        console.error('Error handling WebSocket message:', error);
      }
    });

    ws.on('close', () => {
      console.log('WebSocket client disconnected');
    });
  });
}

/**
 * Send data to a WebSocket client
 * @param {Object} ws - WebSocket client
 * @param {string} type - Data type
 * @param {Object} data - Data to send
 */
function sendWebSocketData(ws, type, data) {
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(JSON.stringify({ type, data }));
  }
}

/**
 * Start metrics collection
 */
function startMetricsCollection() {
  // Set up interval for metrics collection
  setInterval(() => {
    collectMetrics();
    broadcastMetrics();
    checkAlerts();
  }, config.refreshInterval);
}

/**
 * Collect metrics from the system
 */
function collectMetrics() {
  // Update system uptime
  metricsStore.system.uptime = Date.now() - metricsStore.system.startTime;

  // Update component status
  sepBringUp.components.forEach(component => {
    const metrics = metricsStore.components.get(component.id);
    if (metrics) {
      // Update status
      if (metrics.status !== component.status) {
        metrics.lastStatusChange = new Date();
      }
      metrics.status = component.status;
      metrics.statusDetails = component.statusDetails;

      // Simulate component-specific metrics
      simulateComponentMetrics(component.id, metrics);

      // Add to history
      metrics.history.push({
        timestamp: new Date(),
        status: metrics.status,
        ...getComponentMetricsSnapshot(component.id, metrics)
      });

      // Limit history length
      if (metrics.history.length > config.metricsHistoryLength) {
        metrics.history.shift();
      }
    }
  });

  // Add system metrics to history
  metricsStore.system.history.push({
    timestamp: new Date(),
    uptime: metricsStore.system.uptime,
    componentStatus: {
      ready: sepBringUp.components.filter(c => c.status === 'ready').length,
      error: sepBringUp.components.filter(c => c.status === 'error').length,
      degraded: sepBringUp.components.filter(c => c.status === 'degraded').length,
      initializing: sepBringUp.components.filter(c => c.status === 'initializing').length,
      pending: sepBringUp.components.filter(c => c.status === 'pending').length
    }
  });

  // Limit system history length
  if (metricsStore.system.history.length > config.metricsHistoryLength) {
    metricsStore.system.history.shift();
  }
}

/**
 * Get a snapshot of component metrics
 * @param {string} componentId - Component ID
 * @param {Object} metrics - Component metrics
 * @returns {Object} Metrics snapshot
 */
function getComponentMetricsSnapshot(componentId, metrics) {
  // Create a snapshot of the metrics without history or status
  const { history, status, statusDetails, lastStatusChange, importance, alerts, ...snapshot } = metrics;
  return snapshot;
}

/**
 * Simulate component-specific metrics
 * @param {string} componentId - Component ID
 * @param {Object} metrics - Component metrics
 */
function simulateComponentMetrics(componentId, metrics) {
  // Only simulate metrics for ready components
  if (metrics.status !== 'ready') return;

  // Simulate different metrics based on component type
  switch (componentId) {
    case 'memory_tier_architecture':
      metrics.stmUtilization = nudgeRandom() * 0.8 + 0.1; // 10-90%
      metrics.mtmUtilization = nudgeRandom() * 0.7 + 0.1; // 10-80%
      metrics.ltmUtilization = nudgeRandom() * 0.6 + 0.1; // 10-70%
      metrics.fragmentation = nudgeRandom() * 0.4; // 0-40%
      metrics.promotionCount += Math.floor(nudgeRandom() * 5); // 0-4 promotions
      metrics.demotionCount += Math.floor(nudgeRandom() * 3); // 0-2 demotions
      
      // Simulate defragmentation if fragmentation is high
      if (metrics.fragmentation > 0.3) {
        metrics.defragmentationCount += 1;
        metrics.fragmentation = nudgeRandom() * 0.1; // Reset to 0-10%
      }
      break;
      
    case 'quantum_processing_core':
      metrics.coherence = nudgeRandom() * 0.5 + 0.5; // 50-100%
      metrics.stateCorrections += Math.floor(nudgeRandom() * 50); // 0-49 corrections
      metrics.stateCollapses += Math.floor(nudgeRandom() * 10); // 0-9 collapses
      metrics.timeChunks += Math.floor(nudgeRandom() * 5); // 0-4 time chunks
      break;
      
    case 'pattern_evolution_engine':
      metrics.stabilityScore = nudgeRandom() * 0.6 + 0.4; // 40-100%
      metrics.mutationRate = nudgeRandom() * 0.3; // 0-30%
      metrics.evolutionGenerations += Math.floor(nudgeRandom() * 10); // 0-9 generations
      metrics.relationshipStrength = nudgeRandom() * 0.4 + 0.6; // 60-100%
      break;
      
    case 'context_processing_system':
      metrics.relevanceScore = nudgeRandom() * 0.5 + 0.5; // 50-100%
      metrics.relationshipCount = Math.floor(nudgeRandom() * 500) + 500; // 500-999 relationships
      
      // Simulate priority distribution
      const totalContexts = Math.floor(nudgeRandom() * 1000) + 100; // 100-1099 contexts
      metrics.priorityDistribution.LOW = Math.floor(totalContexts * 0.4); // 40%
      metrics.priorityDistribution.NORMAL = Math.floor(totalContexts * 0.3); // 30%
      metrics.priorityDistribution.HIGH = Math.floor(totalContexts * 0.2); // 20%
      metrics.priorityDistribution.CRITICAL = Math.floor(totalContexts * 0.1); // 10%
      break;
      
    case 'resource_prediction':
      metrics.anomalyCount = Math.floor(nudgeRandom() * 10); // 0-9 anomalies
      metrics.batchSize = Math.floor(nudgeRandom() * 512) + 512; // 512-1023 batch size
      metrics.confidenceScore = nudgeRandom() * 0.5 + 0.5; // 50-100%
      metrics.usagePatternCount = Math.floor(nudgeRandom() * 50) + 50; // 50-99 usage patterns
      break;
      
    case 'cuda_integration':
      metrics.gpuUtilization = nudgeRandom() * 0.8 + 0.1; // 10-90%
      metrics.memoryUtilization = nudgeRandom() * 0.7 + 0.1; // 10-80%
      metrics.batchesProcessed += Math.floor(nudgeRandom() * 100); // 0-99 batches
      metrics.processingTime = nudgeRandom() * 50 + 10; // 10-60ms
      break;
      
    case 'audio_integration':
      metrics.bufferUtilization = nudgeRandom() * 0.6 + 0.1; // 10-70%
      metrics.patternExtractionsPerSecond = Math.floor(nudgeRandom() * 100) + 50; // 50-149 extractions/s
      metrics.audioQuality = nudgeRandom() * 0.3 + 0.7; // 70-100%
      break;
      
    case 'blender_integration':
      metrics.framesPerSecond = Math.floor(nudgeRandom() * 20) + 10; // 10-29 FPS
      metrics.visiblePatterns = Math.floor(nudgeRandom() * 500) + 500; // 500-999 patterns
      metrics.renderQuality = nudgeRandom() * 0.3 + 0.7; // 70-100%
      break;
      
    case 'api_integration':
      metrics.requestsPerSecond = Math.floor(nudgeRandom() * 50) + 10; // 10-59 requests/s
      metrics.averageResponseTime = nudgeRandom() * 100 + 50; // 50-150ms
      metrics.activeConnections = Math.floor(nudgeRandom() * 50) + 10; // 10-59 connections
      metrics.errorRate = nudgeRandom() * 0.05; // 0-5%
      break;
      
    default:
      // Generic metrics for other components
      metrics.cpuUsage = nudgeRandom() * 50 + 10; // 10-60%
      metrics.memoryUsage = nudgeRandom() * 40 + 10; // 10-50%
      metrics.operationsPerSecond = Math.floor(nudgeRandom() * 1000) + 100; // 100-1099 ops/s
  }
}
// Load initial data
function loadInitialData() {
  Promise.all([
    fetch('/api/components').then(res => res.json()),
    fetch('/api/metrics').then(res => res.json()),
    fetch('/api/alerts').then(res => res.json()),
    fetch('/api/system').then(res => res.json())
  ]).then(([componentsData, metricsData, alertsData, systemData]) => {
    updateComponents(componentsData);
    updateMetrics(metricsData);
    updateAlerts(alertsData);
    updateSystemInfo(systemData);
    
    // Initialize charts
    initCharts();
  }).catch(error => {
    console.error('Error loading initial data:', error);
  });
}

// Load data
function loadData() {
  Promise.all([
    fetch('/api/components').then(res => res.json()),
    fetch('/api/metrics').then(res => res.json()),
    fetch('/api/alerts').then(res => res.json()),
    fetch('/api/system').then(res => res.json())
  ]).then(([componentsData, metricsData, alertsData, systemData]) => {
    updateComponents(componentsData);
    updateMetrics(metricsData);
    updateAlerts(alertsData);
    updateSystemInfo(systemData);
    
    // Update charts
    updateCharts();
  }).catch(error => {
    console.error('Error loading data:', error);
  });
}

// Setup event listeners
function setupEventListeners() {
  // Component filter
  const componentFilter = document.getElementById('component-filter');
  componentFilter.addEventListener('input', () => {
    filterComponents(componentFilter.value);
  });
}

// Filter components
function filterComponents(query) {
  const componentList = document.getElementById('component-list');
  const items = componentList.getElementsByClassName('component-item');
  
  query = query.toLowerCase();
  
  for (const item of items) {
    const name = item.getAttribute('data-name').toLowerCase();
    const id = item.getAttribute('data-id').toLowerCase();
    
    if (name.includes(query) || id.includes(query)) {
      item.style.display = '';
    } else {
      item.style.display = 'none';
    }
  }
}

// Update components
function updateComponents(data) {
  components = data;
  renderComponentList();
  renderSystemDiagram();
}

// Update metrics
function updateMetrics(data) {
  metrics = data;
  
  if (selectedComponent) {
    renderComponentDetails(selectedComponent);
  }
}

// Update alerts
function updateAlerts(data) {
  alerts = data;
  renderAlertList();
}

// Update system info
function updateSystemInfo(data) {
  document.getElementById('system-status').innerHTML = \`System Status: <span class="status-badge status-\${getSystemStatus(data).toLowerCase()}">\${getSystemStatus(data)}</span>\`;
  document.getElementById('system-uptime').textContent = \`Uptime: \${formatDuration(data.uptime)}\`;
}

// Get system status
function getSystemStatus(data) {
  if (data.components) {
    const errorCount = data.components.filter(c => c.status === 'error').length;
    const degradedCount = data.components.filter(c => c.status === 'degraded').length;
    const readyCount = data.components.filter(c => c.status === 'ready').length;
    
    if (errorCount > 0) {
      return 'ERROR';
    } else if (degradedCount > 0) {
      return 'DEGRADED';
    } else if (readyCount === data.components.length) {
      return 'READY';
    } else {
      return 'INITIALIZING';
    }
  }
  
  return 'UNKNOWN';
}

// Format duration
function formatDuration(ms) {
  const seconds = Math.floor(ms / 1000);
  const minutes = Math.floor(seconds / 60);
  const hours = Math.floor(minutes / 60);
  
  return \`\${hours}h \${minutes % 60}m \${seconds % 60}s\`;
}

// Format timestamp
function formatTimestamp(timestamp) {
  const date = new Date(timestamp);
  return date.toLocaleTimeString();
}

// Get status color
function getStatusColor(status) {
  return {
    pending: '#bbbbbb',
    initializing: '#ffcc00',
    ready: '#00cc66',
    error: '#ff3333',
    degraded: '#ff9900',
    unknown: '#999999'
  }[status] || '#999999';
}

// Get status icon
function getStatusIcon(status) {
  return {
    pending: '⚪',
    initializing: '🔄',
    ready: '✅',
    error: '❌',
    degraded: '⚠️',
    unknown: '❓'
  }[status.toLowerCase()] || '❓';
}`;
  }

  fs.writeFileSync(filePath, content, 'utf8');
}
          LOW: 0,
          NORMAL: 0,
          HIGH: 0,
          CRITICAL: 0
        }
      };
      break;
    case 'resource_prediction':
      specificMetrics = {
        anomalyCount: 0,
        batchSize: 0,
        confidenceScore: 0,
        usagePatternCount: 0
      };
      break;
    case 'cuda_integration':
      specificMetrics = {
        gpuUtilization: 0,
        memoryUtilization: 0,
        batchesProcessed: 0,
        processingTime: 0
      };
      break;
    case 'audio_integration':
      specificMetrics = {
        bufferUtilization: 0,
        patternExtractionsPerSecond: 0,
        audioQuality: 0
      };
      break;
    case 'blender_integration':
      specificMetrics = {
        framesPerSecond: 0,
        visiblePatterns: 0,
        renderQuality: 0
      };
      break;
    case 'api_integration':
      specificMetrics = {
        requestsPerSecond: 0,
        averageResponseTime: 0,
        activeConnections: 0,
        errorRate: 0
      };
      break;
    default:
      // Generic metrics for any other components
      specificMetrics = {
        cpuUsage: 0,
        memoryUsage: 0,
        operationsPerSecond: 0
      };
  }
/**
 * Broadcast metrics to all connected WebSocket clients
 */
function broadcastMetrics() {
  if (!wss) return;

  // Prepare metrics data
  const metricsData = {};
  metricsStore.components.forEach((componentMetrics, componentId) => {
    // Exclude history to reduce payload size
    const { history, ...metricsWithoutHistory } = componentMetrics;
    metricsData[componentId] = metricsWithoutHistory;
  });

  // Prepare system data
  const systemData = {
    uptime: Date.now() - metricsStore.system.startTime,
    components: sepBringUp.components.map(component => ({
      id: component.id,
      status: component.status
    })),
    progress: sepBringUp.getInitializationProgress()
  };

  // Broadcast to all clients
  wss.clients.forEach(client => {
    if (client.readyState === WebSocket.OPEN) {
      sendWebSocketData(client, 'metrics', metricsData);
      sendWebSocketData(client, 'system', systemData);
    }
  });
}

/**
 * Check for alerts based on metrics and thresholds
 */
function checkAlerts() {
  // Clear active alerts that are no longer valid
  alerts.active = alerts.active.filter(alert => {
    // Check if alert is still valid
    const isValid = isAlertStillValid(alert);
    
    // If not valid, move to history
    if (!isValid) {
      alert.resolvedAt = new Date();
      alerts.history.push(alert);
      
      // Limit history size
      if (alerts.history.length > alerts.maxHistory) {
        alerts.history.shift();
      }
    }
    
    return isValid;
  });

  // Check for new alerts
  metricsStore.components.forEach((metrics, componentId) => {
    const component = sepBringUp.components.find(c => c.id === componentId);
    if (!component) return;

    // Check component status
    if (component.status === 'error') {
      addAlert({
        componentId,
        metric: 'status',
        value: component.status,
        threshold: null,
        severity: 'critical',
        title: `${component.name} Error`,
        message: component.statusDetails || 'Component is in error state'
      });
    } else if (component.status === 'degraded') {
      addAlert({
        componentId,
        metric: 'status',
        value: component.status,
        threshold: null,
        severity: 'warning',
        title: `${component.name} Degraded`,
        message: component.statusDetails || 'Component is in degraded state'
      });
    }

    // Only check metrics for ready components
    if (component.status !== 'ready') return;

    // Check component-specific metrics
    checkComponentMetrics(componentId, metrics);
  });

  // Broadcast alerts to all clients
  if (wss) {
    wss.clients.forEach(client => {
      if (client.readyState === WebSocket.OPEN) {
        sendWebSocketData(client, 'alerts', alerts.active);
      }
    });
  }
}

/**
 * Check if an alert is still valid
 * @param {Object} alert - Alert to check
 * @returns {boolean} Whether the alert is still valid
 */
function isAlertStillValid(alert) {
  // Status alerts are handled separately
  if (alert.metric === 'status') {
    const component = sepBringUp.components.find(c => c.id === alert.componentId);
    return component && component.status === alert.value;
  }

  // Check if component still exists
  const metrics = metricsStore.components.get(alert.componentId);
  if (!metrics) return false;

  // Check if metric still exists
  if (!(alert.metric in metrics)) return false;

  // Get metric value
  let value = metrics[alert.metric];
  
  // Handle nested metrics
  if (alert.metric.includes('.')) {
    const parts = alert.metric.split('.');
    value = parts.reduce((obj, part) => obj && obj[part], metrics);
  }

  // Check if value still exceeds threshold
  if (alert.comparison === 'gt') {
    return value > alert.threshold;
  } else if (alert.comparison === 'lt') {
    return value < alert.threshold;
  } else if (alert.comparison === 'gte') {
    return value >= alert.threshold;
  } else if (alert.comparison === 'lte') {
    return value <= alert.threshold;
  } else if (alert.comparison === 'eq') {
    return value === alert.threshold;
  } else if (alert.comparison === 'neq') {
    return value !== alert.threshold;
  }

  return false;
}

/**
 * Check component metrics against thresholds
 * @param {string} componentId - Component ID
 * @param {Object} metrics - Component metrics
 */
function checkComponentMetrics(componentId, metrics) {
  // Check metrics based on component type
  switch (componentId) {
    case 'memory_tier_architecture':
      checkThreshold(componentId, metrics, 'stmUtilization', config.thresholds.memory.stmUtilization, 'gt');
      checkThreshold(componentId, metrics, 'mtmUtilization', config.thresholds.memory.mtmUtilization, 'gt');
      checkThreshold(componentId, metrics, 'ltmUtilization', config.thresholds.memory.ltmUtilization, 'gt');
      checkThreshold(componentId, metrics, 'fragmentation', config.thresholds.memory.fragmentation, 'gt');
      break;
      
    case 'quantum_processing_core':
      checkThreshold(componentId, metrics, 'coherence', config.thresholds.quantum.coherence, 'lt');
      checkThreshold(componentId, metrics, 'stateCorrections', config.thresholds.quantum.stateCorrections, 'gt');
      break;
      
    case 'pattern_evolution_engine':
      checkThreshold(componentId, metrics, 'stabilityScore', config.thresholds.pattern.stabilityScore, 'lt');
      checkThreshold(componentId, metrics, 'mutationRate', config.thresholds.pattern.mutationRate, 'gt');
      break;
      
    case 'context_processing_system':
      checkThreshold(componentId, metrics, 'relevanceScore', config.thresholds.context.relevanceScore, 'lt');
      checkThreshold(componentId, metrics, 'relationshipCount', config.thresholds.context.relationshipCount, 'gt');
      break;
      
    case 'resource_prediction':
      checkThreshold(componentId, metrics, 'anomalyCount', config.thresholds.resource.anomalyCount, 'gt');
      checkThreshold(componentId, metrics, 'confidenceScore', config.thresholds.resource.confidenceScore, 'lt');
      break;
      
    default:
      // Check general thresholds for other components
      if ('cpuUsage' in metrics) {
        checkThreshold(componentId, metrics, 'cpuUsage', config.thresholds.general.cpuUsage, 'gt');
      }
      if ('memoryUsage' in metrics) {
        checkThreshold(componentId, metrics, 'memoryUsage', config.thresholds.general.memoryUsage, 'gt');
      }
  }
}

/**
 * Check a metric against a threshold
 * @param {string} componentId - Component ID
 * @param {Object} metrics - Component metrics
 * @param {string} metric - Metric name
 * @param {Object} thresholds - Threshold values
 * @param {string} comparison - Comparison operator
 */
function checkThreshold(componentId, metrics, metric, thresholds, comparison) {
  // Get metric value
  let value = metrics[metric];
  
  // Handle nested metrics
  if (metric.includes('.')) {
    const parts = metric.split('.');
    value = parts.reduce((obj, part) => obj && obj[part], metrics);
  }
  
  // Skip if value is undefined
  if (value === undefined) return;
  
  const component = sepBringUp.components.find(c => c.id === componentId);
  if (!component) return;
  
  // Check critical threshold
  if (thresholds.critical !== undefined) {
    let isCritical = false;
    
    if (comparison === 'gt') {
      isCritical = value > thresholds.critical;
    } else if (comparison === 'lt') {
      isCritical = value < thresholds.critical;
    } else if (comparison === 'gte') {
      isCritical = value >= thresholds.critical;
    } else if (comparison === 'lte') {
      isCritical = value <= thresholds.critical;
    } else if (comparison === 'eq') {
      isCritical = value === thresholds.critical;
    } else if (comparison === 'neq') {
      isCritical = value !== thresholds.critical;
    }
    
    if (isCritical) {
      addAlert({
        componentId,
        metric,
        value,
        threshold: thresholds.critical,
        comparison,
        severity: 'critical',
        title: `${component.name} Critical: ${formatMetricName(metric)}`,
        message: `${formatMetricName(metric)} is ${formatComparisonOperator(comparison)} ${thresholds.critical} (current: ${formatMetricValue(value, metric)})`
      });
      
      // Skip warning check if critical
      return;
    }
  }
  
  // Check warning threshold
  if (thresholds.warning !== undefined) {
    let isWarning = false;
    
    if (comparison === 'gt') {
      isWarning = value > thresholds.warning;
    } else if (comparison === 'lt') {
      isWarning = value < thresholds.warning;
    } else if (comparison === 'gte') {
      isWarning = value >= thresholds.warning;
    } else if (comparison === 'lte') {
      isWarning = value <= thresholds.warning;
    } else if (comparison === 'eq') {
      isWarning = value === thresholds.warning;
    } else if (comparison === 'neq') {
      isWarning = value !== thresholds.warning;
    }
    
    if (isWarning) {
      addAlert({
        componentId,
        metric,
        value,
        threshold: thresholds.warning,
        comparison,
        severity: 'warning',
        title: `${component.name} Warning: ${formatMetricName(metric)}`,
        message: `${formatMetricName(metric)} is ${formatComparisonOperator(comparison)} ${thresholds.warning} (current: ${formatMetricValue(value, metric)})`
      });
    }
  }
}

/**
 * Add an alert
 * @param {Object} alert - Alert to add
 */
function addAlert(alert) {
  // Check if alert already exists
  const existingAlert = alerts.active.find(a => 
    a.componentId === alert.componentId && 
    a.metric === alert.metric && 
    a.severity === alert.severity
  );
  
  if (existingAlert) {
    // Update existing alert
    existingAlert.value = alert.value;
    existingAlert.message = alert.message;
    existingAlert.updatedAt = new Date();
  } else {
    // Add new alert
    alerts.active.push({
      id: generateAlertId(),
      componentId: alert.componentId,
      metric: alert.metric,
      value: alert.value,
      threshold: alert.threshold,
      comparison: alert.comparison,
      severity: alert.severity,
      title: alert.title,
      message: alert.message,
      timestamp: new Date(),
      updatedAt: new Date(),
      active: true
    });
  }
}

/**
 * Generate a unique alert ID
 * @returns {string} Alert ID
 */
function generateAlertId() {
  return `alert_${Date.now()}_${nudgeRandom().toString(36).substr(2, 9)}`;
}

/**
 * Format a metric name for display
 * @param {string} metric - Metric name
 * @returns {string} Formatted metric name
 */
function formatMetricName(metric) {
  // Convert camelCase to Title Case with spaces
  return metric
    .replace(/([A-Z])/g, ' $1')
    .replace(/^./, str => str.toUpperCase())
    .trim();
}

/**
 * Format a comparison operator for display
 * @param {string} operator - Comparison operator
 * @returns {string} Formatted operator
 */
function formatComparisonOperator(operator) {
  switch (operator) {
    case 'gt': return 'above';
    case 'lt': return 'below';
    case 'gte': return 'at or above';
    case 'lte': return 'at or below';
    case 'eq': return 'equal to';
    case 'neq': return 'not equal to';
    default: return operator;
  }
}

/**
 * Format a metric value for display
 * @param {*} value - Metric value
 * @param {string} metric - Metric name
 * @returns {string} Formatted value
 */
function formatMetricValue(value, metric) {
  // Format based on metric type
  if (typeof value === 'number') {
    if (value < 1 && value > 0 && metric.toLowerCase().includes('utilization')) {
      // Format as percentage
      return `${(value * 100).toFixed(1)}%`;
    } else if (value < 1 && value > 0 && (
      metric.toLowerCase().includes('score') || 
      metric.toLowerCase().includes('rate') ||
      metric.toLowerCase().includes('quality')
    )) {
      // Format as decimal
      return value.toFixed(2);
    } else if (Number.isInteger(value)) {
      // Format as integer
      return value.toString();
    } else {
      // Format as decimal
      return value.toFixed(2);
    }
  }
  
  // Default formatting
  return String(value);
}

/**
 * Register event listeners for component status changes
 */
function registerEventListeners() {
  // Listen for component status changes
  sepController.events.on('component:ready', ({ componentId }) => {
    const component = sepBringUp.components.find(c => c.id === componentId);
    if (component) {
      const metrics = metricsStore.components.get(componentId);
      if (metrics) {
        metrics.status = component.status;
        metrics.statusDetails = component.statusDetails;
        metrics.lastStatusChange = new Date();
      }
    }
  });
  
  sepController.events.on('component:error', ({ componentId, error }) => {
    const component = sepBringUp.components.find(c => c.id === componentId);
    if (component) {
      const metrics = metricsStore.components.get(componentId);
      if (metrics) {
        metrics.status = component.status;
        metrics.statusDetails = component.statusDetails;
        metrics.lastStatusChange = new Date();
      }
      
      // Add error alert
      addAlert({
        componentId,
        metric: 'status',
        value: component.status,
        threshold: null,
        severity: 'critical',
        title: `${component.name} Error`,
        message: error.message || 'Component is in error state'
      });
    }
  });
  
  sepController.events.on('component:degraded', ({ componentId }) => {
    const component = sepBringUp.components.find(c => c.id === componentId);
    if (component) {
      const metrics = metricsStore.components.get(componentId);
      if (metrics) {
        metrics.status = component.status;
        metrics.statusDetails = component.statusDetails;
        metrics.lastStatusChange = new Date();
      }
      
      // Add degraded alert
      addAlert({
        componentId,
        metric: 'status',
        value: component.status,
        threshold: null,
        severity: 'warning',
        title: `${component.name} Degraded`,
        message: component.statusDetails || 'Component is in degraded state'
      });
    }
  });
}

// Export the module
module.exports = {
  initDashboard,
  config,
  metricsStore,
  alerts
};

  // Store combined metrics
  metricsStore.components.set(componentId, {
    ...baseMetrics,
    ...specificMetrics
  });
}
  active: [], // Active alerts
  history: [], // Alert history
  maxHistory: 100 // Maximum number of historical alerts to keep
};

// WebSocket server for real-time updates
let wss;