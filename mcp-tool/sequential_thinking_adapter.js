/**
 * Sequential Thinking Adapter
 * Handles integration between Sequential Thinking MCP server and Redis Memory Tier
 */

import { BaseMCPAdapter } from './sep_mcp_controller.js';

class SequentialThinkingAdapter extends BaseMCPAdapter {
    constructor(sequentialServer, memoryTier) {
        super(sequentialServer, memoryTier);
        
        // Initialize coherence weights for different aspects
        this.coherenceWeights = {
            stepFlow: 0.4,      // Logical flow between steps
            goalAlignment: 0.3,  // Relevance to goal
            depth: 0.2,         // Depth of reasoning
            consistency: 0.1     // Consistency of approach
        };
    }

    async processRequest(toolName, args) {
        try {
            // Execute original tool request
            const result = await super.processRequest(toolName, args);

            // Special handling for think tool
            if (toolName === 'think') {
                await this.processReasoningChain(result, args.goal);
            }

            return result;
        } catch (error) {
            console.error('Error in Sequential Thinking Adapter:', error);
            throw error;
        }
    }

    calculateReasoningCoherence(steps, goal) {
        // Calculate different coherence aspects
        const stepFlowScore = this.sep_calculateStepFlowCoherence(steps);
        const goalAlignmentScore = this.sep_calculateGoalAlignmentCoherence(steps, goal);
        const depthScore = this.sep_calculateDepthCoherence(steps);
        const consistencyScore = this.sep_calculateConsistencyCoherence(steps);

        // Weighted average of all aspects
        const totalCoherence = (
            stepFlowScore * this.coherenceWeights.stepFlow +
            goalAlignmentScore * this.coherenceWeights.goalAlignment +
            depthScore * this.coherenceWeights.depth +
            consistencyScore * this.coherenceWeights.consistency
        );

        return Math.max(0.1, Math.min(1.0, totalCoherence));
    }

    sep_calculateStepFlowCoherence(steps) {
        if (!steps || steps.length < 2) return 0.5;

        let flowScore = 0;
        for (let i = 1; i < steps.length; i++) {
            const prevStep = steps[i - 1];
            const currentStep = steps[i];
            
            // Check if current step builds on previous
            const builds = currentStep.premises?.some(p => 
                prevStep.conclusions?.some(c => this.sep_textSimilarity(p, c) > 0.7)
            );
            
            flowScore += builds ? 1 : 0;
        }

        return flowScore / (steps.length - 1);
    }

    sep_calculateGoalAlignmentCoherence(steps, goal) {
        if (!steps || !steps.length || !goal) return 0.5;

        // Check final conclusions against goal
        const finalStep = steps[steps.length - 1];
        if (!finalStep.conclusions) return 0.5;

        const alignmentScores = finalStep.conclusions.map(c => 
            this.sep_textSimilarity(c, goal)
        );

        return Math.max(...alignmentScores);
    }

    sep_calculateDepthCoherence(steps) {
        if (!steps || !steps.length) return 0.5;

        // Evaluate depth based on number of premises and conclusions
        const avgComplexity = steps.reduce((sum, step) => {
            const premiseCount = step.premises?.length || 0;
            const conclusionCount = step.conclusions?.length || 0;
            return sum + (premiseCount + conclusionCount);
        }, 0) / (steps.length * 2);

        return Math.min(1.0, avgComplexity / 5); // Normalize to 0-1
    }

    sep_calculateConsistencyCoherence(steps) {
        if (!steps || steps.length < 2) return 0.5;

        // Check for contradictions between steps
        let contradictions = 0;
        for (let i = 0; i < steps.length; i++) {
            for (let j = i + 1; j < steps.length; j++) {
                const contradicts = this.sep_hasContradiction(steps[i], steps[j]);
                if (contradicts) contradictions++;
            }
        }

        const maxPossibleContradictions = (steps.length * (steps.length - 1)) / 2;
        return 1 - (contradictions / maxPossibleContradictions);
    }

    sep_textSimilarity(text1, text2) {
        // Simple text similarity based on word overlap
        // In production, this would use more sophisticated NLP
        const words1 = new Set(text1.toLowerCase().split(/\W+/));
        const words2 = new Set(text2.toLowerCase().split(/\W+/));
        const intersection = new Set([...words1].filter(x => words2.has(x)));
        const union = new Set([...words1, ...words2]);
        return intersection.size / union.size;
    }

    sep_hasContradiction(step1, step2) {
        // Check for direct contradictions in conclusions
        for (const c1 of step1.conclusions || []) {
            for (const c2 of step2.conclusions || []) {
                // Simple negation check
                if (c1.includes('not') && c2 === c1.replace('not', '').trim()) return true;
                if (c2.includes('not') && c1 === c2.replace('not', '').trim()) return true;
            }
        }
        return false;
    }

    async processReasoningChain(result, goal) {
        const coherenceScore = this.calculateReasoningCoherence(result.steps, goal);
        const stabilityScore = Math.min(coherenceScore + 0.1, 1.0); // Slightly higher than coherence

        // Store the reasoning chain with metadata
        const chainId = `reasoning:${Date.now()}`;
        await this.memoryTier.storePattern(chainId, {
            data: {
                goal,
                steps: result.steps,
                conclusions: result.conclusions
            },
            coherence: coherenceScore,
            stability: stabilityScore,
            generations: 1,
            contentType: 'reasoning_chain',
            metadata: {
                stepCount: result.steps.length,
                goalAchieved: coherenceScore > 0.7
            }
        });

        return {
            ...result,
            coherenceScore,
            stabilityScore,
            chainId
        };
    }

    async enhanceReasoningResults(results) {
        // Add coherence metrics to results
        return {
            ...results,
            coherenceMetrics: {
                score: results.coherenceScore || 0.5,
                confidence: results.stabilityScore || 0.5,
                aspects: {
                    stepFlow: this.sep_calculateStepFlowCoherence(results.steps),
                    goalAlignment: this.sep_calculateGoalAlignmentCoherence(results.steps, results.goal),
                    depth: this.sep_calculateDepthCoherence(results.steps),
                    consistency: this.sep_calculateConsistencyCoherence(results.steps)
                }
            }
        };
    }
}

export { SequentialThinkingAdapter };