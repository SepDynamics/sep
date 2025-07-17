import { promises as fs } from 'fs';
import { exec } from 'child_process';
import { promisify } from 'util';
import path from 'path';
import { glob } from 'glob';
import crypto from 'crypto';
// Import the redis client factory directly
import { createClient } from 'redis'; 
import CoherenceEngine from './coherence-engine.js';
// No longer need the problematic redis_integration module
// import redisIntegration from './redis_integration.js';
import { RedisMemoryTier } from './redis_memory_tier.js';
import { RedisPatternRegistry } from './redis_pattern_registry.js';

const execAsync = promisify(exec);

export class SepOllamaAgent {
    constructor(options = {}) {
        this.modelName = options.model || 'llama3:70b';
        this.contextWindow = options.contextWindow || 8192;
        this.temperature = options.temperature || 0.7;
        this.systemPrompt = options.systemPrompt || this.getDefaultSystemPrompt();
        
        // Memory tier integration
        this.memoryTier = new RedisMemoryTier();
        this.patternRegistry = new RedisPatternRegistry();
        
        // Initialize coherence engine
        this.coherenceEngine = new CoherenceEngine();
        
        // Redis client reference will be initialized in the initialize() method
        this.redisClient = null;
        
        // Codebase state tracking
        this.codebaseHash = null;
        this.lastUpdateTime = null;
        this.codebasePatterns = new Map();
        
        // Initialize Redis-based context storage
        this.contextPrefix = 'sep-ollama:context:';
        this.embeddingPrefix = 'sep-ollama:embedding:';
        this.diffHistoryPrefix = 'sep-ollama:diff:';
    }

    getDefaultSystemPrompt() {
        return `You are SEP-Ollama, a codebase-aware AI agent specialized in the SEP Engine quantum pattern processing system.

You have deep knowledge of:
- Quantum pattern processing algorithms (QBSA, QFH)
- Memory tier architecture (STM, MTM, LTM)
- Pattern coherence and stability metrics
- The entire SEP Engine codebase structure

Your primary capabilities:
1. Generate precise code diffs for requested changes
2. Understand relationships between components
3. Maintain context across the evolving codebase
4. Suggest improvements based on pattern analysis

When generating diffs, always use the unified diff format with proper context.`;
    }

    async initialize() {
        console.log('Initializing SEP-Ollama agent...');
        
        // FIXED: Initialize Redis connection directly within the agent for reliability.
        console.log('[SEP-Ollama Server] Initializing general Redis client...');
        this.redisClient = createClient(); // Assumes default host/port
        this.redisClient.on('error', err => console.error('[SEP-Ollama Agent ERROR] Redis Client Error', err));
        await this.redisClient.connect();
        console.log('[SEP-Ollama Server] General Redis client connected successfully.');
        
        // Initialize memory tier and pattern registry
        await this.memoryTier.initialize();
        await this.patternRegistry.initialize();
        
        // Check if model is available
        try {
            await execAsync(`ollama list | grep ${this.modelName}`);
            console.log(`Model ${this.modelName} is available`);
        } catch (error) {
            console.error(`Model ${this.modelName} not found. Please run: ollama pull ${this.modelName}`);
            throw error;
        }
        
        // Initialize codebase understanding
        await this.updateCodebaseKnowledge();
        
        // Set up file watcher for continuous learning
        this.setupCodebaseWatcher();
        
        console.log('SEP-Ollama agent initialized successfully');
    }

    async updateCodebaseKnowledge() {
        console.log('Updating codebase knowledge...');
        
        // Scan relevant source files
        const patterns = [
            'src/**/*.{cpp,h,hpp}',
            'mcp-tool/**/*.js',
            'docs/**/*.md',
            '.kilocode/rules/**/*.md'
        ];
        
        const files = [];
        for (const pattern of patterns) {
            const matched = await glob(pattern, { cwd: '/sep' });
            files.push(...matched);
        }
        
        // Generate codebase hash
        const hashContent = files.sort().join('\n');
        const newHash = crypto.createHash('sha256').update(hashContent).digest('hex');
        
        if (newHash !== this.codebaseHash) {
            console.log('Codebase changed, updating embeddings...');
            this.codebaseHash = newHash;
            this.lastUpdateTime = new Date();
            
            // Process files in batches
            const batchSize = 10;
            for (let i = 0; i < files.length; i += batchSize) {
                const batch = files.slice(i, i + batchSize);
                await Promise.all(batch.map(file => this.processFile(file)));
            }
            
            // Store codebase metadata
            await this.redisClient.hSet('sep-ollama:metadata', {
                codebaseHash: this.codebaseHash,
                lastUpdate: this.lastUpdateTime.toISOString(),
                fileCount: files.length.toString() // hSet expects string values
            });
        }
        
        console.log(`Processed ${files.length} files`);
    }

    async processFile(filePath) {
        try {
            const fullPath = path.join('/sep', filePath);
            const content = await fs.readFile(fullPath, 'utf8');
            
            // Extract semantic patterns from file
            const patterns = await this.extractPatterns(filePath, content);
            
            // Store in memory tier system
            for (const pattern of patterns) {
                const patternData = {
                    type: 'codebase',
                    filePath,
                    content: pattern.content,
                    coherence: pattern.coherence || 0.5,
                    stability: pattern.stability || 0.7,
                    generation: 1,
                    metadata: {
                        language: this.detectLanguage(filePath),
                        componentType: this.detectComponentType(filePath),
                        dependencies: pattern.dependencies || []
                    }
                };
                
                // Store pattern with appropriate tier
                const tier = this.determineTier(patternData);
                await this.memoryTier.storePattern(tier, pattern.id, patternData);
                
                // Track in local cache
                this.codebasePatterns.set(pattern.id, {
                    filePath,
                    tier,
                    lastAccess: new Date()
                });
            }
            
            // Store file embedding for fast retrieval
            const embedding = await this.generateEmbedding(content);
            await this.redisClient.hSet(this.embeddingPrefix + filePath, {
                embedding: JSON.stringify(embedding),
                processed: new Date().toISOString()
            });
            
        } catch (error) {
            console.error(`Error processing file ${filePath}:`, error);
        }
    }

    async extractPatterns(filePath, content) {
        const patterns = [];
        
        // Extract functions/methods
        const functionPatterns = this.extractFunctions(content, filePath);
        patterns.push(...functionPatterns);
        
        // Extract class definitions
        const classPatterns = this.extractClasses(content, filePath);
        patterns.push(...classPatterns);
        
        // Extract important comments/documentation
        const docPatterns = this.extractDocumentation(content, filePath);
        patterns.push(...docPatterns);
        
        return patterns;
    }

    extractFunctions(content, filePath) {
        const patterns = [];
        const ext = path.extname(filePath);
        
        let regex;
        if (['.cpp', '.h', '.hpp'].includes(ext)) {
            // C++ function extraction
            regex = /(?:(?:inline|static|virtual|explicit|constexpr|extern)\s+)*(?:[\w:]+\s+)*(\w+)\s*\([^)]*\)\s*(?:const)?\s*(?:override)?\s*{/g;
        } else if (['.js', '.ts'].includes(ext)) {
            // JavaScript/TypeScript function extraction
            regex = /(?:async\s+)?(?:function\s+)?(\w+)\s*\([^)]*\)\s*(?:=>)?\s*{/g;
        }
        
        if (regex) {
            let match;
            while ((match = regex.exec(content)) !== null) {
                const functionName = match[1];
                const startIdx = match.index;
                const endIdx = this.findClosingBrace(content, startIdx);
                
                patterns.push({
                    id: crypto.createHash('md5').update(`${filePath}:${functionName}`).digest('hex'),
                    type: 'function',
                    name: functionName,
                    content: content.substring(startIdx, endIdx),
                    coherence: 0.6,
                    stability: 0.8
                });
            }
        }
        
        return patterns;
    }

    extractClasses(content, filePath) {
        const patterns = [];
        const ext = path.extname(filePath);
        
        let regex;
        if (['.cpp', '.h', '.hpp'].includes(ext)) {
            regex = /class\s+(\w+)(?:\s*:\s*(?:public|private|protected)\s+[\w:]+)?\s*{/g;
        } else if (['.js', '.ts'].includes(ext)) {
            regex = /class\s+(\w+)(?:\s+extends\s+\w+)?\s*{/g;
        }
        
        if (regex) {
            let match;
            while ((match = regex.exec(content)) !== null) {
                const className = match[1];
                const startIdx = match.index;
                const endIdx = this.findClosingBrace(content, startIdx);
                
                patterns.push({
                    id: crypto.createHash('md5').update(`${filePath}:class:${className}`).digest('hex'),
                    type: 'class',
                    name: className,
                    content: content.substring(startIdx, endIdx),
                    coherence: 0.7,
                    stability: 0.9
                });
            }
        }
        
        return patterns;
    }

    extractDocumentation(content, filePath) {
        const patterns = [];
        
        // Extract multi-line comments
        const commentRegex = /\/\*\*([\s\S]*?)\*\//g;
        let match;
        while ((match = commentRegex.exec(content)) !== null) {
            const docContent = match[1].trim();
            if (docContent.length > 50) { // Only significant documentation
                patterns.push({
                    id: crypto.createHash('md5').update(`${filePath}:doc:${match.index}`).digest('hex'),
                    type: 'documentation',
                    content: docContent,
                    coherence: 0.5,
                    stability: 0.95
                });
            }
        }
        
        return patterns;
    }

    findClosingBrace(content, startIdx) {
        let braceCount = 0;
        let inString = false;
        let stringChar = null;
        
        for (let i = startIdx; i < content.length; i++) {
            const char = content[i];
            const prevChar = i > 0 ? content[i - 1] : '';
            
            // Handle string literals
            if (!inString && (char === '"' || char === "'" || char === '`')) {
                inString = true;
                stringChar = char;
            } else if (inString && char === stringChar && prevChar !== '\\') {
                inString = false;
            }
            
            // Count braces when not in string
            if (!inString) {
                if (char === '{') braceCount++;
                else if (char === '}') {
                    braceCount--;
                    if (braceCount === 0) {
                        return i + 1;
                    }
                }
            }
        }
        
        return content.length;
    }

    async generateEmbedding(content) {
        // Use coherence engine to generate semantic embedding
        const analysis = await this.coherenceEngine.analyzePatterns(content);
        
        // Create a vector representation
        const embedding = {
            coherence: analysis.coherence,
            complexity: this.calculateComplexity(content),
            semanticHash: crypto.createHash('md5').update(content).digest('hex'),
            keywords: this.extractKeywords(content),
            timestamp: new Date().toISOString()
        };
        
        return embedding;
    }

    calculateComplexity(content) {
        // Simple complexity metric based on various factors
        const lines = content.split('\n').length;
        const characters = content.length;
        const functions = (content.match(/function|def|fn|func/g) || []).length;
        const loops = (content.match(/for|while|do/g) || []).length;
        const conditions = (content.match(/if|else|switch|case/g) || []).length;
        
        return {
            lines,
            characters,
            functions,
            loops,
            conditions,
            score: (functions * 2 + loops * 3 + conditions * 1.5) / lines
        };
    }

    extractKeywords(content) {
        // Extract important identifiers and keywords
        const words = content.match(/\b[a-zA-Z_]\w*\b/g) || [];
        const frequency = {};
        
        for (const word of words) {
            if (word.length > 3) { // Skip short words
                frequency[word] = (frequency[word] || 0) + 1;
            }
        }
        
        // Return top keywords
        return Object.entries(frequency)
            .sort((a, b) => b[1] - a[1])
            .slice(0, 20)
            .map(([word]) => word);
    }

    detectLanguage(filePath) {
        const ext = path.extname(filePath);
        const langMap = {
            '.cpp': 'cpp',
            '.cc': 'cpp',
            '.cxx': 'cpp',
            '.h': 'cpp',
            '.hpp': 'cpp',
            '.js': 'javascript',
            '.mjs': 'javascript',
            '.ts': 'typescript',
            '.py': 'python',
            '.md': 'markdown'
        };
        return langMap[ext] || 'unknown';
    }

    detectComponentType(filePath) {
        if (filePath.includes('quantum')) return 'quantum';
        if (filePath.includes('memory')) return 'memory';
        if (filePath.includes('api')) return 'api';
        if (filePath.includes('mcp-tool')) return 'mcp-tool';
        if (filePath.includes('test')) return 'test';
        return 'core';
    }

    determineTier(patternData) {
        if (patternData.coherence >= 0.9 && patternData.stability >= 0.85) {
            return 'ltm';
        } else if (patternData.coherence >= 0.7) {
            return 'mtm';
        } else {
            return 'stm';
        }
    }

    setupCodebaseWatcher() {
        // Set up periodic updates
        setInterval(() => {
            this.updateCodebaseKnowledge().catch(console.error);
        }, 60 * 60 * 1000); // Update every hour
        
        // TODO: Implement file system watcher for real-time updates
    }

    async complete(prompt, options = {}) {
        // Enhance prompt with codebase context
        const enhancedPrompt = await this.enhanceWithContext(prompt);
        
        // Generate completion using Ollama
        const command = `ollama run ${this.modelName} --temperature ${options.temperature || this.temperature} "${enhancedPrompt.replace(/"/g, '\\"')}"`;
        
        try {
            const { stdout } = await execAsync(command);
            
            // Store interaction in memory tier
            await this.storeInteraction(prompt, stdout, enhancedPrompt);
            
            return stdout.trim();
        } catch (error) {
            console.error('Ollama completion error:', error);
            throw error;
        }
    }

    async enhanceWithContext(prompt) {
        // Search for relevant patterns
        const relevantPatterns = await this.findRelevantPatterns(prompt);
        
        // Build context from patterns
        let context = this.systemPrompt + '\n\n';
        
        if (relevantPatterns.length > 0) {
            context += 'Relevant codebase context:\n\n';
            
            for (const pattern of relevantPatterns) {
                const patternData = await this.memoryTier.getPattern(pattern.tier, pattern.id);
                if (patternData) {
                    context += `File: ${patternData.filePath}\n`;
                    context += `Type: ${patternData.type}\n`;
                    context += `Content:\n${patternData.content}\n\n`;
                }
            }
        }
        
        context += `User request: ${prompt}`;
        
        return context;
    }

    async findRelevantPatterns(query, limit = 5) {
        // Calculate query embedding
        const queryEmbedding = await this.generateEmbedding(query);
        
        // Search across all tiers
        const allPatterns = [];
        
        for (const [patternId, metadata] of this.codebasePatterns.entries()) {
            const pattern = await this.memoryTier.getPattern(metadata.tier, patternId);
            if (pattern) {
                // Calculate relevance score
                const score = this.calculateRelevance(queryEmbedding, pattern);
                allPatterns.push({
                    id: patternId,
                    tier: metadata.tier,
                    score,
                    ...pattern
                });
            }
        }
        
        // Sort by relevance and return top matches
        return allPatterns
            .sort((a, b) => b.score - a.score)
            .slice(0, limit);
    }

    calculateRelevance(queryEmbedding, pattern) {
        let score = 0;
        
        // Keyword matching
        const queryKeywords = queryEmbedding.keywords || [];
        const patternKeywords = pattern.metadata?.keywords || [];
        const keywordOverlap = queryKeywords.filter(k => patternKeywords.includes(k)).length;
        score += keywordOverlap * 0.1;
        
        // Coherence similarity
        score += (1 - Math.abs(queryEmbedding.coherence - pattern.coherence)) * 0.3;
        
        // Component type bonus
        if (pattern.metadata?.componentType && queryEmbedding.keywords.includes(pattern.metadata.componentType)) {
            score += 0.2;
        }
        
        return score;
    }

    async storeInteraction(prompt, response, context) {
        const interactionId = crypto.createHash('md5').update(`${prompt}:${Date.now()}`).digest('hex');
        
        const interaction = {
            id: interactionId,
            prompt,
            response,
            context,
            timestamp: new Date().toISOString(),
            coherence: 0.8, // Default coherence for interactions
            stability: 0.9
        };
        
        // Store in appropriate memory tier
        await this.memoryTier.storePattern('mtm', interactionId, interaction);
        
        // Update pattern relationships
        await this.updatePatternRelationships(interactionId, interaction);
    }

    async updatePatternRelationships(patternId, pattern) {
        // Find related patterns and update relationships
        const relatedPatterns = await this.findRelevantPatterns(pattern.prompt, 10);
        
        for (const related of relatedPatterns) {
            if (related.id !== patternId) {
                await this.patternRegistry.addRelationship(patternId, related.id, related.score);
            }
        }
    }

    async generateDiff(request) {
        const prompt = `Generate a unified diff for the following request:
        
${request}

Remember to:
1. Use proper unified diff format
2. Include adequate context (3 lines before/after)
3. Ensure the diff can be applied with 'patch' command
4. Only modify what is necessary

Generate the diff now:`;

        const response = await this.complete(prompt);
        
        // Extract diff from response
        const diffMatch = response.match(/```diff\n([\s\S]*?)```/);
        if (diffMatch) {
            const diff = diffMatch[1];
            
            // Store diff in history
            const diffId = crypto.createHash('md5').update(diff).digest('hex');
            // FIXED: Use this.redisClient instead of a non-existent redisClient
            await this.redisClient.hSet(this.diffHistoryPrefix + diffId, {
                request,
                diff,
                timestamp: new Date().toISOString()
            });
            
            return {
                diff,
                diffId,
                confidence: this.assessDiffConfidence(diff)
            };
        }
        
        throw new Error('Failed to generate valid diff');
    }

    assessDiffConfidence(diff) {
        // Simple confidence assessment based on diff characteristics
        const lines = diff.split('\n');
        const additions = lines.filter(l => l.startsWith('+')).length;
        const deletions = lines.filter(l => l.startsWith('-')).length;
        const hunks = lines.filter(l => l.startsWith('@@')).length;
        
        // Basic heuristics
        if (hunks === 0) return 0;
        if (additions + deletions === 0) return 0;
        if (additions + deletions > 1000) return 0.5; // Very large diff
        
        return Math.min(1, 0.7 + (hunks * 0.1));
    }

    async shutdown() {
        console.log('Shutting down SEP-Ollama agent...');
        // Cleanup resources
        // FIXED: Use this.redisClient and check if it exists before quitting
        if (this.redisClient) {
            await this.redisClient.quit();
        }
    }
}

// Export singleton instance
export const sepOllamaAgent = new SepOllamaAgent({
    model: 'llama3:70b',
    temperature: 0.7,
    contextWindow: 8192
});