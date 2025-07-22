// main.js - Market Signal Generation and Live Trading Orchestrator
// Orchestrates the entire cycle of signal generation, validation, and live trading

const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');
require('dotenv').config();

// Check environment variables
if (!process.env.OANDA_API_KEY || !process.env.OANDA_ACCOUNT_ID) {
  console.error('ERROR: Missing OANDA API credentials. Check your .env file.');
  console.error('Required variables: OANDA_API_KEY, OANDA_ACCOUNT_ID');
  process.exit(1);
}

// Risk management validation
const maxLoss = parseFloat(process.env.OANDA_MAX_LOSS_USD || '100');
const riskPerTrade = parseFloat(process.env.OANDA_RISK_PER_TRADE || '2');
const stopLossPips = parseFloat(process.env.OANDA_STOP_LOSS_PIPS || '5');

// Verify risk parameters
console.log('--- RISK PARAMETERS ---');
console.log(`Maximum loss: $${maxLoss}`);
console.log(`Risk per trade: ${riskPerTrade}%`);
console.log(`Stop loss: ${stopLossPips} pips`);
console.log('----------------------\n');

// Trading metrics
let sessionStartTime = new Date();
let totalTradesExecuted = 0;
let profitLoss = 0;

/**
 * Execute the signal generation and trading cycle
 */
async function executeFullCycle() {
  console.log(`\n====== CYCLE START: ${new Date().toISOString()} ======\n`);

  try {
    // Step 1: Generate signals
    console.log('Generating signals...');
    execSync('node brains.js', { stdio: 'inherit' });
    
    // Step 2: Execute trade validation and live trading
    console.log('\nValidating and executing trades...');
    execSync('node processor.js', { stdio: 'inherit' });
    
    // Step 3: Record metrics
    const tradeLog = readTradeLog();
    updateMetrics(tradeLog);
    
    console.log('\n--- SESSION METRICS ---');
    console.log(`Session duration: ${getSessionDuration()}`);
    console.log(`Total trades executed: ${totalTradesExecuted}`);
    console.log(`Current P/L: $${profitLoss.toFixed(2)}`);
    
    if (profitLoss <= -maxLoss) {
      console.warn('WARNING: Maximum loss reached. Trading will be disabled.');
    }
    
    console.log(`\n====== CYCLE COMPLETE: ${new Date().toISOString()} ======\n`);
  } catch (error) {
    console.error(`Error during execution cycle: ${error.message}`);
  }
}

/**
 * Read trade log file
 * @returns {Array} Array of trade log entries
 */
function readTradeLog() {
  const logPath = path.resolve(__dirname, 'trade_log.json');
  try {
    if (fs.existsSync(logPath)) {
      return JSON.parse(fs.readFileSync(logPath, 'utf-8'));
    }
    return [];
  } catch (error) {
    console.error(`Error reading trade log: ${error.message}`);
    return [];
  }
}

/**
 * Update metrics based on trade log
 * @param {Array} tradeLog - Array of trade log entries 
 */
function updateMetrics(tradeLog) {
  if (!tradeLog || tradeLog.length === 0) return;
  
  // Count recent trades
  const newTrades = tradeLog.filter(trade => 
    new Date(trade.timestamp) > sessionStartTime
  );
  
  totalTradesExecuted += newTrades.length;
  
  // Calculate profit/loss
  newTrades.forEach(trade => {
    if (trade.profitLoss) {
      profitLoss += trade.profitLoss;
    }
  });
}

/**
 * Get session duration as formatted string
 * @returns {string} Formatted duration
 */
function getSessionDuration() {
  const now = new Date();
  const durationMs = now - sessionStartTime;
  const hours = Math.floor(durationMs / (1000 * 60 * 60));
  const minutes = Math.floor((durationMs % (1000 * 60 * 60)) / (1000 * 60));
  const seconds = Math.floor((durationMs % (1000 * 60)) / 1000);
  
  return `${hours}h ${minutes}m ${seconds}s`;
}

/**
 * Main function
 */
async function main() {
  console.log('===== EUR/USD SIGNAL TRADING SYSTEM =====');
  console.log('Starting market signal generation and live trading...');
  console.log(`Using OANDA account: ${process.env.OANDA_ACCOUNT_ID}`);
  console.log(`Session start: ${sessionStartTime.toISOString()}`);
  console.log('=======================================\n');
  
  // Execute first cycle immediately
  await executeFullCycle();
  
  // Set up regular cycles if needed
  // const cycleInterval = 15 * 60 * 1000; // 15 minutes
  // setInterval(executeFullCycle, cycleInterval);
}

// Run the main function
main().catch(error => {
  console.error(`Fatal error: ${error.message}`);
  process.exit(1);
});