/**
 * Obsidian MCP Adapter
 * Handles integration between MCP-Obsidian server and SEP Memory Tier system
 */

import { BaseMCPAdapter } from './sep_mcp_controller.js';

class ObsidianAdapter extends BaseMCPAdapter {
    constructor(obsidianServer, memoryTier) {
        super(obsidianServer, memoryTier);
        
        // Initialize Obsidian-specific settings
        this.settings = {
            coherenceWeights: {
                frontmatter: 0.15,    // YAML frontmatter adds structure
                wikilinks: 0.15,      // Internal links show relationships
                tags: 0.1,            // Tags add categorization
                headers: 0.1,         // Headers show organization
                codeBlocks: 0.1,      // Code blocks add technical content
                lists: 0.1,           // Lists show structured content
                baseContent: 0.3      // Base weight for content relevance
            },
            minCoherence: 0.1,
            defaultStability: 0.5
        };
    }

    /**
     * Process MCP-Obsidian tool requests
     */
    async processRequest(toolName, args) {
        try {
            // Execute original tool request
            const result = await super.processRequest(toolName, args);

            // Handle specific tools
            switch (toolName) {
                case 'search_vault':
                    return await this.enhanceSearchResults(result, args.query);
                case 'get_note':
                    return await this.processNote(result, args.path);
                default:
                    return result;
            }
        } catch (error) {
            console.error(`Error in ObsidianAdapter.processRequest:`, error);
            throw error;
        }
    }

    /**
     * Calculate coherence score for Markdown content
     * Enhanced version of the base Markdown handler that considers Obsidian-specific features
     */
    calculateMarkdownCoherence(content, query = '') {
        try {
            const {
                frontmatter,
                wikilinks,
                tags,
                headers,
                codeBlocks,
                lists,
                baseContent
            } = this.settings.coherenceWeights;

            let score = 0;

            // Check for YAML frontmatter
            if (content.match(/^---\n[\s\S]*?\n---/)) {
                score += frontmatter;
            }

            // Count wiki-style internal links
            const wikilinkCount = (content.match(/\[\[.*?\]\]/g) || []).length;
            score += Math.min(wikilinks * (wikilinkCount / 5), wikilinks);

            // Count tags
            const tagCount = (content.match(/#[a-zA-Z0-9_-]+/g) || []).length;
            score += Math.min(tags * (tagCount / 3), tags);

            // Check headers (weighted by level)
            const headerMatches = content.match(/^#{1,6}\s.+$/gm) || [];
            const headerScore = headerMatches.reduce((acc, header) => {
                const level = header.match(/^#{1,6}/)[0].length;
                return acc + (1 / level);
            }, 0);
            score += Math.min(headers * (headerScore / 3), headers);

            // Check code blocks
            const codeBlockCount = (content.match(/```[\s\S]*?```/g) || []).length;
            score += Math.min(codeBlocks * (codeBlockCount / 2), codeBlocks);

            // Check lists
            const listItemCount = (content.match(/^[\s-]*[-*+]\s/gm) || []).length;
            score += Math.min(lists * (listItemCount / 5), lists);

            // Calculate base content coherence
            let contentScore = baseContent;
            if (query) {
                // If there's a query, adjust base score by relevance
                const queryTerms = query.toLowerCase().split(/\s+/);
                const contentLower = content.toLowerCase();
                const matchCount = queryTerms.filter(term => contentLower.includes(term)).length;
                contentScore *= (matchCount / queryTerms.length);
            }
            score += contentScore;

            return Math.max(this.settings.minCoherence, Math.min(score, 1.0));
        } catch (error) {
            console.error('Error calculating Markdown coherence:', error);
            return this.settings.minCoherence;
        }
    }

    /**
     * Store a note in the memory tier system
     */
    async storeNote(note, query = '') {
        try {
            const coherence = this.calculateMarkdownCoherence(note.content, query);
            const patternId = `obsidian:note:${note.path}`;

            await this.memoryTier.storePattern(patternId, {
                data: {
                    path: note.path,
                    content: note.content,
                    metadata: note.metadata || {}
                },
                contentType: 'markdown',
                coherence,
                stability: this.settings.defaultStability,
                generations: 1,
                mutation_rate: 0.0
            });

            return patternId;
        } catch (error) {
            console.error('Error storing note:', error);
            throw error;
        }
    }

    /**
     * Process and enhance search results with coherence scores
     */
    async enhanceSearchResults(results, query) {
        try {
            const enhancedResults = {
                ...results,
                notes: await Promise.all(results.notes.map(async (note) => {
                    // Calculate coherence score
                    const coherence = this.calculateMarkdownCoherence(note.content, query);

                    // Store note in memory tier
                    const patternId = await this.storeNote(note, query);

                    // Enhance note with coherence info
                    return {
                        ...note,
                        coherence,
                        patternId
                    };
                }))
            };

            // Sort by coherence score
            enhancedResults.notes.sort((a, b) => b.coherence - a.coherence);

            return enhancedResults;
        } catch (error) {
            console.error('Error enhancing search results:', error);
            throw error;
        }
    }

    /**
     * Process a single note retrieval
     */
    async processNote(note, path) {
        try {
            if (!note) return null;

            // Calculate coherence score
            const coherence = this.calculateMarkdownCoherence(note.content);

            // Store in memory tier
            const patternId = await this.storeNote(note);

            // Return enhanced note
            return {
                ...note,
                coherence,
                patternId
            };
        } catch (error) {
            console.error(`Error processing note ${path}:`, error);
            throw error;
        }
    }
}

export default ObsidianAdapter;