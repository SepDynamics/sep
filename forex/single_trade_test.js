// single_trade_test.js - Execute a single test trade with OANDA and verify account changes
// This script will execute one small trade and verify it with explicit account changes

const fs = require("fs");
const https = require("https");
require("dotenv").config();

// Required environment variables
const OANDA_API_KEY = process.env.OANDA_API_KEY;
const OANDA_ACCOUNT_ID = process.env.OANDA_ACCOUNT_ID;
const API_URL = "api-fxtrade.oanda.com";

// Trade parameters
const INSTRUMENT = "EUR_USD";
const UNITS = 100; // Increased position size for better execution chance
const DIRECTION = "BUY"; // BUY or SELL

// Validation tracking
let initialBalance = 0;
let finalBalance = 0;
let orderId = null;
let tradeId = null;
let transactionIds = [];

/**
 * Make API request to OANDA
 * @param {string} method - HTTP method
 * @param {string} endpoint - API endpoint
 * @param {Object} data - Request data for POST/PUT
 * @returns {Promise<Object>} Response data
 */
function makeRequest(method, endpoint, data = null) {
  return new Promise((resolve, reject) => {
    console.log(`Making ${method} request to: ${endpoint}`);

    const options = {
      hostname: API_URL,
      port: 443,
      path: endpoint,
      method: method,
      headers: {
        Authorization: `Bearer ${OANDA_API_KEY}`,
        "Content-Type": "application/json",
        "Accept-Datetime-Format": "RFC3339",
      },
    };

    const req = https.request(options, (res) => {
      let responseData = "";

      res.on("data", (chunk) => {
        responseData += chunk;
      });

      res.on("end", () => {
        if (res.statusCode >= 200 && res.statusCode < 300) {
          try {
            const parsed = JSON.parse(responseData);
            console.log(`Request successful (${res.statusCode})`);
            resolve(parsed);
          } catch (error) {
            reject(new Error(`Failed to parse response: ${error.message}`));
          }
        } else {
          try {
            const errorData = JSON.parse(responseData);
            console.error(
              `API Error (${res.statusCode}): ${JSON.stringify(
                errorData,
                null,
                2
              )}`
            );
            reject(
              new Error(
                `API Error ${res.statusCode}: ${JSON.stringify(errorData)}`
              )
            );
          } catch (error) {
            reject(new Error(`API Error ${res.statusCode}: ${responseData}`));
          }
        }
      });
    });

    req.on("error", (error) => {
      reject(new Error(`Request error: ${error.message}`));
    });

    if (data) {
      const jsonData = JSON.stringify(data);
      console.log(`Request body: ${jsonData}`);
      req.write(jsonData);
    }

    req.end();
  });
}

/**
 * Get account details
 * @returns {Promise<Object>} Account details
 */
async function getAccountDetails() {
  try {
    console.log(`\n==== Getting account details for ${OANDA_ACCOUNT_ID} ====`);
    const response = await makeRequest(
      "GET",
      `/v3/accounts/${OANDA_ACCOUNT_ID}`
    );
    console.log(`Account currency: ${response.account.currency}`);
    console.log(`Account balance: ${response.account.balance}`);
    console.log(`Margin available: ${response.account.marginAvailable}`);
    console.log(`Open trade count: ${response.account.openTradeCount}`);
    console.log(`Open position count: ${response.account.openPositionCount}`);
    console.log(`Pending order count: ${response.account.pendingOrderCount}`);
    console.log(`Unrealized PL: ${response.account.unrealizedPL}`);
    console.log(`NAV: ${response.account.NAV}`);

    return response.account;
  } catch (error) {
    console.error(`Error getting account details: ${error.message}`);
    throw error;
  }
}

/**
 * Execute a market order
 * @returns {Promise<Object>} Order response
 */
async function executeMarketOrder() {
  try {
    console.log(
      `\n==== Executing ${DIRECTION} market order for ${UNITS} units of ${INSTRUMENT} ====`
    );

    const units = DIRECTION === "BUY" ? UNITS : -UNITS;

    // Get current price for calculating stop loss
    const currentPrice = await getCurrentPrice();
    console.log(`Current EUR/USD price: ${currentPrice}`);

    const orderData = {
      order: {
        units: units.toString(),
        instrument: INSTRUMENT,
        timeInForce: "FOK", // Must use FOK for market orders in OANDA
        type: "MARKET",
        positionFill: "DEFAULT",
        // Add order comment for identification
        clientExtensions: {
          comment: "Signal system verification trade",
        },
      },
    };

    const response = await makeRequest(
      "POST",
      `/v3/accounts/${OANDA_ACCOUNT_ID}/orders`,
      orderData
    );

    // Save IDs for verification
    orderId = response.orderCreateTransaction?.id;

    if (response.orderFillTransaction) {
      tradeId = response.orderFillTransaction.tradeOpened?.tradeID;
    }

    if (response.relatedTransactionIDs) {
      transactionIds = response.relatedTransactionIDs;
    }

    console.log("Order response:");
    console.log(`- Order ID: ${orderId}`);
    console.log(`- Trade ID: ${tradeId || "Not available"}`);
    console.log(`- Related Transaction IDs: ${transactionIds.join(", ")}`);

    if (response.orderFillTransaction) {
      console.log(`- Fill price: ${response.orderFillTransaction.price}`);
      console.log(`- Fill time: ${response.orderFillTransaction.time}`);
      console.log(`- Filled units: ${response.orderFillTransaction.units}`);
    }

    return response;
  } catch (error) {
    console.error(`Error executing market order: ${error.message}`);
    throw error;
  }
}

/**
 * Get open trade details
 * @returns {Promise<Object>} Trade details
 */
async function getOpenTrades() {
  try {
    console.log(`\n==== Getting open trades ====`);
    const response = await makeRequest(
      "GET",
      `/v3/accounts/${OANDA_ACCOUNT_ID}/openTrades`
    );

    if (response.trades && response.trades.length > 0) {
      console.log(`Found ${response.trades.length} open trades:`);

      response.trades.forEach((trade) => {
        console.log(`\nTrade ID: ${trade.id}`);
        console.log(`Instrument: ${trade.instrument}`);
        console.log(`Units: ${trade.currentUnits}`);
        console.log(`Open time: ${trade.openTime}`);
        console.log(`Price: ${trade.price}`);
        console.log(`Current price: ${trade.currentPrice}`);
        console.log(`Unrealized PL: ${trade.unrealizedPL}`);
      });
    } else {
      console.log("No open trades found.");
    }

    return response.trades || [];
  } catch (error) {
    console.error(`Error getting open trades: ${error.message}`);
    throw error;
  }
}

/**
 * Get current price
 * @returns {Promise<string>} Current EUR/USD price
 */
async function getCurrentPrice() {
  try {
    const response = await makeRequest(
      "GET",
      `/v3/instruments/${INSTRUMENT}/candles?count=1&price=M&granularity=S5`
    );
    return response.candles[0].mid.c;
  } catch (error) {
    console.error(`Error getting current price: ${error.message}`);
    return "1.0830"; // Fallback price if request fails
  }
}

/**
 * Get transaction details
 * @param {string} transactionId - Transaction ID
 * @returns {Promise<Object>} Transaction details
 */
async function getTransaction(transactionId) {
  try {
    console.log(
      `\n==== Getting transaction details for ID: ${transactionId} ====`
    );
    const response = await makeRequest(
      "GET",
      `/v3/accounts/${OANDA_ACCOUNT_ID}/transactions/${transactionId}`
    );

    console.log("Transaction details:");
    console.log(`- Type: ${response.transaction.type}`);
    console.log(`- Instrument: ${response.transaction.instrument || "N/A"}`);
    console.log(`- Time: ${response.transaction.time}`);
    if (response.transaction.units) {
      console.log(`- Units: ${response.transaction.units}`);
    }
    if (response.transaction.price) {
      console.log(`- Price: ${response.transaction.price}`);
    }
    if (response.transaction.pl) {
      console.log(`- P/L: ${response.transaction.pl}`);
    }
    if (response.transaction.accountBalance) {
      console.log(`- Account balance: ${response.transaction.accountBalance}`);
    }

    return response.transaction;
  } catch (error) {
    console.error(`Error getting transaction: ${error.message}`);
    throw error;
  }
}

/**
 * Close an open trade
 * @param {string} tradeId - Trade ID to close
 * @returns {Promise<Object>} Close response
 */
async function closeTrade(tradeId) {
  try {
    console.log(`\n==== Closing trade ID: ${tradeId} ====`);

    const response = await makeRequest(
      "PUT",
      `/v3/accounts/${OANDA_ACCOUNT_ID}/trades/${tradeId}/close`
    );

    console.log("Trade close response:");
    console.log(`- Order ID: ${response.orderCreateTransaction?.id}`);

    if (response.orderFillTransaction) {
      console.log(`- Fill price: ${response.orderFillTransaction.price}`);
      console.log(`- Fill time: ${response.orderFillTransaction.time}`);
      console.log(`- P/L: ${response.orderFillTransaction.pl}`);
      console.log(
        `- Account balance: ${response.orderFillTransaction.accountBalance}`
      );
    }

    return response;
  } catch (error) {
    console.error(`Error closing trade: ${error.message}`);
    throw error;
  }
}

/**
 * Verify account changes
 * @returns {Promise<boolean>} Verification result
 */
async function verifyAccountChanges() {
  console.log("\n==== VERIFICATION SUMMARY ====");
  console.log(`Initial account balance: ${initialBalance}`);
  console.log(`Final account balance: ${finalBalance}`);

  const difference = finalBalance - initialBalance;
  console.log(`Balance difference: ${difference.toFixed(5)}`);

  if (difference !== 0) {
    console.log("✅ VERIFICATION SUCCESSFUL: Account balance changed");
    return true;
  } else {
    console.log("❌ VERIFICATION FAILED: No change in account balance");
    return false;
  }
}

/**
 * Run the test
 */
async function runTest() {
  console.log("===== OANDA SINGLE TRADE TEST =====");
  console.log(`Test started: ${new Date().toISOString()}`);
  console.log(`Account ID: ${OANDA_ACCOUNT_ID}`);
  console.log(`Instrument: ${INSTRUMENT}`);
  console.log(`Direction: ${DIRECTION}`);
  console.log(`Units: ${UNITS}`);
  console.log("===================================\n");

  try {
    // Step 1: Get initial account details
    const initialAccount = await getAccountDetails();
    initialBalance = parseFloat(initialAccount.balance);

    // Step 2: Execute market order
    const orderResponse = await executeMarketOrder();

    // Step 3: Get open trades
    const openTrades = await getOpenTrades();

    // Step 4: Get transaction details for each transaction ID
    for (const transactionId of transactionIds) {
      await getTransaction(transactionId);
    }

    // Step 5: Close the trade if we have a trade ID
    if (tradeId) {
      await closeTrade(tradeId);
    }

    // Step 6: Get final account details
    const finalAccount = await getAccountDetails();
    finalBalance = parseFloat(finalAccount.balance);

    // Step 7: Verify account changes
    const verified = await verifyAccountChanges();

    console.log(
      `\n===== TEST ${verified ? "COMPLETED SUCCESSFULLY" : "FAILED"} =====`
    );
    console.log(`Test ended: ${new Date().toISOString()}`);
    console.log("====================================\n");
  } catch (error) {
    console.error(`\nTest failed with error: ${error.message}`);
  }
}

// Run the test
runTest();
