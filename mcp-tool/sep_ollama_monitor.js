import express from 'express';
import redisIntegration from './redis_integration.js';
import { WebSocketServer } from 'ws';
import http from 'http';

// Initialize Redis connection
await redisIntegration.connect();
const redisClient = redisIntegration.client;

const app = express();
const server = http.createServer(app);
const wss = new WebSocketServer({ server });

app.use(express.static('public'));

// Metrics collection
class MetricsCollector {
    constructor() {
        this.metrics = {
            requests: {
                total: 0,
                successful: 0,
                failed: 0,
                avgResponseTime: 0
            },
            patterns: {
                stm: 0,
                mtm: 0,
                ltm: 0,
                total: 0
            },
            diffs: {
                generated: 0,
                avgConfidence: 0
            },
            codebase: {
                filesProcessed: 0,
                lastUpdate: null,
                hash: null
            },
            system: {
                memoryUsage: 0,
                cpuUsage: 0,
                uptime: 0
            }
        };
        
        this.responseTimes = [];
        this.diffConfidences = [];
    }

    async collectMetrics() {
        try {
            // Pattern metrics
            this.metrics.patterns.stm = await redisClient.hlen('memory:stm:patterns') || 0;
            this.metrics.patterns.mtm = await redisClient.hlen('memory:mtm:patterns') || 0;
            this.metrics.patterns.ltm = await redisClient.hlen('memory:ltm:patterns') || 0;
            this.metrics.patterns.total = this.metrics.patterns.stm + 
                                         this.metrics.patterns.mtm + 
                                         this.metrics.patterns.ltm;
            
            // Codebase metrics
            const metadata = await redisClient.hgetall('sep-ollama:metadata');
            if (metadata) {
                this.metrics.codebase.filesProcessed = parseInt(metadata.fileCount) || 0;
                this.metrics.codebase.lastUpdate = metadata.lastUpdate;
                this.metrics.codebase.hash = metadata.codebaseHash;
            }
            
            // System metrics
            const usage = process.memoryUsage();
            this.metrics.system.memoryUsage = Math.round(usage.heapUsed / 1024 / 1024); // MB
            this.metrics.system.uptime = process.uptime();
            
            // Calculate averages
            if (this.responseTimes.length > 0) {
                this.metrics.requests.avgResponseTime = 
                    this.responseTimes.reduce((a, b) => a + b, 0) / this.responseTimes.length;
            }
            
            if (this.diffConfidences.length > 0) {
                this.metrics.diffs.avgConfidence = 
                    this.diffConfidences.reduce((a, b) => a + b, 0) / this.diffConfidences.length;
            }
            
        } catch (error) {
            console.error('Error collecting metrics:', error);
        }
        
        return this.metrics;
    }

    recordRequest(success, responseTime) {
        this.metrics.requests.total++;
        if (success) {
            this.metrics.requests.successful++;
        } else {
            this.metrics.requests.failed++;
        }
        
        this.responseTimes.push(responseTime);
        // Keep only last 100 response times
        if (this.responseTimes.length > 100) {
            this.responseTimes.shift();
        }
    }

    recordDiff(confidence) {
        this.metrics.diffs.generated++;
        this.diffConfidences.push(confidence);
        // Keep only last 50 confidences
        if (this.diffConfidences.length > 50) {
            this.diffConfidences.shift();
        }
    }
}

const collector = new MetricsCollector();

// WebSocket connections
wss.on('connection', (ws) => {
    console.log('Monitor client connected');
    
    // Send initial metrics
    collector.collectMetrics().then(metrics => {
        ws.send(JSON.stringify({ type: 'metrics', data: metrics }));
    });
    
    // Set up periodic updates
    const interval = setInterval(async () => {
        const metrics = await collector.collectMetrics();
        if (ws.readyState === ws.OPEN) {
            ws.send(JSON.stringify({ type: 'metrics', data: metrics }));
        }
    }, 2000); // Update every 2 seconds
    
    ws.on('close', () => {
        clearInterval(interval);
        console.log('Monitor client disconnected');
    });
});

// API endpoints for metrics
app.get('/api/metrics', async (req, res) => {
    const metrics = await collector.collectMetrics();
    res.json(metrics);
});

app.get('/api/metrics/history', async (req, res) => {
    const { metric, duration = '1h' } = req.query;
    
    // TODO: Implement historical metrics storage and retrieval
    res.json({
        metric,
        duration,
        data: [],
        message: 'Historical metrics not yet implemented'
    });
});

// Monitoring dashboard HTML
app.get('/', (req, res) => {
    res.send(`
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SEP-Ollama Monitor</title>
    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            margin: 0;
            padding: 20px;
            background: #0a0a0a;
            color: #e0e0e0;
        }
        
        h1 {
            color: #00ff88;
            margin-bottom: 30px;
            font-size: 2.5em;
            text-align: center;
        }
        
        .dashboard {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            max-width: 1400px;
            margin: 0 auto;
        }
        
        .metric-card {
            background: #1a1a1a;
            border: 1px solid #333;
            border-radius: 12px;
            padding: 20px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3);
            transition: transform 0.2s;
        }
        
        .metric-card:hover {
            transform: translateY(-2px);
            border-color: #00ff88;
        }
        
        .metric-title {
            font-size: 1.2em;
            color: #00ff88;
            margin-bottom: 15px;
            font-weight: 600;
        }
        
        .metric-value {
            font-size: 2.5em;
            font-weight: bold;
            color: #fff;
            margin-bottom: 5px;
        }
        
        .metric-label {
            font-size: 0.9em;
            color: #888;
        }
        
        .metric-grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 10px;
            margin-top: 10px;
        }
        
        .sub-metric {
            background: #252525;
            padding: 10px;
            border-radius: 6px;
            text-align: center;
        }
        
        .sub-metric-value {
            font-size: 1.5em;
            font-weight: bold;
            color: #00ff88;
        }
        
        .sub-metric-label {
            font-size: 0.8em;
            color: #aaa;
            margin-top: 5px;
        }
        
        .status-indicator {
            display: inline-block;
            width: 10px;
            height: 10px;
            border-radius: 50%;
            margin-right: 5px;
            background: #00ff88;
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0% { opacity: 1; }
            50% { opacity: 0.5; }
            100% { opacity: 1; }
        }
        
        .error { color: #ff4444; }
        .warning { color: #ffaa00; }
        .success { color: #00ff88; }
        
        .progress-bar {
            width: 100%;
            height: 8px;
            background: #333;
            border-radius: 4px;
            overflow: hidden;
            margin-top: 10px;
        }
        
        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, #00ff88, #00cc66);
            transition: width 0.3s ease;
        }
        
        .timestamp {
            font-size: 0.8em;
            color: #666;
            text-align: center;
            margin-top: 20px;
        }
    </style>
</head>
<body>
    <h1>🚀 SEP-Ollama Monitor</h1>
    
    <div class="dashboard">
        <div class="metric-card">
            <div class="metric-title">
                <span class="status-indicator"></span>
                Request Metrics
            </div>
            <div class="metric-value" id="total-requests">0</div>
            <div class="metric-label">Total Requests</div>
            <div class="metric-grid">
                <div class="sub-metric">
                    <div class="sub-metric-value success" id="successful-requests">0</div>
                    <div class="sub-metric-label">Successful</div>
                </div>
                <div class="sub-metric">
                    <div class="sub-metric-value error" id="failed-requests">0</div>
                    <div class="sub-metric-label">Failed</div>
                </div>
                <div class="sub-metric">
                    <div class="sub-metric-value" id="avg-response-time">0ms</div>
                    <div class="sub-metric-label">Avg Response</div>
                </div>
                <div class="sub-metric">
                    <div class="sub-metric-value" id="success-rate">100%</div>
                    <div class="sub-metric-label">Success Rate</div>
                </div>
            </div>
        </div>
        
        <div class="metric-card">
            <div class="metric-title">Memory Tiers</div>
            <div class="metric-value" id="total-patterns">0</div>
            <div class="metric-label">Total Patterns</div>
            <div class="metric-grid">
                <div class="sub-metric">
                    <div class="sub-metric-value" id="stm-patterns">0</div>
                    <div class="sub-metric-label">STM</div>
                </div>
                <div class="sub-metric">
                    <div class="sub-metric-value" id="mtm-patterns">0</div>
                    <div class="sub-metric-label">MTM</div>
                </div>
                <div class="sub-metric">
                    <div class="sub-metric-value" id="ltm-patterns">0</div>
                    <div class="sub-metric-label">LTM</div>
                </div>
                <div class="sub-metric">
                    <div class="sub-metric-value" id="memory-usage">0%</div>
                    <div class="sub-metric-label">Usage</div>
                </div>
            </div>
            <div class="progress-bar">
                <div class="progress-fill" id="memory-progress" style="width: 0%"></div>
            </div>
        </div>
        
        <div class="metric-card">
            <div class="metric-title">Diff Generation</div>
            <div class="metric-value" id="diffs-generated">0</div>
            <div class="metric-label">Diffs Generated</div>
            <div class="metric-grid">
                <div class="sub-metric">
                    <div class="sub-metric-value" id="avg-confidence">0%</div>
                    <div class="sub-metric-label">Avg Confidence</div>
                </div>
                <div class="sub-metric">
                    <div class="sub-metric-value" id="diff-rate">0/h</div>
                    <div class="sub-metric-label">Generation Rate</div>
                </div>
            </div>
        </div>
        
        <div class="metric-card">
            <div class="metric-title">Codebase</div>
            <div class="metric-value" id="files-processed">0</div>
            <div class="metric-label">Files Processed</div>
            <div class="metric-grid">
                <div class="sub-metric" style="grid-column: span 2;">
                    <div class="sub-metric-value" id="last-update">Never</div>
                    <div class="sub-metric-label">Last Update</div>
                </div>
            </div>
            <div style="margin-top: 10px; font-size: 0.8em; color: #666; word-break: break-all;">
                Hash: <span id="codebase-hash">-</span>
            </div>
        </div>
        
        <div class="metric-card">
            <div class="metric-title">System</div>
            <div class="metric-value" id="memory-usage-mb">0 MB</div>
            <div class="metric-label">Memory Usage</div>
            <div class="metric-grid">
                <div class="sub-metric" style="grid-column: span 2;">
                    <div class="sub-metric-value" id="uptime">0s</div>
                    <div class="sub-metric-label">Uptime</div>
                </div>
            </div>
        </div>
    </div>
    
    <div class="timestamp" id="last-updated">Last updated: Never</div>
    
    <script>
        const ws = new WebSocket('ws://localhost:3002');
        
        ws.onmessage = (event) => {
            const message = JSON.parse(event.data);
            if (message.type === 'metrics') {
                updateMetrics(message.data);
            }
        };
        
        ws.onerror = (error) => {
            console.error('WebSocket error:', error);
        };
        
        function updateMetrics(metrics) {
            // Request metrics
            document.getElementById('total-requests').textContent = metrics.requests.total;
            document.getElementById('successful-requests').textContent = metrics.requests.successful;
            document.getElementById('failed-requests').textContent = metrics.requests.failed;
            document.getElementById('avg-response-time').textContent = 
                Math.round(metrics.requests.avgResponseTime) + 'ms';
            
            const successRate = metrics.requests.total > 0 
                ? (metrics.requests.successful / metrics.requests.total * 100).toFixed(1)
                : 100;
            document.getElementById('success-rate').textContent = successRate + '%';
            
            // Pattern metrics
            document.getElementById('total-patterns').textContent = metrics.patterns.total;
            document.getElementById('stm-patterns').textContent = metrics.patterns.stm;
            document.getElementById('mtm-patterns').textContent = metrics.patterns.mtm;
            document.getElementById('ltm-patterns').textContent = metrics.patterns.ltm;
            
            const maxPatterns = 1000 + 10000 + 100000; // STM + MTM + LTM limits
            const usagePercent = (metrics.patterns.total / maxPatterns * 100).toFixed(1);
            document.getElementById('memory-usage').textContent = usagePercent + '%';
            document.getElementById('memory-progress').style.width = usagePercent + '%';
            
            // Diff metrics
            document.getElementById('diffs-generated').textContent = metrics.diffs.generated;
            document.getElementById('avg-confidence').textContent = 
                (metrics.diffs.avgConfidence * 100).toFixed(1) + '%';
            
            const diffRate = metrics.system.uptime > 0 
                ? (metrics.diffs.generated / (metrics.system.uptime / 3600)).toFixed(1)
                : 0;
            document.getElementById('diff-rate').textContent = diffRate + '/h';
            
            // Codebase metrics
            document.getElementById('files-processed').textContent = metrics.codebase.filesProcessed;
            document.getElementById('last-update').textContent = 
                metrics.codebase.lastUpdate 
                    ? new Date(metrics.codebase.lastUpdate).toLocaleString()
                    : 'Never';
            document.getElementById('codebase-hash').textContent = 
                metrics.codebase.hash ? metrics.codebase.hash.substring(0, 8) : '-';
            
            // System metrics
            document.getElementById('memory-usage-mb').textContent = metrics.system.memoryUsage + ' MB';
            document.getElementById('uptime').textContent = formatUptime(metrics.system.uptime);
            
            // Update timestamp
            document.getElementById('last-updated').textContent = 
                'Last updated: ' + new Date().toLocaleTimeString();
        }
        
        function formatUptime(seconds) {
            if (seconds < 60) return Math.round(seconds) + 's';
            if (seconds < 3600) return Math.round(seconds / 60) + 'm';
            if (seconds < 86400) return Math.round(seconds / 3600) + 'h';
            return Math.round(seconds / 86400) + 'd';
        }
    </script>
</body>
</html>
    `);
});

// Export collector for use in main server
export { collector };

const PORT = process.env.MONITOR_PORT || 3002;
server.listen(PORT, () => {
    console.log(`SEP-Ollama Monitor running on http://localhost:${PORT}`);
});