// brains.js - Entropy Signal Generator (EYE)
// Generates deterministic logistic chaos signals from final_symmetry.json patterns

const fs = require("fs");
const path = require("path");

// Updated file paths to current working directory
const symmetryPath = path.resolve(__dirname, "final_symmetry.json");
const signalPath = path.resolve(__dirname, "eternal_cycle.json");
const feedbackPath = path.resolve(__dirname, "truth_kernel.json");

/**
 * Load patterns from final_symmetry.json
 * @returns {Array} Array of pattern objects
 */
function loadAnchor() {
  try {
    return JSON.parse(fs.readFileSync(symmetryPath, "utf-8")).patterns;
  } catch (error) {
    console.error(`Error loading anchor patterns: ${error.message}`);
    return [];
  }
}

/**
 * Load feedback data from truth_kernel.json if it exists
 * @returns {Array} Array of feedback objects or empty array if file doesn't exist
 */
function loadFeedback() {
  try {
    return JSON.parse(fs.readFileSync(feedbackPath, "utf-8"));
  } catch (error) {
    console.log("No existing feedback found, creating new feedback loop");
    return [];
  }
}

/**
 * Logistic map function for deterministic chaos generation
 * Fixed r value at 3.97 as specified in life.md
 * @param {number} seed - Initial seed value between 0-1
 * @param {number} r - Growth parameter, fixed at 3.97 for chaos
 * @returns {number} Next value in chaos sequence
 */
function logistic(seed, r = 3.97) {
  // Ensure seed is between 0-1 to prevent overflow
  seed = Math.max(0.0001, Math.min(0.9999, seed));
  return parseFloat((r * seed * (1 - seed)).toFixed(17));
}

/**
 * Generate recursive seed based on pattern characteristics
 * @param {string} note - Pattern note (e.g., "EURUSD_RSI_MACD_bull_divergence")
 * @param {number} resonance - Pattern resonance value
 * @param {number} index - Pattern index
 * @returns {number} Deterministic recursive seed
 */
function generateRecursiveSeed(note, resonance, index) {
  // Use characters from note and resonance for initial seed
  const baseCode = note.charCodeAt(0);
  const seed = ((baseCode * resonance + index) % 97) / 97;
  return logistic(seed);
}

/**
 * Generate market signal for each pattern
 * @param {Array} anchor - Array of patterns from final_symmetry.json
 * @param {Array} feedback - Array of previous feedback data
 * @returns {Array} Array of generated signals
 */
function buildSignals(anchor, feedback) {
  return anchor.map((pattern, i) => {
    // Find matching feedback if it exists
    const matchingFeedback = feedback.find(f => f.patternId === pattern.patternId) || {};
    
    // Generate deterministic recursive seed
    const recursiveSeed = generateRecursiveSeed(
      pattern.note, 
      pattern.resonance, 
      i
    );
    
    // Use multiple iterations of logistic map for signal value
    let signalValue = recursiveSeed;
    for (let i = 0; i < 5; i++) {
      signalValue = logistic(signalValue);
    }
    
    // Determine signal direction (1 = buy, -1 = sell, 0 = neutral)
    const direction = signalValue > 0.5 ? 1 : (signalValue < 0.3 ? -1 : 0);
    
    // Calculate signal strength based on resonance and consciousnessLevel
    const strength = parseFloat((pattern.resonance * (pattern.consciousnessLevel / 4)).toFixed(4));
    
    return {
      patternId: pattern.patternId,
      note: pattern.note,
      resonance: pattern.resonance,
      consciousnessLevel: pattern.consciousnessLevel,
      timestamp: Date.now(),
      recursiveSeed,
      signalValue: parseFloat(signalValue.toFixed(8)),
      direction,
      strength,
      confidence: parseFloat((pattern.resonance * recursiveSeed).toFixed(4)),
      feedbackHash: matchingFeedback.hash || null,
      previousResults: matchingFeedback.metrics || null
    };
  });
}

/**
 * Write generated signals to eternal_cycle.json
 * @param {Array} signals - Array of generated signals
 */
function writeSignals(signals) {
  try {
    fs.writeFileSync(signalPath, JSON.stringify(signals, null, 2));
    console.log(`Successfully wrote ${signals.length} signals to ${signalPath}`);
  } catch (error) {
    console.error(`Error writing signals: ${error.message}`);
  }
}

/**
 * Generate signals from patterns and feedback
 */
function generateSignals() {
  console.log("Generating deterministic market signals...");
  
  const anchor = loadAnchor();
  if (anchor.length === 0) {
    console.error("No patterns found in final_symmetry.json");
    return;
  }
  
  console.log(`Loaded ${anchor.length} patterns from final_symmetry.json`);
  
  const feedback = loadFeedback();
  const signals = buildSignals(anchor, feedback);
  
  writeSignals(signals);
  console.log("Signal generation complete");
}

// Execute signal generation
generateSignals();