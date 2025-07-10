import { promises as fs } from 'fs';
import { join } from 'path';
import CoherenceEngine from './quantum/coherence-engine.js';
import AdaptiveTierManager from './memory/adaptive-tier-manager.js';
import AdvancedContextProcessor from './context/advanced-processor.js';
import SelfEvolution from './quantum/self-evolution.js';
import PatternManifold from './quantum/pattern-manifold.js';
import { readFileSync, writeFileSync, existsSync, mkdirSync } from 'fs';

/**
 * AutonomousBrain - Core intelligence system for SEP with enhanced
 * quantum-inspired learning capabilities and autonomous adaptation
 */
export default class AutonomousBrain {
    constructor(config = {}) {
        this.config = {
            coherenceThresholds: {
                minimum: 0.1,           // Minimum viability threshold
                mtmPromotion: 0.7,      // STM → MTM promotion threshold
                ltmPromotion: 0.9,      // MTM → LTM promotion threshold
                demotion: 0.3,          // Demotion threshold
                relationshipMinimum: 0.6 // Minimum relationship strength
            },
            memoryPaths: {
                stm: './quantum-memory/stm',
                mtm: './quantum-memory/mtm',
                ltm: './quantum-memory/ltm',
                manifolds: './quantum-memory/manifolds'
            },
            evolutionConfig: {
                autoEvolutionInterval: 3600000, // 1 hour in milliseconds
                adaptationRate: 0.05,           // Rate of self-adaptation
                minSamplesBeforeAdaptation: 10  // Minimum interactions before adapting
            },
            ...config
        };
        
        // Initialize the core components
        this.coherenceEngine = new CoherenceEngine(this.config.coherenceThresholds);
        this.memoryManager = new AdaptiveTierManager(this.config.memoryPaths);
        this.contextProcessor = new AdvancedContextProcessor();
        this.selfEvolution = new SelfEvolution(this.config.evolutionConfig);
        this.patternManifold = new PatternManifold();
        
        // Sequential ID system - the foundation of relevance tracking
        this.idCounter = this.loadIdCounter();
        this.idMap = new Map(); // Maps entities to their sequential IDs
        
        this.interactionCounter = 0;
        this.lastEvolutionTime = Date.now();
        
        // Ensure all directories exist
        this.initializeSystem();
        
        // Start the autonomous evolution process
        this.startAutoEvolution();
    }
    
    async initializeSystem() {
        try {
            // Ensure all memory directories exist
            for (const path of Object.values(this.config.memoryPaths)) {
                await fs.mkdir(path, { recursive: true });
            }
            
            // Initialize all components
            await this.coherenceEngine.initialize();
            await this.memoryManager.initialize();
            await this.contextProcessor.initialize();
            await this.patternManifold.initialize();
            
            console.log('Autonomous Brain initialized successfully');
        } catch (error) {
            console.error('Error initializing system:', error);
        }
    }
    
    /**
     * The core processing function that handles all input with minimal user guidance
     * @param {Object} input - The input to process
     * @returns {Object} - The processing result
     */
    async process(input) {
        try {
            this.interactionCounter++;
            
            // Phase 1: Context understanding with minimal input requirements
            const contextResult = await this.contextProcessor.extractContext(input);
            
            // Phase 2: Pattern recognition and quantum-inspired state collapse
            const patternResult = await this.coherenceEngine.analyzePatterns(contextResult);
            
            // Phase 3: Determine the most coherent interpretation through quantum collapse
            const quantumState = await this.determineQuantumState(patternResult);
            
            // Phase 4: Memory placement based on coherence scores
            const memoryResult = await this.storeInAppropriateMemory(quantumState);
            
            // Phase 5: Update relationship strengths in the computational manifold
            await this.patternManifold.updateRelationships(memoryResult);
            
            // Check if we should trigger autonomous evolution
            this.checkForEvolutionTrigger();
            
            // Adapt to user interaction patterns
            if (this.interactionCounter >= this.config.evolutionConfig.minSamplesBeforeAdaptation) {
                await this.adaptToUserPatterns(input, contextResult);
            }
            
            return {
                response: quantumState.response,
                coherence: quantumState.coherence,
                memory_tier: memoryResult.tier,
                adaptiveInsights: this.getAdaptiveInsights(quantumState, memoryResult)
            };
        } catch (error) {
            console.error('Error in autonomous processing:', error);
            return {
                status: 'error',
                error: error.message,
                fallbackResponse: 'I encountered an issue but am learning from it.'
            };
        }
    }
    
    /**
     * Determines the most coherent quantum state from potential interpretations
     * @param {Object} patternResult - The result of pattern analysis
     * @returns {Object} - The collapsed quantum state
     */
    async determineQuantumState(patternResult) {
        // Get all potential interpretations of the input
        const potentialStates = patternResult.potentialStates || [];
        let collapsedState = null;
        
        if (potentialStates.length > 0) {
            // Sort by coherence and collapse to highest coherence state
            potentialStates.sort((a, b) => b.coherence - a.coherence);
            collapsedState = potentialStates[0];
            
            // Log information about the collapse for later learning
            await this.logStateCollapse(potentialStates, collapsedState);
        } else {
            // Create new state if no potential states exist
            collapsedState = {
                id: this.getNextId('state'),
                position: this.patternManifold.calculateManifoldPosition(patternResult),
                coherence: patternResult.coherence || this.config.coherenceThresholds.minimum,
                patterns: patternResult.patterns || [],
                response: this.generateResponse(patternResult)
            };
        }
        
        return {
            ...collapsedState,
            collapsedAt: new Date().toISOString(),
            originalCoherence: patternResult.coherence
        };
    }
    
    /**
     * Stores the processed information in the appropriate memory tier
     * @param {Object} quantumState - The collapsed quantum state
     * @returns {Object} - Information about where the memory was stored
     */
    async storeInAppropriateMemory(quantumState) {
        const coherence = quantumState.coherence || this.config.coherenceThresholds.minimum;
        let tier = 'stm';
        
        // Determine memory tier based on coherence thresholds
        if (coherence >= this.config.coherenceThresholds.ltmPromotion) {
            tier = 'ltm';
        } else if (coherence >= this.config.coherenceThresholds.mtmPromotion) {
            tier = 'mtm';
        }
        
        // Store in appropriate memory tier
        const memoryItem = {
            id: quantumState.id || this.generateId(),
            coherence,
            position: quantumState.position,
            patterns: quantumState.patterns,
            metadata: {
                timestamp: new Date().toISOString(),
                generation: 1
            }
        };
        
        const result = await this.memoryManager.store(tier, memoryItem);
        
        return {
            tier,
            id: memoryItem.id,
            ...result
        };
    }
    
    /**
     * Logs information about state collapse for learning purposes
     * @param {Array} potentialStates - All potential interpretations
     * @param {Object} collapsedState - The chosen interpretation
     */
    async logStateCollapse(potentialStates, collapsedState) {
        try {
            const collapseLog = {
                timestamp: new Date().toISOString(),
                potentialStates: potentialStates.map(s => ({
                    id: s.id,
                    coherence: s.coherence
                })),
                chosenState: {
                    id: collapsedState.id,
                    coherence: collapsedState.coherence
                },
                decisionFactors: {
                    coherenceDelta: potentialStates.length > 1 ? 
                        collapsedState.coherence - potentialStates[1].coherence : 0,
                    confidenceScore: this.calculateConfidence(potentialStates, collapsedState)
                }
            };
            
            const logPath = join(this.config.memoryPaths.manifolds, 'collapse_logs.json');
            let logs = [];
            
            try {
                const existingData = await fs.readFile(logPath, 'utf8');
                logs = JSON.parse(existingData);
            } catch (err) {
                // File doesn't exist yet, that's fine
            }
            
            logs.push(collapseLog);
            await fs.writeFile(logPath, JSON.stringify(logs, null, 2));
        } catch (error) {
            console.error('Error logging state collapse:', error);
        }
    }
    
    /**
     * Calculates confidence in the chosen state
     * @param {Array} states - All potential states
     * @param {Object} chosen - The chosen state
     * @returns {number} - Confidence score between 0-1
     */
    calculateConfidence(states, chosen) {
        if (states.length <= 1) return 1;
        
        // Calculate coherence gap between top choice and alternatives
        const coherenceGap = chosen.coherence - states[1].coherence;
        
        // Higher gap means higher confidence
        return Math.min(1, Math.max(0, coherenceGap / 0.3));
    }
    
    /**
     * Generates a response based on the pattern result
     * @param {Object} patternResult - The result of pattern analysis
     * @returns {Object} - The generated response
     */
    generateResponse(patternResult) {
        // This would be more sophisticated in a real implementation
        return {
            message: patternResult.message || "I've processed your input.",
            confidence: patternResult.coherence || 0.5,
            suggestedNextSteps: patternResult.suggestedNextSteps || []
        };
    }
    
    /**
     * Provides insights into how the system is adapting
     * @param {Object} state - The quantum state
     * @param {Object} memory - The memory storage result
     * @returns {Object} - Insights about adaptation
     */
    getAdaptiveInsights(state, memory) {
        return {
            adaptationRate: this.config.evolutionConfig.adaptationRate,
            interactionCount: this.interactionCounter,
            lastEvolution: new Date(this.lastEvolutionTime).toISOString(),
            coherenceTrend: this.coherenceEngine.getCoherenceTrend(),
            storageOptimization: this.memoryManager.getOptimizationMetrics()
        };
    }
    
    /**
     * Starts the autonomous evolution process
     */
    startAutoEvolution() {
        const interval = this.config.evolutionConfig.autoEvolutionInterval;
        setInterval(() => this.evolve(), interval);
        console.log(`Autonomous evolution scheduled every ${interval/60000} minutes`);
    }
    
    /**
     * Checks if evolution should be triggered
     */
    checkForEvolutionTrigger() {
        const now = Date.now();
        const timeSinceLastEvolution = now - this.lastEvolutionTime;
        
        // Trigger evolution if enough time has passed
        if (timeSinceLastEvolution >= this.config.evolutionConfig.autoEvolutionInterval) {
            this.evolve();
        }
    }
    
    /**
     * Core evolution function that improves the system over time
     */
    async evolve() {
        try {
            console.log('Starting autonomous evolution...');
            this.lastEvolutionTime = Date.now();
            
            // Phase 1: Analyze patterns across memory tiers
            const patterns = await this.coherenceEngine.analyzePatternsAcrossTiers();
            
            // Phase 2: Promote/demote memory items based on coherence
            await this.memoryManager.rebalanceMemoryTiers(patterns);
            
            // Phase 3: Update manifold topology based on pattern relationships
            await this.patternManifold.evolveManifold(patterns);
            
            // Phase 4: Optimize thresholds based on system performance
            await this.optimizeThresholds(patterns);
            
            // Phase 5: Self-improve the processing algorithms
            await this.selfEvolution.improveProcessing(patterns);
            
            console.log('Evolution completed successfully');
            return {
                status: 'evolved',
                timestamp: new Date().toISOString(),
                improvements: {
                    patterns: patterns.evolved?.length || 0,
                    thresholds: this.coherenceEngine.getThresholdChanges(),
                    algorithms: this.selfEvolution.getImprovementMetrics()
                }
            };
        } catch (error) {
            console.error('Error during evolution:', error);
            return {
                status: 'error',
                error: error.message
            };
        }
    }
    
    /**
     * Adapts to user interaction patterns to improve future responses
     * @param {Object} input - The user input
     * @param {Object} contextResult - The extracted context
     */
    async adaptToUserPatterns(input, contextResult) {
        try {
            // Track user interaction patterns
            await this.contextProcessor.trackInteractionPattern(input, contextResult);
            
            // Adjust coherence thresholds based on user feedback patterns
            const adaptationRate = this.config.evolutionConfig.adaptationRate;
            
            // Example: If we're consistently getting high coherence inputs, increase thresholds
            const recentCoherenceAvg = this.coherenceEngine.getRecentCoherenceAverage();
            
            if (recentCoherenceAvg > this.config.coherenceThresholds.mtmPromotion + 0.15) {
                this.config.coherenceThresholds.mtmPromotion += adaptationRate * 0.1;
                this.config.coherenceThresholds.ltmPromotion += adaptationRate * 0.05;
                console.log('Adapted coherence thresholds upward based on user patterns');
            } else if (recentCoherenceAvg < this.config.coherenceThresholds.mtmPromotion - 0.15) {
                this.config.coherenceThresholds.mtmPromotion -= adaptationRate * 0.1;
                this.config.coherenceThresholds.ltmPromotion -= adaptationRate * 0.05;
                console.log('Adapted coherence thresholds downward based on user patterns');
            }
            
            // Keep thresholds within sensible bounds
            this.config.coherenceThresholds.mtmPromotion = Math.min(0.9, Math.max(0.4, this.config.coherenceThresholds.mtmPromotion));
            this.config.coherenceThresholds.ltmPromotion = Math.min(0.98, Math.max(0.7, this.config.coherenceThresholds.ltmPromotion));
            
            // Update the coherence engine with the new thresholds
            this.coherenceEngine.updateThresholds(this.config.coherenceThresholds);
        } catch (error) {
            console.error('Error adapting to user patterns:', error);
        }
    }
    
    /**
     * Optimizes thresholds based on system performance
     * @param {Object} patterns - The patterns from evolution
     */
    async optimizeThresholds(patterns) {
        // Analyze memory tier distributions
        const tierDistribution = await this.memoryManager.getTierDistribution();
        
        // If STM is overloaded, adjust thresholds to promote more to MTM
        if (tierDistribution.stm > 0.7) {
            this.config.coherenceThresholds.mtmPromotion -= 0.05;
            console.log('Lowering MTM promotion threshold due to STM overload');
        }
        
        // If LTM is too empty, adjust thresholds to promote more
        if (tierDistribution.ltm < 0.1) {
            this.config.coherenceThresholds.ltmPromotion -= 0.05;
            console.log('Lowering LTM promotion threshold to grow long-term memory');
        }
        
        // Keep thresholds within sensible bounds
        this.config.coherenceThresholds.mtmPromotion = Math.min(0.9, Math.max(0.4, this.config.coherenceThresholds.mtmPromotion));
        this.config.coherenceThresholds.ltmPromotion = Math.min(0.98, Math.max(0.7, this.config.coherenceThresholds.ltmPromotion));
        
        // Update the coherence engine with the new thresholds
        this.coherenceEngine.updateThresholds(this.config.coherenceThresholds);
    }
    
    /**
     * Gets the next sequential ID for an entity type
     * This is the foundation of relevance - sequential IDs show the path of information
     *
     * @param {string} entityType - The type of entity (state, memory, pattern, etc.)
     * @returns {number} - The next sequential ID
     */
    getNextId(entityType) {
        // Increment the global counter first
        this.idCounter++;
        
        // Store the ID mapping for this entity type if needed
        if (!this.idMap.has(entityType)) {
            this.idMap.set(entityType, []);
        }
        
        // Add the new ID to the mapping
        const typeIds = this.idMap.get(entityType);
        typeIds.push(this.idCounter);
        
        // Persist the ID counter to disk to ensure continuity even after restarts
        this.persistIdCounter();
        
        return this.idCounter;
    }
    
    /**
     * Loads the ID counter from persistent storage
     * @returns {number} - The current ID counter value
     */
    loadIdCounter() {
        try {
            // Create the directory if it doesn't exist
            const dirPath = join(this.config.memoryPaths.manifolds);
            if (!existsSync(dirPath)) {
                mkdirSync(dirPath, { recursive: true });
            }
            
            const idFilePath = join(dirPath, 'id_counter.json');
            if (existsSync(idFilePath)) {
                const data = JSON.parse(readFileSync(idFilePath, 'utf8'));
                return data.counter || 1;
            }
        } catch (error) {
            console.error('Error loading ID counter, starting from 1:', error);
        }
        return 1; // Start from 1 if no existing counter
    }
    
    /**
     * Persists the ID counter to disk
     */
    persistIdCounter() {
        try {
            const dirPath = join(this.config.memoryPaths.manifolds);
            if (!existsSync(dirPath)) {
                mkdirSync(dirPath, { recursive: true });
            }
            
            const idFilePath = join(dirPath, 'id_counter.json');
            writeFileSync(idFilePath, JSON.stringify({
                counter: this.idCounter,
                lastUpdated: new Date().toISOString()
            }, null, 2));
        } catch (error) {
            console.error('Error persisting ID counter:', error);
        }
    }
}