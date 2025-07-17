#!/usr/bin/env node

import { spawn, exec } from 'child_process';
import { promises as fs } from 'fs';
import path from 'path';
import chalk from 'chalk';
import { promisify } from 'util';

const execAsync = promisify(exec);
import net from 'net';

// Check if a port is available
async function isPortAvailable(port) {
    return new Promise((resolve) => {
        const server = net.createServer();
        server.once('error', () => resolve(false));
        server.once('listening', () => {
            server.close();
            resolve(true);
        });
        server.listen(port);
    });
}

// Kill process using a specific port
async function killPortProcess(port) {
    try {
        // Try to find and kill the process using the port
        const { stdout } = await execAsync(`lsof -ti:${port}`);
        const pid = stdout.trim();
        if (pid) {
            await execAsync(`kill -9 ${pid}`);
            console.log(chalk.yellow(`⚠ Killed process ${pid} using port ${port}`));
            // Wait a bit for the port to be freed
            await new Promise(resolve => setTimeout(resolve, 1000));
        }
    } catch (error) {
        // No process found on the port, which is fine
    }
}

// Check dependencies and environment
async function checkEnvironment() {
    console.log(chalk.blue.bold('🔍 Checking SEP-Ollama environment...\n'));
    
    const issues = [];
    
    // 1. Check Ollama installation
    try {
        await execAsync('ollama --version');
        console.log(chalk.green('✓ Ollama is installed'));
    } catch (error) {
        issues.push('Ollama is not installed. Run: curl -fsSL https://ollama.ai/install.sh | sh');
    }
    
    // 2. Check if Ollama service is running
    try {
        const { stdout } = await execAsync('curl -s http://localhost:11434/api/tags');
        console.log(chalk.green('✓ Ollama service is running'));
    } catch (error) {
        console.log(chalk.yellow('⚠ Ollama service not running, attempting to start...'));
        spawn('ollama', ['serve'], { detached: true, stdio: 'ignore' }).unref();
        await new Promise(resolve => setTimeout(resolve, 3000));
    }
    
    // 3. Check Redis/Valkey
    try {
        await execAsync('redis-cli ping');
        console.log(chalk.green('✓ Redis/Valkey is running'));
    } catch (error) {
        issues.push('Redis/Valkey is not running. Start it with: systemctl start redis');
    }
    
    // 4. Check model availability
    try {
        const { stdout } = await execAsync('ollama list');
        if (stdout.includes('llama3:70b') || stdout.includes('sep-ollama')) {
            console.log(chalk.green('✓ LLM model is available'));
        } else {
            console.log(chalk.yellow('⚠ Model not found. Run: node mcp-tool/sep_ollama_setup.js'));
        }
    } catch (error) {
        issues.push('Cannot check Ollama models');
    }
    
    // 5. Check Node modules
    try {
        await fs.access('mcp-tool/node_modules');
        console.log(chalk.green('✓ Node modules installed'));
    } catch (error) {
        console.log(chalk.yellow('⚠ Node modules not found, installing...'));
        await execAsync('cd mcp-tool && npm install');
    }
    
    if (issues.length > 0) {
        console.log(chalk.red('\n❌ Environment issues found:'));
        issues.forEach(issue => console.log(chalk.red(`  • ${issue}`)));
        process.exit(1);
    }
    
    console.log(chalk.green('\n✓ All checks passed!\n'));
}

// Start the SEP-Ollama services
async function startServices() {
    console.log(chalk.blue.bold('🚀 Starting SEP-Ollama services...\n'));
    
    const services = [
        {
            name: 'SEP-Ollama Server',
            script: 'sep_ollama_server.js',
            port: 3001,
            color: chalk.cyan
        },
        {
            name: 'SEP-Ollama Monitor',
            script: 'sep_ollama_monitor.js',
            port: 3002,
            color: chalk.magenta
        }
    ];
    
    const processes = [];
    
    for (const service of services) {
        console.log(service.color(`Starting ${service.name}...`));
        
        // Check if port is available
        const portAvailable = await isPortAvailable(service.port);
        if (!portAvailable) {
            console.log(chalk.yellow(`⚠ Port ${service.port} is already in use`));
            await killPortProcess(service.port);
        }
        
        const proc = spawn('node', [`mcp-tool/${service.script}`], {
            cwd: '/sep',
            env: { ...process.env, PORT: service.port }
        });
        
        proc.stdout.on('data', (data) => {
            console.log(service.color(`[${service.name}] ${data.toString().trim()}`));
        });
        
        proc.stderr.on('data', (data) => {
            console.error(service.color(`[${service.name} ERROR] ${data.toString().trim()}`));
        });
        
        proc.on('error', (error) => {
            console.error(service.color(`[${service.name} ERROR] Failed to start: ${error.message}`));
        });
        
        processes.push(proc);
        
        // Wait a bit for service to start
        await new Promise(resolve => setTimeout(resolve, 2000));
    }
    
    console.log(chalk.green.bold('\n✨ SEP-Ollama is running!\n'));
    console.log(chalk.white('Services:'));
    console.log(chalk.cyan('  • API Server: http://localhost:3001'));
    console.log(chalk.magenta('  • Monitor: http://localhost:3002'));
    console.log(chalk.gray('\nPress Ctrl+C to stop all services\n'));
    
    // Test the API
    setTimeout(async () => {
        console.log(chalk.blue('\n🧪 Testing API endpoint...'));
        try {
            const response = await fetch('http://localhost:3001/health');
            const data = await response.json();
            console.log(chalk.green('✓ API Health Check:'), data);
            
            // Test a simple completion
            console.log(chalk.blue('\n🧪 Testing completion...'));
            const completionResponse = await fetch('http://localhost:3001/api/v1/complete', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ 
                    prompt: 'What is the SEP Engine?',
                    temperature: 0.7 
                })
            });
            
            if (completionResponse.ok) {
                const result = await completionResponse.json();
                console.log(chalk.green('✓ Completion test successful'));
                console.log(chalk.gray('Response preview:'), result.completion.substring(0, 100) + '...');
            }
        } catch (error) {
            console.error(chalk.red('API test failed:'), error.message);
        }
    }, 5000);
    
    // Handle shutdown
    process.on('SIGINT', () => {
        console.log(chalk.yellow('\n\nShutting down SEP-Ollama...'));
        processes.forEach(proc => proc.kill());
        process.exit(0);
    });
    
    // Keep the main process alive
    process.stdin.resume();
}

// Main execution
async function main() {
    try {
        await checkEnvironment();
        await startServices();
    } catch (error) {
        console.error(chalk.red('Fatal error:'), error);
        process.exit(1);
    }
}

// Run if called directly
if (import.meta.url === `file://${process.argv[1]}`) {
    main().catch(console.error);
}

export { checkEnvironment, startServices };