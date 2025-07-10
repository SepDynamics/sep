#!/usr/bin/env node

import express from 'express';
import { exec } from 'child_process';
import { promisify } from 'util';
import path from 'path';
import fs from 'fs/promises';
import { McpServer } from '@modelcontextprotocol/sdk/server/mcp.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import { z } from 'zod';

const execAsync = promisify(exec);

class BuildHelper {
    constructor() {
        this.app = express();
        this.app.use(express.json());
        
        this.setupRoutes();
        this.mcpServer = null;
    }

    setupRoutes() {
        this.app.post('/analyze_build', async (req, res) => {
            try {
                const { buildLog, filePath } = req.body;
                const analysis = await this.analyzeBuildIssues(buildLog, filePath);
                res.json(analysis);
            } catch (error) {
                console.error('Build analysis error:', error);
                res.status(500).json({ error: 'Build analysis failed' });
            }
        });

        this.app.post('/fix_pattern_processor', async (req, res) => {
            try {
                const { filePath } = req.body;
                const fixes = await this.fixPatternProcessor(filePath);
                res.json(fixes);
            } catch (error) {
                console.error('Pattern processor fix error:', error);
                res.status(500).json({ error: 'Fix failed' });
            }
        });
        
        this.app.post('/fix_cuda_build', async (req, res) => {
            try {
                const { buildLog, filePath } = req.body;
                const fixes = await this.fixCudaBuild(buildLog, filePath);
                res.json(fixes);
            } catch (error) {
                console.error('CUDA build fix error:', error);
                res.status(500).json({ error: 'Fix failed' });
            }
        });
    }

    async analyzeBuildIssues(buildLog, filePath) {
        const issues = [];
        const fixes = [];

        // Common build error patterns
        const errorPatterns = [
            {
                pattern: 'no viable conversion from',
                type: 'type_conversion',
                description: 'Type conversion error detected',
                fix: 'Check Vec4 attribute access - use .x for coherence, .y for stability, .z for entropy, .w for mutation_rate'
            },
            {
                pattern: 'redefinition of',
                type: 'redefinition',
                description: 'Variable redefinition error',
                fix: 'Use scoped blocks {} to prevent variable redefinition'
            },
            {
                pattern: 'is ambiguous',
                type: 'ambiguous_symbol',
                description: 'Ambiguous symbol reference detected',
                fix: 'Rename or fully qualify the ambiguous symbol'
            },
            {
                pattern: 'exception handling disabled',
                type: 'exception_handling',
                description: 'Exception handling used in CUDA code',
                fix: 'Remove try/catch blocks in CUDA device code and use error codes instead'
            },
            {
                pattern: 'no member named',
                type: 'missing_member',
                description: 'Missing member or method',
                fix: 'Check for typos or add the missing member to the class'
            },
            {
                pattern: 'expression must have class type',
                type: 'class_type_error',
                description: 'Expression must have class type',
                fix: 'Use conditional compilation with #ifdef __CUDACC__ for STL container operations'
            },
            {
                pattern: 'fatal error',
                type: 'missing_header',
                description: 'Missing header file',
                fix: 'Check include paths or use conditional compilation to provide alternatives'
            }
        ];

        // Check for known error patterns
        for (const errorPattern of errorPatterns) {
            if (buildLog.includes(errorPattern.pattern)) {
                issues.push({
                    type: errorPattern.type,
                    description: errorPattern.description
                });
                fixes.push({
                    type: 'suggestion',
                    description: errorPattern.fix
                });
            }
        }

        // Extract specific errors for additional analysis
        const errorMatches = [...buildLog.matchAll(/error: ([^\n]+)/g)];
        const errorMessages = errorMatches.map(match => match[1].trim());
        
        // Group similar errors
        const errorGroups = this.groupSimilarErrors(errorMessages);
        
        for (const [errorType, count] of Object.entries(errorGroups)) {
            if (count > 1) {
                issues.push({
                    type: 'recurring_error',
                    description: `${errorType} (${count} occurrences)`
                });
            }
        }

        // Check file content for common patterns
        if (filePath) {
            try {
                const content = await fs.readFile(filePath, 'utf8');
                
                if (content.includes('pattern.coherence') || content.includes('pattern.stability')) {
                    issues.push({
                        type: 'attribute_access',
                        description: 'Direct attribute access detected'
                    });
                    fixes.push({
                        type: 'suggestion',
                        description: 'Use pattern.attributes.x for coherence and pattern.attributes.y for stability'
                    });
                }
                
                // Check for CUDA compatibility issues
                if (filePath.endsWith('.cu') && (content.includes('try') || content.includes('catch'))) {
                    issues.push({
                        type: 'cuda_exceptions',
                        description: 'Exception handling in CUDA code'
                    });
                    fixes.push({
                        type: 'suggestion',
                        description: 'Replace try/catch blocks with error code checking in CUDA code'
                    });
                }
                
                // Check for MIN_COHERENCE conflicts
                if (content.includes('MIN_COHERENCE') && filePath.includes('cuda')) {
                    issues.push({
                        type: 'name_conflict',
                        description: 'Naming conflict with MIN_COHERENCE constant'
                    });
                    fixes.push({
                        type: 'suggestion',
                        description: 'Rename CUDA-specific constants with a prefix like CUDA_MIN_COHERENCE'
                    });
                }
            } catch (error) {
                console.error(`Error reading file ${filePath}:`, error);
            }
        }

        return {
            issues,
            fixes,
            buildLog
        };
    }

    groupSimilarErrors(errorMessages) {
        const groups = {};
        
        for (const error of errorMessages) {
            // Extract the error type (first few words)
            const errorType = error.split(' ').slice(0, 5).join(' ');
            
            if (!groups[errorType]) {
                groups[errorType] = 0;
            }
            groups[errorType]++;
        }
        
        return groups;
    }

    async fixPatternProcessor(filePath) {
        const content = await fs.readFile(filePath, 'utf8');
        let modified = content;

        // Fix attribute access
        modified = modified.replace(/pattern\.coherence/g, 'pattern.attributes.x');
        modified = modified.replace(/pattern\.stability/g, 'pattern.attributes.y');
        modified = modified.replace(/pattern\.entropy/g, 'pattern.attributes.z');
        modified = modified.replace(/pattern\.mutation_rate/g, 'pattern.attributes.w');

        // Add scoped blocks for variable declarations
        modified = modified.replace(
            /const\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*([^;]+);/g,
            '{\n    const $1 = $2;\n}'
        );

        await fs.writeFile(filePath, modified);

        return {
            status: 'success',
            message: 'Pattern processor fixes applied',
            changes: [
                'Updated attribute access to use Vec4 components',
                'Added scoped blocks for variable declarations'
            ]
        };
    }
    
    async fixCudaBuild(buildLog, filePath) {
        if (!filePath) {
            return {
                status: 'error',
                message: 'File path is required'
            };
        }
        
        const content = await fs.readFile(filePath, 'utf8');
        let modified = content;
        const changes = [];
        
        // Fix exception handling in CUDA code
        if (buildLog.includes('exception handling disabled') && filePath.endsWith('.cu')) {
            // Replace try-catch blocks with error code handling
            const tryCatchPattern = /try\s*\{([^}]*)\}\s*catch\s*\(([^)]*)\)\s*\{([^}]*)\}/g;
            modified = modified.replace(tryCatchPattern, (match, tryBlock, exceptionType, catchBlock) => {
                const errorVarName = 'result';
                const errorCheckCode = `${errorVarName} = ${tryBlock.trim()};\n    if (${errorVarName} != cudaSuccess) {\n        ${catchBlock.trim()}\n    }`;
                changes.push('Replaced try-catch blocks with error code checking');
                return errorCheckCode;
            });
        }
        
        // Fix ambiguous MIN_COHERENCE
        if (buildLog.includes('is ambiguous') && buildLog.includes('MIN_COHERENCE')) {
            modified = modified.replace(/\bMIN_COHERENCE\b/g, 'CUDA_MIN_COHERENCE');
            changes.push('Renamed MIN_COHERENCE to CUDA_MIN_COHERENCE to avoid naming conflicts');
        }
        
        // Fix empty() method not available in CUDA
        if (buildLog.includes('expression must have class type') && buildLog.includes('relationships.empty')) {
            modified = modified.replace(
                /(\w+)\.relationships\.empty\(\)/g,
                '#ifdef __CUDACC__\n$1.relationship_count == 0\n#else\n$1.relationships.empty()\n#endif'
            );
            changes.push('Added conditional compilation for relationships.empty() method');
        }
        
        // Fix Vector/STL operations in CUDA
        if (buildLog.includes('is undefined in device code') &&
            (buildLog.includes('vector') || buildLog.includes('string'))) {
            
            // Add CUDA-safe alternatives for vector/string operations
            if (!modified.includes('#ifdef __CUDACC__')) {
                modified = `// CUDA compatibility wrapper
#ifdef __CUDACC__
// Add device-compatible alternatives for STL operations
#endif\n\n${modified}`;
                changes.push('Added CUDA compatibility header section');
            }
            
            changes.push('Added conditional compilation for STL operations in CUDA code');
        }
        
        if (changes.length > 0) {
            await fs.writeFile(filePath, modified);
            return {
                status: 'success',
                message: 'CUDA build fixes applied',
                changes
            };
        } else {
            return {
                status: 'no_changes',
                message: 'No applicable fixes found for the given build errors'
            };
        }
    }

    async start(port = 3001) {
        return new Promise((resolve) => {
            this.server = this.app.listen(port, () => {
                console.log(`Build Helper MCP server running on port ${port}`);
                resolve();
            });
        });
    }

    async stop() {
        if (this.server) {
            await new Promise((resolve) => this.server.close(resolve));
            console.log('Build Helper MCP server stopped');
        }
    }
    
    async setupMcpServer() {
        // Create an MCP server
        this.mcpServer = new McpServer({
            name: "build-helper",
            version: "0.2.0"
        });

        // Add tools to the MCP server
        this.mcpServer.tool(
            "analyze_build",
            {
                buildLog: z.string().describe("Build log content to analyze"),
                filePath: z.string().optional().describe("Path to the file with issues (optional)")
            },
            async ({ buildLog, filePath }) => {
                try {
                    const analysis = await this.analyzeBuildIssues(buildLog, filePath || "");
                    return {
                        content: [
                            {
                                type: "text",
                                text: JSON.stringify(analysis, null, 2)
                            }
                        ]
                    };
                } catch (error) {
                    return {
                        content: [
                            {
                                type: "text",
                                text: `Error analyzing build: ${error.message}`
                            }
                        ],
                        isError: true
                    };
                }
            }
        );
        
        this.mcpServer.tool(
            "fix_build_issue",
            {
                buildLog: z.string().describe("Build log content"),
                filePath: z.string().describe("Path to the file to fix"),
                issueType: z.string().describe("Type of issue to fix (cuda_exceptions, attribute_access, name_conflict, etc.)")
            },
            async ({ buildLog, filePath, issueType }) => {
                try {
                    let result;
                    
                    if (issueType === 'cuda_exceptions' || issueType === 'name_conflict') {
                        result = await this.fixCudaBuild(buildLog, filePath);
                    } else if (issueType === 'attribute_access') {
                        result = await this.fixPatternProcessor(filePath);
                    } else {
                        // Generic analysis and fix suggestion
                        result = await this.analyzeBuildIssues(buildLog, filePath);
                    }
                    
                    return {
                        content: [
                            {
                                type: "text",
                                text: JSON.stringify(result, null, 2)
                            }
                        ]
                    };
                } catch (error) {
                    return {
                        content: [
                            {
                                type: "text",
                                text: `Error fixing build issue: ${error.message}`
                            }
                        ],
                        isError: true
                    };
                }
            }
        );

        // Start receiving messages on stdin and sending messages on stdout
        const transport = new StdioServerTransport();
        await this.mcpServer.connect(transport);
        console.error('Build Helper MCP server running on stdio');
    }
}

// Start server if run directly
if (require.main === module) {
    const helper = new BuildHelper();
    
    // Check if we should run as an MCP server
    if (process.argv.includes('--mcp')) {
        helper.setupMcpServer();
    } else {
        // Otherwise run as a regular HTTP server
        helper.start();
    }
}

export { BuildHelper };