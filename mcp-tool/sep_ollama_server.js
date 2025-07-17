import express from 'express';
import cors from 'cors';
import { sepOllamaAgent } from './sep_ollama_agent.js';
import redisIntegration from './redis_integration.js';
import AutonomousBrain from './autonomous-brain.js';

// Initialize autonomous brain instance
const autonomousBrain = new AutonomousBrain();

// Initialize Redis connection
await redisIntegration.connect();
const redisClient = redisIntegration.client;

const app = express();
app.use(express.json({ limit: '50mb' }));
app.use(cors());

// Initialize agent on startup
let agentReady = false;
sepOllamaAgent.initialize()
    .then(() => {
        agentReady = true;
        console.log('SEP-Ollama agent ready');
    })
    .catch(err => {
        console.error('Failed to initialize agent:', err);
        process.exit(1);
    });

// Health check endpoint
app.get('/health', (req, res) => {
    res.json({
        status: agentReady ? 'ready' : 'initializing',
        model: sepOllamaAgent.modelName,
        codebaseHash: sepOllamaAgent.codebaseHash,
        lastUpdate: sepOllamaAgent.lastUpdateTime
    });
});

// Main completion endpoint
app.post('/api/v1/complete', async (req, res) => {
    if (!agentReady) {
        return res.status(503).json({ error: 'Agent not ready' });
    }

    try {
        const { prompt, temperature, useContext = true } = req.body;
        
        if (!prompt) {
            return res.status(400).json({ error: 'Prompt is required' });
        }

        // Get completion from agent with temperature option
        const completion = await sepOllamaAgent.complete(prompt, {
            temperature
        });

        res.json({
            completion,
            metadata: {
                model: sepOllamaAgent.modelName,
                temperature: temperature || sepOllamaAgent.temperature,
                timestamp: new Date().toISOString()
            }
        });
    } catch (error) {
        console.error('Completion error:', error);
        res.status(500).json({ 
            error: 'Completion failed',
            message: error.message 
        });
    }
});

// Diff generation endpoint
app.post('/api/v1/generate-diff', async (req, res) => {
    if (!agentReady) {
        return res.status(503).json({ error: 'Agent not ready' });
    }

    try {
        const { request, targetFiles } = req.body;
        
        if (!request) {
            return res.status(400).json({ error: 'Request is required' });
        }

        // Enhance request with target files if provided
        let enhancedRequest = request;
        if (targetFiles && targetFiles.length > 0) {
            enhancedRequest += '\n\nTarget files:\n' + targetFiles.join('\n');
        }

        const result = await sepOllamaAgent.generateDiff(enhancedRequest);
        
        res.json({
            diff: result.diff,
            diffId: result.diffId,
            confidence: result.confidence,
            canApply: result.confidence > 0.7
        });
    } catch (error) {
        console.error('Diff generation error:', error);
        res.status(500).json({ 
            error: 'Diff generation failed',
            message: error.message 
        });
    }
});

// Pattern search endpoint
app.post('/api/v1/search-patterns', async (req, res) => {
    if (!agentReady) {
        return res.status(503).json({ error: 'Agent not ready' });
    }

    try {
        const { query, limit = 10 } = req.body;
        
        if (!query) {
            return res.status(400).json({ error: 'Query is required' });
        }

        const patterns = await sepOllamaAgent.findRelevantPatterns(query, limit);
        
        res.json({
            patterns: patterns.map(p => ({
                id: p.id,
                type: p.type,
                filePath: p.filePath,
                score: p.score,
                coherence: p.coherence,
                stability: p.stability,
                tier: p.tier
            })),
            totalFound: patterns.length
        });
    } catch (error) {
        console.error('Pattern search error:', error);
        res.status(500).json({ 
            error: 'Pattern search failed',
            message: error.message 
        });
    }
});

// Codebase update endpoint
app.post('/api/v1/update-codebase', async (req, res) => {
    if (!agentReady) {
        return res.status(503).json({ error: 'Agent not ready' });
    }

    try {
        // Trigger codebase knowledge update
        await sepOllamaAgent.updateCodebaseKnowledge();
        
        res.json({
            success: true,
            codebaseHash: sepOllamaAgent.codebaseHash,
            lastUpdate: sepOllamaAgent.lastUpdateTime,
            patternsStored: sepOllamaAgent.codebasePatterns.size
        });
    } catch (error) {
        console.error('Codebase update error:', error);
        res.status(500).json({ 
            error: 'Codebase update failed',
            message: error.message 
        });
    }
});

// Memory tier statistics endpoint
app.get('/api/v1/memory-stats', async (req, res) => {
    try {
        const stats = {
            stm: await redisClient.hlen('memory:stm:patterns'),
            mtm: await redisClient.hlen('memory:mtm:patterns'),
            ltm: await redisClient.hlen('memory:ltm:patterns'),
            totalPatterns: sepOllamaAgent.codebasePatterns.size,
            interactions: await redisClient.hlen('sep-ollama:interactions'),
            diffs: await redisClient.keys('sep-ollama:diff:*').then(keys => keys.length)
        };
        
        res.json(stats);
    } catch (error) {
        console.error('Stats error:', error);
        res.status(500).json({ 
            error: 'Failed to get memory stats',
            message: error.message 
        });
    }
});

// Context management endpoint
app.post('/api/v1/context', async (req, res) => {
    if (!agentReady) {
        return res.status(503).json({ error: 'Agent not ready' });
    }

    try {
        const { action, contextId, data } = req.body;
        
        switch (action) {
            case 'save':
                if (!contextId || !data) {
                    return res.status(400).json({ error: 'contextId and data required' });
                }
                await redisClient.hset(`sep-ollama:context:${contextId}`, data);
                res.json({ success: true, contextId });
                break;
                
            case 'load':
                if (!contextId) {
                    return res.status(400).json({ error: 'contextId required' });
                }
                const context = await redisClient.hgetall(`sep-ollama:context:${contextId}`);
                res.json({ context });
                break;
                
            case 'delete':
                if (!contextId) {
                    return res.status(400).json({ error: 'contextId required' });
                }
                await redisClient.del(`sep-ollama:context:${contextId}`);
                res.json({ success: true });
                break;
                
            default:
                res.status(400).json({ error: 'Invalid action' });
        }
    } catch (error) {
        console.error('Context management error:', error);
        res.status(500).json({ 
            error: 'Context operation failed',
            message: error.message 
        });
    }
});

// Diff history endpoint
app.get('/api/v1/diff-history', async (req, res) => {
    try {
        const { limit = 10, offset = 0 } = req.query;
        
        const diffKeys = await redisClient.keys('sep-ollama:diff:*');
        const paginatedKeys = diffKeys.slice(offset, offset + limit);
        
        const diffs = await Promise.all(
            paginatedKeys.map(async key => {
                const diffData = await redisClient.hgetall(key);
                return {
                    diffId: key.split(':').pop(),
                    ...diffData
                };
            })
        );
        
        res.json({
            diffs: diffs.sort((a, b) => 
                new Date(b.timestamp) - new Date(a.timestamp)
            ),
            total: diffKeys.length,
            limit: parseInt(limit),
            offset: parseInt(offset)
        });
    } catch (error) {
        console.error('Diff history error:', error);
        res.status(500).json({ 
            error: 'Failed to get diff history',
            message: error.message 
        });
    }
});

// Apply diff endpoint (experimental)
app.post('/api/v1/apply-diff', async (req, res) => {
    if (!agentReady) {
        return res.status(503).json({ error: 'Agent not ready' });
    }

    try {
        const { diffId, dryRun = true } = req.body;
        
        if (!diffId) {
            return res.status(400).json({ error: 'diffId required' });
        }

        const diffData = await redisClient.hgetall(`sep-ollama:diff:${diffId}`);
        if (!diffData || !diffData.diff) {
            return res.status(404).json({ error: 'Diff not found' });
        }

        // For now, just return the diff content
        // TODO: Implement actual diff application with proper validation
        res.json({
            diffId,
            diff: diffData.diff,
            request: diffData.request,
            timestamp: diffData.timestamp,
            dryRun,
            warning: 'Diff application not yet implemented. Please apply manually.'
        });
    } catch (error) {
        console.error('Apply diff error:', error);
        res.status(500).json({ 
            error: 'Failed to apply diff',
            message: error.message 
        });
    }
});

// Graceful shutdown
process.on('SIGINT', async () => {
    console.log('\nShutting down SEP-Ollama server...');
    await sepOllamaAgent.shutdown();
    process.exit(0);
});

const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
    console.log(`SEP-Ollama server running on port ${PORT}`);
});