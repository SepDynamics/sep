#!/usr/bin/env node

import { exec } from 'child_process';
import { promisify } from 'util';
import ora from 'ora';
import chalk from 'chalk';

const execAsync = promisify(exec);

const MODEL_NAME = 'llama3:70b';
const REQUIRED_RAM_GB = 80; // 70B model needs significant RAM

async function checkSystemRequirements() {
    const spinner = ora('Checking system requirements...').start();
    
    try {
        // Check available memory
        const { stdout: memInfo } = await execAsync("free -g | grep Mem | awk '{print $2}'");
        const totalRAM = parseInt(memInfo.trim());
        
        if (totalRAM < REQUIRED_RAM_GB) {
            spinner.warn(chalk.yellow(`Warning: System has ${totalRAM}GB RAM, but ${REQUIRED_RAM_GB}GB is recommended for ${MODEL_NAME}`));
        } else {
            spinner.succeed(chalk.green(`System RAM: ${totalRAM}GB ✓`));
        }
        
        // Check if Ollama is installed
        try {
            await execAsync('ollama --version');
            spinner.succeed(chalk.green('Ollama is installed ✓'));
        } catch (error) {
            spinner.fail(chalk.red('Ollama is not installed'));
            console.log(chalk.yellow('\nPlease install Ollama:'));
            console.log(chalk.cyan('curl -fsSL https://ollama.ai/install.sh | sh'));
            process.exit(1);
        }
        
        // Check if Ollama service is running
        try {
            await execAsync('curl -s http://localhost:11434/api/tags > /dev/null');
            spinner.succeed(chalk.green('Ollama service is running ✓'));
        } catch (error) {
            spinner.warn(chalk.yellow('Ollama service is not running'));
            console.log(chalk.yellow('\nStarting Ollama service...'));
            await execAsync('ollama serve &');
            await new Promise(resolve => setTimeout(resolve, 3000)); // Wait for service to start
        }
        
    } catch (error) {
        spinner.fail(chalk.red('Failed to check system requirements'));
        console.error(error);
        process.exit(1);
    }
}

async function checkModel() {
    const spinner = ora(`Checking for ${MODEL_NAME} model...`).start();
    
    try {
        const { stdout } = await execAsync('ollama list');
        const models = stdout.trim().split('\n');
        const hasModel = models.some(line => line.includes(MODEL_NAME));
        
        if (hasModel) {
            spinner.succeed(chalk.green(`Model ${MODEL_NAME} is already installed ✓`));
            return true;
        } else {
            spinner.warn(chalk.yellow(`Model ${MODEL_NAME} not found`));
            return false;
        }
    } catch (error) {
        spinner.fail(chalk.red('Failed to check models'));
        console.error(error);
        return false;
    }
}

async function pullModel() {
    console.log(chalk.cyan(`\nPulling ${MODEL_NAME} model...`));
    console.log(chalk.yellow('This will download ~39GB and may take a while...'));
    
    try {
        // Use spawn for real-time output
        const { spawn } = await import('child_process');
        const pullProcess = spawn('ollama', ['pull', MODEL_NAME]);
        
        pullProcess.stdout.on('data', (data) => {
            process.stdout.write(data.toString());
        });
        
        pullProcess.stderr.on('data', (data) => {
            process.stderr.write(data.toString());
        });
        
        return new Promise((resolve, reject) => {
            pullProcess.on('close', (code) => {
                if (code === 0) {
                    console.log(chalk.green(`\n✓ Successfully pulled ${MODEL_NAME}`));
                    resolve();
                } else {
                    reject(new Error(`Process exited with code ${code}`));
                }
            });
        });
    } catch (error) {
        console.error(chalk.red('Failed to pull model:'), error);
        process.exit(1);
    }
}

async function testModel() {
    const spinner = ora('Testing model...').start();
    
    try {
        const testPrompt = 'Respond with exactly: "SEP-Ollama ready"';
        const { stdout } = await execAsync(`ollama run ${MODEL_NAME} "${testPrompt}"`);
        
        if (stdout.includes('SEP-Ollama ready')) {
            spinner.succeed(chalk.green('Model test passed ✓'));
        } else {
            spinner.warn(chalk.yellow('Model test produced unexpected output'));
            console.log('Output:', stdout);
        }
    } catch (error) {
        spinner.fail(chalk.red('Model test failed'));
        console.error(error);
    }
}

async function configureOllama() {
    const spinner = ora('Configuring Ollama for SEP...').start();
    
    try {
        // Create custom modelfile for SEP-Ollama
        const modelfileContent = `FROM ${MODEL_NAME}

# Set system parameters
PARAMETER temperature 0.7
PARAMETER top_p 0.95
PARAMETER repeat_penalty 1.1
PARAMETER seed 42

# Set context window
PARAMETER num_ctx 8192

# SEP-specific system prompt
SYSTEM """You are SEP-Ollama, a specialized AI agent for the SEP Engine quantum pattern processing system. 
You have deep knowledge of the codebase including:
- Quantum algorithms (QBSA, QFH)
- Memory tier architecture (STM, MTM, LTM)
- Pattern coherence and stability metrics
- C++ and JavaScript implementation details

When generating code or diffs, be precise and follow the existing code style."""
`;

        await execAsync(`echo '${modelfileContent}' > /tmp/sep-ollama.modelfile`);
        
        // Create custom model
        spinner.text = 'Creating custom SEP-Ollama model...';
        await execAsync('ollama create sep-ollama -f /tmp/sep-ollama.modelfile');
        
        spinner.succeed(chalk.green('Created custom sep-ollama model ✓'));
        
        // Update the inference.js to use custom model
        console.log(chalk.yellow('\nUpdate your code to use "sep-ollama" instead of "llama3:70b"'));
        
    } catch (error) {
        spinner.fail(chalk.red('Failed to configure Ollama'));
        console.error(error);
    }
}

async function main() {
    console.log(chalk.blue.bold('\n🚀 SEP-Ollama Setup\n'));
    
    await checkSystemRequirements();
    
    const hasModel = await checkModel();
    
    if (!hasModel) {
        const readline = await import('readline');
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });
        
        const answer = await new Promise(resolve => {
            rl.question(chalk.yellow(`\nDo you want to download ${MODEL_NAME}? (y/n): `), resolve);
        });
        rl.close();
        
        if (answer.toLowerCase() === 'y') {
            await pullModel();
        } else {
            console.log(chalk.red('Model download cancelled'));
            process.exit(0);
        }
    }
    
    await testModel();
    await configureOllama();
    
    console.log(chalk.green.bold('\n✨ SEP-Ollama setup complete!\n'));
    console.log(chalk.cyan('You can now run the SEP-Ollama server:'));
    console.log(chalk.white('  node mcp-tool/sep_ollama_server.js\n'));
}

// Run setup
main().catch(console.error);