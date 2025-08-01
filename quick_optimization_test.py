#!/usr/bin/env python3
"""
Quick accuracy optimization test using the current pme_testbed
"""

import subprocess
import json
import time

def run_testbed():
    """Run the current testbed and extract accuracy"""
    try:
        result = subprocess.run(
            ['./build/examples/pme_testbed', 'Testing/OANDA/O-test-2.json'], 
            capture_output=True, text=True, timeout=30, cwd='/sep'
        )
        
        # Parse accuracy from stdout and stderr combined
        output = result.stdout + result.stderr
        for line in output.split('\n'):
            if 'Accuracy:' in line:
                accuracy_str = line.split('Accuracy: ')[1].replace('%', '')
                return float(accuracy_str)
        return 0.0
        
    except Exception as e:
        print(f"Error running testbed: {e}")
        return 0.0

def test_modified_pme():
    """Test current configuration and identify improvement opportunities"""
    print("🎯 SEP Engine Quick Optimization Test")
    print("=" * 50)
    
    # Run baseline test
    print("Running baseline test...")
    baseline_accuracy = run_testbed()
    print(f"✅ Baseline Accuracy: {baseline_accuracy:.2f}%")
    
    # Analyze the current implementation
    print("\n📊 Analysis of Current Implementation:")
    print("1. Pattern Metrics:")
    print("   - Coherence: 1.0 / (1.0 + range) → Too simplistic")
    print("   - Stability: (close - open) * 10000 → Just price change")  
    print("   - Entropy: abs(close-open) / range → Misused as phase")
    
    print("\n2. Signal Generation:")
    print("   - Fixed weights: stability=0.5, coherence=0.3, entropy=0.2")
    print("   - Single threshold: 0.55 for both BUY/SELL")
    print("   - No volatility adjustment or market regime detection")
    
    print("\n🎯 Recommended Optimization Sequence:")
    print("1. Enhanced Pattern Metrics (+8-12% expected)")
    print("2. Dynamic Thresholds (+3-5% expected)")  
    print("3. Volume Confirmation (+2-3% expected)")
    print("4. Multi-timeframe Context (+5-8% expected)")
    
    return baseline_accuracy

def prepare_enhanced_implementation():
    """Prepare enhanced version with better pattern calculations"""
    print("\n🚀 Preparing Enhanced Implementation...")
    
    enhanced_code = '''
    // Enhanced Pattern Calculations for SEP Engine
    
    // 1. True Coherence (autocorrelation-based)
    double calculateEnhancedCoherence(const std::vector<double>& prices) {
        if (prices.size() < 6) return 0.5;
        
        double autocorr = 0.0;
        double variance = 0.0;
        int lag = 3; // 3-candle lag for forex
        
        for (size_t i = lag; i < prices.size(); ++i) {
            double x = prices[i] - prices[i-1];
            double y = prices[i-lag] - prices[i-lag-1];
            autocorr += x * y;
            variance += x * x;
        }
        
        return variance > 0 ? 0.5 + 0.5 * (autocorr / variance) : 0.5;
    }
    
    // 2. Multi-timeframe Stability
    double calculateEnhancedStability(const std::vector<Candle>& candles, size_t idx) {
        if (idx < 10) return 0.5;
        
        double short_trend = 0.0, medium_trend = 0.0;
        
        // 3-candle short trend
        for (int i = 1; i <= 3; ++i) {
            short_trend += candles[idx].close - candles[idx-i].close;
        }
        
        // 10-candle medium trend  
        for (int i = 1; i <= 10; ++i) {
            medium_trend += candles[idx].close - candles[idx-i].close;
        }
        
        bool trends_align = (short_trend * medium_trend) > 0;
        double trend_ratio = std::abs(short_trend) / std::max(0.0001, std::abs(medium_trend));
        
        return trends_align ? 0.5 + 0.3 * std::min(1.0, trend_ratio) : 0.3;
    }
    
    // 3. Shannon Entropy for Market Randomness
    double calculateEnhancedEntropy(const std::vector<Candle>& candles, size_t idx) {
        if (idx < 10) return 0.5;
        
        std::vector<int> bins(5, 0); // 5 price movement categories
        
        for (int i = 1; i <= 10; ++i) {
            double change = candles[idx-i+1].close - candles[idx-i].close;
            double range = candles[idx-i].high - candles[idx-i].low;
            
            if (range > 0) {
                double norm_change = change / range;
                if (norm_change < -0.5) bins[0]++;      // Strong down
                else if (norm_change < -0.1) bins[1]++; // Down
                else if (norm_change < 0.1) bins[2]++;  // Flat
                else if (norm_change < 0.5) bins[3]++;  // Up
                else bins[4]++;                         // Strong up
            }
        }
        
        double entropy = 0.0;
        for (int count : bins) {
            if (count > 0) {
                double p = count / 10.0;
                entropy -= p * std::log2(p);
            }
        }
        
        return entropy / std::log2(5.0); // Normalize to [0,1]
    }
    '''
    
    print("Enhanced pattern calculation algorithms prepared.")
    print("Key improvements:")
    print("- Autocorrelation-based coherence measurement")
    print("- Multi-timeframe trend stability analysis")
    print("- Shannon entropy for market randomness quantification")
    
    return enhanced_code

def main():
    baseline = test_modified_pme()
    enhanced_code = prepare_enhanced_implementation()
    
    print(f"\n📈 Optimization Summary:")
    print(f"Current Accuracy: {baseline:.2f}%")
    print(f"Target Accuracy: 70.0%")
    print(f"Required Improvement: +{70.0 - baseline:.2f}%")
    
    print(f"\n✅ Next Steps:")
    print("1. Implement enhanced pattern metrics in pme_testbed.cpp")
    print("2. Add parameter tuning capability") 
    print("3. Test different weight combinations")
    print("4. Integrate with demo account for live validation")
    
    return baseline

if __name__ == "__main__":
    main()
