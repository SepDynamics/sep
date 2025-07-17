#!/usr/bin/env node

import chalk from 'chalk';

const API_BASE = 'http://localhost:3001';

async function testEndpoint(name, endpoint, options = {}) {
    console.log(chalk.blue(`\nTesting ${name}...`));
    
    try {
        const response = await fetch(`${API_BASE}${endpoint}`, options);
        const data = await response.json();
        
        if (response.ok) {
            console.log(chalk.green(`✓ ${name} passed`));
            console.log(chalk.gray('Response:'), JSON.stringify(data, null, 2));
        } else {
            console.log(chalk.red(`✗ ${name} failed`));
            console.log(chalk.red('Error:'), data);
        }
        
        return { success: response.ok, data };
    } catch (error) {
        console.log(chalk.red(`✗ ${name} failed`));
        console.log(chalk.red('Error:'), error.message);
        return { success: false, error };
    }
}

async function runTests() {
    console.log(chalk.bold.blue('\n🧪 SEP-Ollama Integration Tests\n'));
    
    // 1. Health Check
    await testEndpoint('Health Check', '/health');
    
    // Wait a bit for initialization
    await new Promise(resolve => setTimeout(resolve, 2000));
    
    // 2. Simple Completion
    await testEndpoint('Simple Completion', '/api/v1/complete', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            prompt: 'Explain the SEP Engine memory tier system in one sentence.'
        })
    });
    
    // 3. Code-aware Completion
    await testEndpoint('Code-aware Completion', '/api/v1/complete', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            prompt: 'What is the coherence threshold for LTM promotion?'
        })
    });
    
    // 4. Diff Generation
    await testEndpoint('Diff Generation', '/api/v1/generate-diff', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            request: 'Add a comment explaining the coherence calculation in quantum_processor.cpp',
            targetFiles: ['src/quantum/quantum_processor.cpp']
        })
    });
    
    // 5. Pattern Search
    await testEndpoint('Pattern Search', '/api/v1/search-patterns', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            query: 'memory tier transitions',
            limit: 5
        })
    });
    
    // 6. Memory Stats
    await testEndpoint('Memory Statistics', '/api/v1/memory-stats');
    
    // 7. Context Management
    const contextId = 'test-context-' + Date.now();
    await testEndpoint('Save Context', '/api/v1/context', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            action: 'save',
            contextId,
            data: {
                testKey: 'testValue',
                timestamp: new Date().toISOString()
            }
        })
    });
    
    await testEndpoint('Load Context', '/api/v1/context', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            action: 'load',
            contextId
        })
    });
    
    console.log(chalk.green.bold('\n✨ Tests completed!\n'));
}

// Check if server is running
async function checkServer() {
    try {
        const response = await fetch(`${API_BASE}/health`);
        return response.ok;
    } catch (error) {
        return false;
    }
}

async function main() {
    const isRunning = await checkServer();
    
    if (!isRunning) {
        console.log(chalk.red('❌ SEP-Ollama server is not running!'));
        console.log(chalk.yellow('\nPlease start the server first:'));
        console.log(chalk.cyan('  node mcp-tool/start_sep_ollama.js\n'));
        process.exit(1);
    }
    
    await runTests();
}

main().catch(console.error);