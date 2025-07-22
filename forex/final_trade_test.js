// final_trade_test.js - Direct OANDA trade execution
// Executes a single market order with sufficient size to verify account changes

const https = require("https");
require("dotenv").config();

// Required environment variables
const OANDA_API_KEY = process.env.OANDA_API_KEY;
const OANDA_ACCOUNT_ID = process.env.OANDA_ACCOUNT_ID;
const API_URL = "api-fxtrade.oanda.com";

// Trade parameters - larger size for definitive testing
const INSTRUMENT = "EUR_USD";
const UNITS = 1000; // Larger position size for definitive test
const DIRECTION = "BUY"; // BUY or SELL

// Start tracking
let initialBalance;
let finalBalance;

/**
 * Make API request to OANDA
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
            console.error(`API Error (${res.statusCode}): ${JSON.stringify(errorData)}`);
            reject(new Error(`API Error ${res.statusCode}: ${JSON.stringify(errorData)}`));
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
 * Get account summary 
 */
async function getAccountSummary() {
  try {
    console.log(`\n========= ACCOUNT SUMMARY =========`);
    const response = await makeRequest("GET", `/v3/accounts/${OANDA_ACCOUNT_ID}/summary`);
    console.log(`Account ID: ${response.account.id}`);
    console.log(`Account Name: ${response.account.alias}`);
    console.log(`Account Currency: ${response.account.currency}`);
    console.log(`Balance: ${response.account.balance}`);
    console.log(`Margin Available: ${response.account.marginAvailable}`);
    console.log(`Margin Used: ${response.account.marginUsed}`);
    console.log(`Open Trade Count: ${response.account.openTradeCount}`);
    console.log(`Unrealized PL: ${response.account.unrealizedPL}`);
    console.log(`NAV: ${response.account.NAV}`);
    console.log(`====================================\n`);
    
    return response.account;
  } catch (error) {
    console.error(`Error getting account summary: ${error.message}`);
    throw error;
  }
}

/**
 * Execute a market order with immediate or cancel time in force
 */
async function executeMarketOrder() {
  try {
    console.log(`\n========= EXECUTING MARKET ORDER =========`);
    console.log(`Instrument: ${INSTRUMENT}`);
    console.log(`Direction: ${DIRECTION}`);
    console.log(`Units: ${UNITS}`);
    
    const units = DIRECTION === "BUY" ? UNITS : -UNITS;
    
    // Direct market order with FOK time in force (Fill Or Kill)
    const orderData = {
      order: {
        units: units.toString(),
        instrument: INSTRUMENT,
        timeInForce: "FOK", // Fill or Kill
        type: "MARKET",
        positionFill: "DEFAULT"
      }
    };
    
    const response = await makeRequest("POST", `/v3/accounts/${OANDA_ACCOUNT_ID}/orders`, orderData);
    
    console.log(`\n===== ORDER RESPONSE =====`);
    
    // Order created transaction
    if (response.orderCreateTransaction) {
      console.log(`Order Create Transaction:`);
      console.log(`- ID: ${response.orderCreateTransaction.id}`);
      console.log(`- Type: ${response.orderCreateTransaction.type}`);
      console.log(`- Time: ${response.orderCreateTransaction.time}`);
      console.log(`- Units: ${response.orderCreateTransaction.units}`);
    }
    
    // Order fill transaction
    if (response.orderFillTransaction) {
      console.log(`\nOrder Fill Transaction:`);
      console.log(`- ID: ${response.orderFillTransaction.id}`);
      console.log(`- Type: ${response.orderFillTransaction.type}`);
      console.log(`- Time: ${response.orderFillTransaction.time}`);
      console.log(`- Units: ${response.orderFillTransaction.units}`);
      console.log(`- Price: ${response.orderFillTransaction.price}`);
      console.log(`- PL: ${response.orderFillTransaction.pl}`);
      console.log(`- Commission: ${response.orderFillTransaction.commission}`);
      
      if (response.orderFillTransaction.tradeOpened) {
        console.log(`- Trade ID: ${response.orderFillTransaction.tradeOpened.tradeID}`);
      }
      
      // Check for guaranteed execution
      if (response.orderFillTransaction.price) {
        console.log(`✅ TRADE EXECUTED SUCCESSFULLY at price ${response.orderFillTransaction.price}`);
        return true;
      }
    }
    
    // Order cancel transaction
    if (response.orderCancelTransaction) {
      console.log(`\nOrder Cancel Transaction:`);
      console.log(`- ID: ${response.orderCancelTransaction.id}`);
      console.log(`- Type: ${response.orderCancelTransaction.type}`);
      console.log(`- Time: ${response.orderCancelTransaction.time}`);
      console.log(`- Reason: ${response.orderCancelTransaction.reason}`);
      console.log(`❌ ORDER CANCELLED: ${response.orderCancelTransaction.reason}`);
      return false;
    }
    
    return response.orderFillTransaction ? true : false;
  } catch (error) {
    console.error(`Error executing market order: ${error.message}`);
    return false;
  }
}

/**
 * Verify account changes
 */
async function verifyAccountChanges() {
  console.log(`\n========= VERIFICATION RESULTS =========`);
  console.log(`Initial Balance: ${initialBalance}`);
  console.log(`Final Balance: ${finalBalance}`);
  
  const difference = parseFloat((finalBalance - initialBalance).toFixed(4));
  console.log(`Balance Difference: ${difference}`);
  
  if (difference !== 0) {
    console.log(`✅ VERIFICATION SUCCESSFUL - Account balance changed by ${difference}`);
    return true;
  } else {
    console.log(`❌ VERIFICATION FAILED - No change in account balance`);
    return false;
  }
}

/**
 * Main function
 */
async function main() {
  console.log(`===== DIRECT OANDA TRADE TEST =====`);
  console.log(`Starting execution at: ${new Date().toISOString()}`);
  
  try {
    // Get initial account state
    const initialAccount = await getAccountSummary();
    initialBalance = parseFloat(initialAccount.balance);
    
    // Execute market order
    const success = await executeMarketOrder();
    
    // Wait a moment to ensure OANDA processes the trade
    console.log(`\nWaiting 3 seconds for trade processing...`);
    await new Promise(resolve => setTimeout(resolve, 3000));
    
    // Get final account state
    const finalAccount = await getAccountSummary();
    finalBalance = parseFloat(finalAccount.balance);
    
    // Verify changes
    const verified = await verifyAccountChanges();
    
    console.log(`\n===== TEST ${verified ? "PASSED" : "FAILED"} =====`);
    console.log(`Execution finished at: ${new Date().toISOString()}`);
    
  } catch (error) {
    console.error(`Fatal error: ${error.message}`);
  }
}

// Run the main function
main();