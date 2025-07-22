// oanda_connector.js - OANDA API Integration Module
// Connects to OANDA for real-time market data and direct trading

const fs = require('fs');
const path = require('path');
const https = require('https');
const dotenv = require('dotenv');

// Load environment variables
dotenv.config();

// API configuration with fallbacks to environment variables
const config = {
  apiKey: process.env.OANDA_API_KEY || '',
  accountId: process.env.OANDA_ACCOUNT_ID || '',
  maxLossUSD: parseFloat(process.env.OANDA_MAX_LOSS_USD || '100'), // Default $100 max loss
  riskPerTrade: parseFloat(process.env.OANDA_RISK_PER_TRADE || '2'), // Default 2% risk per trade
  baseCurrency: 'EUR_USD' // Default currency pair
};

// API URL - Live trading only
const API_URL = 'api-fxtrade.oanda.com';

// Validation flags
let isConfigValid = false;
let accountBalance = 0;
let totalLossUSD = 0;
let isConnected = false;
let tradingEnabled = true;

/**
 * Initialize the OANDA connector
 * @returns {Promise<boolean>} True if initialization successful
 */
async function initialize() {
  console.log('Initializing OANDA connector for LIVE trading...');
  
  // Validate API credentials
  if (!config.apiKey || !config.accountId) {
    console.error('ERROR: Missing OANDA API credentials. Check your .env file.');
    isConfigValid = false;
    return false;
  }
  
  try {
    // Test connection and get account details
    const accountInfo = await getAccountSummary();
    accountBalance = parseFloat(accountInfo.account.balance);
    isConnected = true;
    isConfigValid = true;
    
    console.log(`Successfully connected to OANDA account: ${config.accountId}`);
    console.log(`Account balance: $${accountBalance.toFixed(2)}`);
    console.log(`Maximum allowed loss: $${config.maxLossUSD}`);
    console.log(`Risk per trade: ${config.riskPerTrade}%`);
    
    // Check if we have sufficient balance
    if (accountBalance < 50) {
      console.warn('WARNING: Account balance is very low. Trading may be limited.');
    }
    
    return true;
  } catch (error) {
    console.error(`ERROR: Failed to initialize OANDA connection: ${error.message}`);
    isConnected = false;
    isConfigValid = false;
    return false;
  }
}

/**
 * Make API request to OANDA
 * @param {string} method - HTTP method (GET, POST, etc.)
 * @param {string} endpoint - API endpoint
 * @param {Object} data - Request data for POST/PUT requests
 * @returns {Promise<Object>} Response data
 */
function makeRequest(method, endpoint, data = null) {
  return new Promise((resolve, reject) => {
    const options = {
      hostname: API_URL,
      port: 443,
      path: endpoint,
      method: method,
      headers: {
        'Authorization': `Bearer ${config.apiKey}`,
        'Content-Type': 'application/json',
        'Accept-Datetime-Format': 'RFC3339'
      }
    };
    
    const req = https.request(options, (res) => {
      let responseData = '';
      
      res.on('data', (chunk) => {
        responseData += chunk;
      });
      
      res.on('end', () => {
        if (res.statusCode >= 200 && res.statusCode < 300) {
          try {
            resolve(JSON.parse(responseData));
          } catch (error) {
            reject(new Error(`Failed to parse response: ${error.message}`));
          }
        } else {
          try {
            const errorData = JSON.parse(responseData);
            reject(new Error(`API Error ${res.statusCode}: ${JSON.stringify(errorData)}`));
          } catch (error) {
            reject(new Error(`API Error ${res.statusCode}: ${responseData}`));
          }
        }
      });
    });
    
    req.on('error', (error) => {
      reject(new Error(`Request error: ${error.message}`));
    });
    
    if (data) {
      req.write(JSON.stringify(data));
    }
    
    req.end();
  });
}

/**
 * Get account summary
 * @returns {Promise<Object>} Account information
 */
async function getAccountSummary() {
  try {
    return await makeRequest('GET', `/v3/accounts/${config.accountId}/summary`);
  } catch (error) {
    throw new Error(`Failed to get account summary: ${error.message}`);
  }
}

/**
 * Get current price for instrument
 * @param {string} instrument - Instrument name (e.g., 'EUR_USD')
 * @returns {Promise<Object>} Current price data
 */
async function getCurrentPrice(instrument = config.baseCurrency) {
  try {
    const response = await makeRequest('GET', `/v3/instruments/${instrument}/candles?count=10&price=M&granularity=M1`);
    return {
      instrument,
      time: new Date().toISOString(),
      bid: parseFloat(response.candles[response.candles.length - 1].mid.c),
      ask: parseFloat(response.candles[response.candles.length - 1].mid.c),
      spread: 0.0002, // Approximated spread
      candles: response.candles
    };
  } catch (error) {
    throw new Error(`Failed to get current price for ${instrument}: ${error.message}`);
  }
}

/**
 * Calculate ATR (Average True Range) for instrument
 * @param {string} instrument - Instrument name (e.g., 'EUR_USD')
 * @param {string} granularity - Candle granularity (default: 'H1')
 * @param {number} periods - Number of periods for ATR calculation (default: 14)
 * @returns {Promise<number>} ATR value
 */
async function calculateATR(instrument = config.baseCurrency, granularity = 'H1', periods = 14) {
  try {
    const response = await makeRequest('GET', 
      `/v3/instruments/${instrument}/candles?count=${periods+1}&price=M&granularity=${granularity}`);
    
    if (!response.candles || response.candles.length < periods) {
      throw new Error('Insufficient candle data for ATR calculation');
    }
    
    // Calculate true ranges
    const trueRanges = [];
    for (let i = 1; i < response.candles.length; i++) {
      const high = parseFloat(response.candles[i].mid.h);
      const low = parseFloat(response.candles[i].mid.l);
      const prevClose = parseFloat(response.candles[i-1].mid.c);
      
      // True Range = max(high - low, |high - prevClose|, |low - prevClose|)
      const tr1 = high - low;
      const tr2 = Math.abs(high - prevClose);
      const tr3 = Math.abs(low - prevClose);
      const trueRange = Math.max(tr1, tr2, tr3);
      trueRanges.push(trueRange);
    }
    
    // Calculate average true range
    const atr = trueRanges.reduce((sum, tr) => sum + tr, 0) / trueRanges.length;
    return atr;
  } catch (error) {
    throw new Error(`Failed to calculate ATR for ${instrument}: ${error.message}`);
  }
}

/**
 * Execute trade based on signal
 * @param {Object} signal - Signal object from signal generator
 * @param {Object} marketData - Current market data
 * @returns {Promise<Object>} Trade result
 */
async function executeTrade(signal, marketData) {
  // Safety checks
  if (!isConfigValid || !isConnected) {
    throw new Error('OANDA connector not properly initialized');
  }
  
  if (!tradingEnabled) {
    console.warn('Trading is disabled due to maximum loss reached');
    return {
      executed: false,
      reason: 'Trading disabled due to maximum loss reached',
      signal: signal
    };
  }
  
  // Skip trades with low confidence
  if (signal.resonance < 0.55) {
    return {
      executed: false,
      reason: 'Signal resonance too low',
      signal: signal
    };
  }
  
  try {
    // Calculate position size based on risk parameters
    const accountInfo = await getAccountSummary();
    const currentBalance = parseFloat(accountInfo.account.balance);
    
    // Use ATR for dynamic stop loss if available, otherwise default to 5 pips
    const stopLossPips = marketData.atr ? Math.ceil(marketData.atr * 10000) : 5;
    
    // Calculate position size (risk % of account per trade)
    const riskAmount = currentBalance * (config.riskPerTrade / 100);
    const pipValue = 0.0001; // For EUR/USD, 1 pip = 0.0001
    const positionUnits = Math.floor(riskAmount / (stopLossPips * pipValue));
    
    // Determine trade direction from signal
    const units = signal.direction > 0 ? positionUnits : -positionUnits;
    
    // Log the trade details
    console.log('-------------------------------------');
    console.log(`LIVE TRADE EXECUTION [${new Date().toISOString()}]`);
    console.log(`Instrument: ${config.baseCurrency}`);
    console.log(`Direction: ${signal.direction > 0 ? 'BUY' : 'SELL'}`);
    console.log(`Units: ${units}`);
    console.log(`Current price: ${marketData.bid}`);
    console.log(`Stop loss: ${marketData.bid - (signal.direction * stopLossPips * pipValue)}`);
    console.log(`Signal confidence: ${signal.confidence}`);
    console.log(`Signal resonance: ${signal.resonance}`);
    console.log('-------------------------------------');
    
    // Execute the trade with OANDA API
    const stopLossPrice = (signal.direction > 0) 
      ? (marketData.bid - (stopLossPips * pipValue)).toFixed(5)
      : (marketData.ask + (stopLossPips * pipValue)).toFixed(5);
      
    const orderData = {
      order: {
        units: units.toString(),
        instrument: config.baseCurrency,
        timeInForce: 'FOK',
        type: 'MARKET',
        positionFill: 'DEFAULT',
        stopLossOnFill: {
          price: stopLossPrice,
          timeInForce: 'GTC'
        }
      }
    };
    
    const response = await makeRequest('POST', `/v3/accounts/${config.accountId}/orders`, orderData);
    
    console.log(`Order executed: ID ${response.orderCreateTransaction.id}`);
    console.log(`Filled price: ${response.orderFillTransaction?.price || 'Unknown'}`);
    
    return {
      executed: true,
      orderId: response.orderCreateTransaction.id,
      price: response.orderFillTransaction?.price,
      units: units,
      stopLoss: stopLossPrice,
      signal: signal
    };
  } catch (error) {
    console.error(`Trade execution error: ${error.message}`);
    return {
      executed: false,
      error: error.message,
      signal: signal
    };
  }
}

/**
 * Get market data including price and ATR
 * @param {string} instrument - Instrument name (e.g., 'EUR_USD')
 * @returns {Promise<Object>} Market data
 */
async function getMarketData(instrument = config.baseCurrency) {
  try {
    const priceData = await getCurrentPrice(instrument);
    const atr = await calculateATR(instrument);
    
    return {
      instrument: instrument,
      time: priceData.time,
      bid: priceData.bid,
      ask: priceData.ask,
      spread: priceData.ask - priceData.bid,
      atr: atr,
      volatilityLevel: getVolatilityLevel(atr),
      dailyChange: 0, // Would calculate from daily candles in a full implementation
      volume: 0 // OANDA doesn't provide volume data
    };
  } catch (error) {
    throw new Error(`Failed to get market data: ${error.message}`);
  }
}

/**
 * Get volatility level based on ATR
 * @param {number} atr - ATR value
 * @returns {number} Volatility level (1-4)
 */
function getVolatilityLevel(atr) {
  if (atr < 0.003) return 1;
  if (atr < 0.007) return 2;
  if (atr < 0.012) return 3;
  return 4;
}

/**
 * Check if the connector is properly initialized
 * @returns {boolean} True if connector is initialized
 */
function isInitialized() {
  return isConfigValid && isConnected;
}

/**
 * Get current trading status
 * @returns {Object} Trading status
 */
function getTradingStatus() {
  return {
    enabled: tradingEnabled,
    totalLoss: totalLossUSD,
    maxLoss: config.maxLossUSD,
    balance: accountBalance
  };
}

/**
 * Reset trading status (e.g., to reset loss counter)
 */
function resetTradingStatus() {
  totalLossUSD = 0;
  tradingEnabled = true;
  console.log('Trading status reset');
}

/**
 * Update loss tracking after a trade
 * @param {number} profitLoss - Profit/loss amount in USD
 * @returns {boolean} Whether trading is still enabled
 */
function updateLossTracking(profitLoss) {
  if (profitLoss < 0) {
    totalLossUSD += Math.abs(profitLoss);
    
    if (totalLossUSD >= config.maxLossUSD) {
      tradingEnabled = false;
      console.warn(`WARNING: Maximum loss limit of $${config.maxLossUSD} reached. Trading disabled.`);
    }
  }
  
  return tradingEnabled;
}

// Export functions
module.exports = {
  initialize,
  getMarketData,
  executeTrade,
  getAccountSummary,
  isInitialized,
  getTradingStatus,
  resetTradingStatus,
  updateLossTracking
};