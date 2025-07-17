#!/usr/bin/env node

import { spawn, exec } from 'child_process';
import { promises as fs } from 'fs';
import path from 'path';
import chalk from 'chalk';
import { promisify } from 'util';

const execAsync = promisify(exec);
import net from 'net';

// Check if a port is available with retries
async function isPortAvailable(port, retries = 3, delay = 1000) {
    for (let attempt = 1; attempt <= retries; attempt++) {
        try {
            const available = await new Promise((resolve, reject) => {
                const server = net.createServer();
                server.once('error', (err) => {
                    if (err.code === 'EADDRINUSE') {
                        resolve(false);
                    } else {
                        reject(err);
                    }
                });
                server.once('listening', () => {
                    server.close();
                    resolve(true);
                });
                server.listen(port);
            });
            
            if (available) return true;
            
            if (attempt < retries) {
                console.log(chalk.yellow(`Port ${port} not available, attempt ${attempt}/${retries}. Retrying...`));
                await new Promise(resolve => setTimeout(resolve, delay));
            }
        } catch (error) {
            console.error(chalk.red(`Error checking port ${port}:`, error.message));
            if (attempt < retries) {
                await new Promise(resolve => setTimeout(resolve, delay));
            }
        }
    }
    return false;
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

// Verify service health
async function verifyServiceHealth(name, port, maxAttempts = 5, interval = 1000) {
    const endpoint = `http://localhost:${port}/health`;
    
    for (let attempt = 1; attempt <= maxAttempts; attempt++) {
        try {
            const response = await fetch(endpoint);
            if (response.ok) {
                console.log(chalk.green(`✓ ${name} health check passed`));
                return true;
            }
        } catch (error) {
            if (attempt === maxAttempts) {
                console.error(chalk.red(`${name} health check failed after ${maxAttempts} attempts`));
                return false;
            }
            await new Promise(resolve => setTimeout(resolve, interval));
        }
    }
    return false;
}

// Find next available port
async function findAvailablePort(startPort, maxAttempts = 10) {
    for (let port = startPort; port < startPort + maxAttempts; port++) {
        if (await isPortAvailable(port, 2)) {
            return port;
        }
    }
    return null;
}

// Start the SEP-Ollama services
async function startServices() {
    console.log(chalk.blue.bold('🚀 Starting SEP-Ollama services...\n'));
    
    const processes = new Map();
    const cleanupTimeouts = new Map();
    
    const services = [
        {
            name: 'SEP-Ollama Server',
            script: 'sep_ollama_server.js',
            port: 3001,
            color: chalk.cyan,
            required: true
        },
        {
            name: 'SEP-Ollama Monitor',
            script: 'sep_ollama_monitor.js',
            port: 3002,
            color: chalk.magenta,
            required: false
        }
    ];
    
    async function startService(service) {
        console.log(service.color(`Starting ${service.name}...`));
        
        let port = service.port;
        let portAvailable = await isPortAvailable(port, 3);
        
        if (!portAvailable) {
            if (service.required) {
                console.log(chalk.yellow(`⚠ Port ${port} is in use, attempting to free...`));
                await killPortProcess(port);
                portAvailable = await isPortAvailable(port, 2);
                
                if (!portAvailable) {
                    throw new Error(`Could not free required port ${port} for ${service.name}`);
                }
            } else {
                console.log(chalk.yellow(`⚠ Finding alternative port for ${service.name}...`));
                port = await findAvailablePort(port);
                
                if (!port) {
                    console.log(chalk.yellow(`⚠ Skipping ${service.name} - no available ports found`));
                    return null;
                }
                console.log(chalk.yellow(`⚠ Using alternative port ${port} for ${service.name}`));
            }
        }
        
        const proc = spawn('node', [`mcp-tool/${service.script}`], {
            cwd: '/sep',
            env: { ...process.env, PORT: port }
        });
        
        proc.stdout.on('data', (data) => {
            console.log(service.color(`[${service.name}] ${data.toString().trim()}`));
        });
        
        proc.stderr.on('data', (data) => {
            console.error(service.color(`[${service.name} ERROR] ${data.toString().trim()}`));
        });
        
        proc.on('error', (error) => {
            console.error(service.color(`[${service.name} ERROR] Failed to start: ${error.message}`));
            if (service.required) {
                process.exit(1);
            }
        });
        
        proc.on('exit', (code, signal) => {
            console.log(service.color(`[${service.name}] Process exited with code ${code}, signal ${signal}`));
            processes.delete(service.name);
            
            if (service.required && code !== 0) {
                console.error(chalk.red(`Required service ${service.name} failed, shutting down...`));
                shutdownServices();
            }
        });
        
        processes.set(service.name, { proc, port });
        
        // Wait briefly for service to start
        await new Promise(resolve => setTimeout(resolve, 2000));
        
        // Verify service health
        if (!(await verifyServiceHealth(service.name, port))) {
            if (service.required) {
                throw new Error(`${service.name} failed health check`);
            } else {
                console.log(chalk.yellow(`⚠ Warning: ${service.name} health check failed, but continuing as it's optional`));
            }
        }
        
        return port;
    }
    
    try {
        for (const service of services) {
            const port = await startService(service);
            if (port) {
                service.actualPort = port;
            }
        }
    } catch (error) {
        console.error(chalk.red('Error starting services:'), error.message);
        await shutdownServices();
        process.exit(1);
    }
    
    console.log(chalk.green.bold('\n✨ SEP-Ollama is running!\n'));
    console.log(chalk.white('Services:'));
    console.log(chalk.cyan('  • API Server: http://localhost:3001'));
    
    const monitorService = services.find(s => s.name === 'SEP-Ollama Monitor');
    if (monitorService && monitorService.actualPort) {
        console.log(chalk.magenta(`  • Monitor: http://localhost:${monitorService.actualPort}`));
    } else {
        console.log(chalk.yellow('  • Monitor: Not running'));
    }
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
    
    // Handle graceful shutdown
    async function shutdownServices() {
        console.log(chalk.yellow('\n\nShutting down SEP-Ollama services...'));
        
        for (const [name, { proc }] of processes.entries()) {
            if (proc.killed) continue;
            
            console.log(chalk.yellow(`Stopping ${name}...`));
            
            // Set a cleanup timeout
            const cleanupTimeout = setTimeout(() => {
                console.log(chalk.red(`Force killing ${name} after timeout`));
                proc.kill('SIGKILL');
            }, 5000);
            
            cleanupTimeouts.set(name, cleanupTimeout);
            
            // Try graceful shutdown first
            proc.kill('SIGTERM');
        }
        
        // Wait for all processes to exit or be killed
        await Promise.all([...processes.values()].map(
            ({ proc }) => new Promise(resolve => proc.on('exit', resolve))
        ));
        
        // Clear all timeouts
        for (const timeout of cleanupTimeouts.values()) {
            clearTimeout(timeout);
        }
        
        console.log(chalk.green('All services stopped'));
    }
    
    // Handle shutdown signals
    process.on('SIGINT', shutdownServices);
    process.on('SIGTERM', shutdownServices);
    
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