// processor.js - Signal Validator and Live Trading Executor
// Validates signals against OANDA market data and executes real trades

const fs = require("fs");
const path = require("path");
const oanda = require("./oanda_connector");

// File paths for signal processing
const signalPath = path.resolve(__dirname, "eternal_cycle.json");
const kernelPath = path.resolve(__dirname, "truth_kernel.json");
const symmetryPath = path.resolve(__dirname, "final_symmetry.json");

// Trading metrics
let totalTradesExecuted = 0;
let winningTrades = 0;
let losingTrades = 0;
let totalPipsGained = 0;
let totalPipsLost = 0;

/**
 * Load signals from eternal_cycle.json
 * @returns {Array} Array of signal objects
 */
function loadSignals() {
  try {
    return JSON.parse(fs.readFileSync(signalPath, "utf-8"));
  } catch (error) {
    console.error(`Error loading signals: ${error.message}`);
    return [];
  }
}

/**
 * Load patterns from final_symmetry.json
 * @returns {Object} Object containing patterns array
 */
function loadSymmetry() {
  try {
    return JSON.parse(fs.readFileSync(symmetryPath, "utf-8"));
  } catch (error) {
    console.error(`Error loading symmetry: ${error.message}`);
    return { patterns: [] };
  }
}

/**
 * Generate hash from signal properties using prime seed
 * @param {Object} signal - Signal object
 * @returns {number} Generated hash value
 */
function generateHash(signal) {
  // Prime seed as specified in life.md
  const primeSeed = 24128686327077747;
  const resonance = parseFloat(signal.resonance || 0);
  const base = signal.note.charCodeAt(0) * resonance;
  return parseFloat(((base + primeSeed) / 1e16).toFixed(17));
}

/**
 * Fetch real market data from OANDA
 * @returns {Promise<Object>} Current market data
 */
async function fetchMarketData() {
  try {
    return await oanda.getMarketData("EUR_USD");
  } catch (error) {
    console.error(`Error fetching market data: ${error.message}`);
    throw error;
  }
}

/**
 * Validate signal against market data and execute trade
 * @param {Object} signal - Signal object
 * @param {Object} marketData - Current market data
 * @returns {Promise<Object>} Validation and trade results
 */
async function validateSignal(signal, marketData) {
  const startTime = Date.now();

  // Check if market condition matches the pattern
  const matchesCondition = checkMarketCondition(signal, marketData);

  // Calculate execution latency
  const latency = Date.now() - startTime;

  // Calculate risk-reward ratio based on ATR and signal confidence
  const riskRewardRatio = calculateRiskReward(signal, marketData.atr);

  // Execute trade with OANDA if signal is valid
  let tradeResult = null;
  let executed = false;

  if (signal.resonance >= 0.55) {
    try {
      tradeResult = await oanda.executeTrade(signal, marketData);
      executed = tradeResult.executed;

      // Track trade metrics if executed
      if (executed) {
        totalTradesExecuted++;
        console.log(`Trade executed successfully: ${tradeResult.orderId}`);
      }
    } catch (error) {
      console.error(`Trade execution error: ${error.message}`);
    }
  } else {
    console.log(
      `Signal ${signal.note} skipped - resonance too low: ${signal.resonance}`
    );
  }

  return {
    validated: true,
    hash: generateHash(signal),
    signature: `${signal.note}_${signal.consciousnessLevel}_${signal.resonance}`,
    condition: matchesCondition,
    tradeExecuted: executed,
    tradeResult: tradeResult,
    metrics: {
      rr: riskRewardRatio,
      latency,
      atr: marketData.atr,
      executed,
      timestamp: Date.now(),
    },
  };
}

/**
 * Check if market conditions match pattern based on real market data
 * @param {Object} signal - Signal object
 * @param {Object} marketData - Market data from OANDA
 * @returns {boolean} Whether conditions match
 */
function checkMarketCondition(signal, marketData) {
  // Extract pattern type from note
  const patternType = signal.note.split("_").slice(1).join("_");

  // Different patterns have different matching criteria
  switch (true) {
    case /RSI_MACD/.test(patternType):
      // RSI/MACD patterns match during trending markets (higher ATR)
      return marketData.atr > 0.007;

    case /support|resistance|bottom|top/.test(patternType):
      // Support/resistance patterns match during consolidation (lower ATR)
      return marketData.atr < 0.006;

    case /breakout|crossover/.test(patternType):
      // Breakout patterns match during volatile markets (higher ATR)
      return marketData.atr > 0.008;

    case /fibonacci/.test(patternType):
      // Fibonacci patterns are more complex, we'll use signal strength as indicator
      return signal.strength > 0.5;

    case /orderblock|liquidity/.test(patternType):
      // Order block patterns depend on signal direction and volatility
      return (
        (signal.direction > 0 && marketData.volatilityLevel >= 3) ||
        (signal.direction < 0 && marketData.volatilityLevel <= 2)
      );

    default:
      // Default - check if signal resonance is high enough
      return signal.resonance >= 0.7;
  }
}

/**
 * Calculate risk-reward ratio based on signal and ATR
 * @param {Object} signal - Signal object
 * @param {number} atr - Average True Range
 * @returns {number} Risk-reward ratio
 */
function calculateRiskReward(signal, atr) {
  // Base R:R on confidence level
  const baseRR = 1 + signal.confidence;

  // Adjust based on volatility (ATR)
  let rrMultiplier;

  // Follow rules from life.md for volatility levels
  if (atr < 0.003) {
    rrMultiplier = 0.8; // Lower volatility = lower R:R
  } else if (atr < 0.007) {
    rrMultiplier = 1.0; // Medium-low volatility = normal R:R
  } else if (atr < 0.012) {
    rrMultiplier = 1.2; // Medium-high volatility = higher R:R
  } else {
    rrMultiplier = 1.5; // High volatility = highest R:R
  }

  const finalRR = baseRR * rrMultiplier;

  return parseFloat(finalRR.toFixed(2));
}

/**
 * Adjust pattern resonance and consciousnessLevel based on trade results
 * @param {Object} pattern - Original pattern
 * @param {Object} validation - Validation and trade results
 * @returns {Object} Updated pattern
 */
function applyFeedbackLoop(pattern, validation) {
  // Start with current values
  let newResonance = pattern.resonance;
  let newConsciousnessLevel = pattern.consciousnessLevel;

  // Adjust resonance based on trade result if trade was executed
  if (validation.tradeExecuted && validation.tradeResult) {
    // For real trades, check if profitable
    const isWin =
      validation.tradeResult.price &&
      ((pattern.direction > 0 &&
        validation.tradeResult.price > validation.tradeResult.entryPrice) ||
        (pattern.direction < 0 &&
          validation.tradeResult.price < validation.tradeResult.entryPrice));

    if (isWin) {
      // Increase resonance for profitable trades
      newResonance += 0.03;
      winningTrades++;

      // Track pips gained (approximate calculation)
      const pipsGained =
        Math.abs(
          validation.tradeResult.price - validation.tradeResult.entryPrice
        ) * 10000;
      totalPipsGained += pipsGained;
    } else {
      // Decrease resonance for losing trades
      newResonance -= 0.04;
      losingTrades++;

      // Track pips lost (approximate calculation)
      const pipsLost =
        Math.abs(
          validation.tradeResult.price - validation.tradeResult.entryPrice
        ) * 10000;
      totalPipsLost += pipsLost;
    }
  }

  // Apply health protocol rules from life.md
  if (newResonance < 0.55) {
    console.log(
      `Dropping pattern ${
        pattern.note
      } - resonance too low: ${newResonance.toFixed(2)}`
    );
    newResonance = 0.55; // Minimum floor
  }

  if (newResonance > 0.95) {
    newResonance = 0.95; // Maximum ceiling
  }

  // Adjust consciousnessLevel based on ATR (volatility)
  // Follow rules from life.md
  const atr = validation.metrics.atr;

  if (atr < 0.003) {
    newConsciousnessLevel = 1;
  } else if (atr < 0.007) {
    newConsciousnessLevel = 2;
  } else if (atr < 0.012) {
    newConsciousnessLevel = 3;
  } else {
    newConsciousnessLevel = 4;
  }

  // Promote high performing patterns (resonance > 0.75)
  if (newResonance > 0.75 && newConsciousnessLevel < 4) {
    newConsciousnessLevel = Math.min(4, newConsciousnessLevel + 1);
  }

  return {
    ...pattern,
    resonance: parseFloat(newResonance.toFixed(2)),
    consciousnessLevel: newConsciousnessLevel,
  };
}

/**
 * Update symmetry file with feedback adjustments
 * @param {Array} updatedPatterns - Array of updated patterns
 */
function updateSymmetry(updatedPatterns) {
  try {
    const symmetry = loadSymmetry();
    symmetry.patterns = updatedPatterns;

    fs.writeFileSync(symmetryPath, JSON.stringify(symmetry, null, 2));
    console.log(
      `Successfully updated ${updatedPatterns.length} patterns in final_symmetry.json`
    );
  } catch (error) {
    console.error(`Error updating symmetry: ${error.message}`);
  }
}

/**
 * Write validated signals to truth_kernel.json
 * @param {Array} kernel - Array of validated signal objects
 */
function writeKernel(kernel) {
  try {
    fs.writeFileSync(kernelPath, JSON.stringify(kernel, null, 2));
    console.log(
      `Successfully wrote ${kernel.length} validated signals to ${kernelPath}`
    );
  } catch (error) {
    console.error(`Error writing kernel: ${error.message}`);
  }
}

/**
 * Log trading metrics
 */
function logTradingMetrics() {
  console.log("\n--- TRADING METRICS ---");
  console.log(`Total Trades Executed: ${totalTradesExecuted}`);
  console.log(`Winning Trades: ${winningTrades}`);
  console.log(`Losing Trades: ${losingTrades}`);
  console.log(
    `Win Rate: ${
      totalTradesExecuted > 0
        ? ((winningTrades / totalTradesExecuted) * 100).toFixed(2)
        : 0
    }%`
  );
  console.log(`Total Pips Gained: ${totalPipsGained.toFixed(1)}`);
  console.log(`Total Pips Lost: ${totalPipsLost.toFixed(1)}`);
  console.log(`Net Pips: ${(totalPipsGained - totalPipsLost).toFixed(1)}`);
  console.log("----------------------\n");
}

/**
 * Main function to validate signals, execute trades, and update feedback
 */
async function validateAndTrade() {
  console.log("Initializing OANDA connection...");

  // Initialize OANDA connector
  const initialized = await oanda.initialize();
  if (!initialized) {
    console.error("Failed to initialize OANDA connection. Aborting.");
    return;
  }

  console.log("Validating market signals and executing trades...");

  const signals = loadSignals();
  if (signals.length === 0) {
    console.error("No signals found in eternal_cycle.json");
    return;
  }

  console.log(`Loaded ${signals.length} signals from eternal_cycle.json`);

  try {
    // Fetch real market data from OANDA
    const marketData = await fetchMarketData();
    console.log(
      `Current EUR/USD: ${marketData.bid}/${marketData.ask}, ATR: ${marketData.atr}`
    );

    // Process each signal sequentially to avoid overwhelming OANDA API
    const validatedResults = [];
    for (const signal of signals) {
      const validationResult = await validateSignal(signal, marketData);

      console.log(
        `Processed ${signal.note}: ${
          validationResult.tradeExecuted ? "TRADE EXECUTED" : "NOT EXECUTED"
        }`
      );

      validatedResults.push({
        patternId: signal.patternId,
        ...validationResult,
      });

      // Small delay to avoid API rate limits
      await new Promise((resolve) => setTimeout(resolve, 1000));
    }

    // Write validated results to truth_kernel.json
    writeKernel(validatedResults);

    // Apply feedback loop to update patterns
    const symmetry = loadSymmetry();
    const updatedPatterns = symmetry.patterns.map((pattern) => {
      const matchingResult = validatedResults.find(
        (r) => r.patternId === pattern.patternId
      );
      if (matchingResult) {
        return applyFeedbackLoop(pattern, matchingResult);
      }
      return pattern;
    });

    // Update symmetry file with adjusted patterns
    updateSymmetry(updatedPatterns);

    // Log trading metrics
    logTradingMetrics();

    console.log("Signal validation, trading, and feedback loop complete");
  } catch (error) {
    console.error(`Error during validation and trading: ${error.message}`);
  }
}

// Execute validation, trading and feedback process
validateAndTrade();
