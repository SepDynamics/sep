#!/usr/bin/env python3
"""
SEP Engine Accuracy Optimization Plan - Systematic Testing Framework

This script implements a systematic approach to testing different configurations
and improvements to reach our target of 70%+ accuracy from the current 47%.
"""

import subprocess
import json
import pandas as pd
import numpy as np
from datetime import datetime
import itertools

class AccuracyOptimizer:
    def __init__(self):
        self.current_baseline = 47.24
        self.target_accuracy = 70.0
        self.testbed_path = './build/examples/pme_testbed'
        self.data_path = 'Testing/OANDA/O-test-2.json'
        self.results = []
        
    def run_testbed_with_params(self, stability_w, coherence_w, entropy_w, 
                               buy_threshold, sell_threshold):
        """Run pme_testbed with specific parameters and return accuracy."""
        try:
            cmd = [
                self.testbed_path, self.data_path,
                str(stability_w), str(coherence_w), str(entropy_w),
                str(buy_threshold), str(sell_threshold)
            ]
            
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
            
            # Parse accuracy from output
            for line in result.stderr.split('\n'):
                if 'Accuracy:' in line:
                    accuracy_str = line.split('Accuracy: ')[1].replace('%', '')
                    return float(accuracy_str)
            return 0.0
            
        except Exception as e:
            print(f"Error running testbed: {e}")
            return 0.0
    
    def phase1_pattern_optimization(self):
        """Phase 1: Optimize current pattern metric weights"""
        print("🔬 Phase 1: Pattern Weight Optimization")
        print("=" * 50)
        
        # Define search space for pattern weights
        weights = [0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8]
        thresholds = [0.45, 0.50, 0.55, 0.60, 0.65, 0.70]
        
        best_accuracy = self.current_baseline
        best_config = None
        
        total_tests = len(weights) ** 3 * len(thresholds) ** 2
        test_count = 0
        
        for stability_w, coherence_w, entropy_w in itertools.product(weights, repeat=3):
            # Normalize weights to sum to 1.0
            total = stability_w + coherence_w + entropy_w
            stability_w /= total
            coherence_w /= total
            entropy_w /= total
            
            for buy_thresh, sell_thresh in itertools.product(thresholds, repeat=2):
                test_count += 1
                accuracy = self.run_testbed_with_params(
                    stability_w, coherence_w, entropy_w, buy_thresh, sell_thresh
                )
                
                if accuracy > best_accuracy:
                    best_accuracy = accuracy
                    best_config = {
                        'stability_w': stability_w,
                        'coherence_w': coherence_w, 
                        'entropy_w': entropy_w,
                        'buy_threshold': buy_thresh,
                        'sell_threshold': sell_thresh,
                        'accuracy': accuracy
                    }
                    print(f"✅ NEW BEST: {accuracy:.2f}% (gain: +{accuracy-self.current_baseline:.2f}%)")
                    print(f"   Config: s={stability_w:.3f}, c={coherence_w:.3f}, e={entropy_w:.3f}")
                    print(f"   Thresholds: buy={buy_thresh:.2f}, sell={sell_thresh:.2f}")
                
                if test_count % 50 == 0:
                    print(f"Progress: {test_count}/{total_tests} ({100*test_count/total_tests:.1f}%)")
        
        return best_config
    
    def test_advanced_configurations(self):
        """Test configurations with advanced pattern logic"""
        print("\n🚀 Testing Advanced Configurations")
        print("=" * 50)
        
        # Test asymmetric thresholds (different buy/sell)
        advanced_configs = [
            # Aggressive buy, conservative sell
            {'stability_w': 0.6, 'coherence_w': 0.3, 'entropy_w': 0.1, 
             'buy_threshold': 0.45, 'sell_threshold': 0.65},
            
            # Conservative buy, aggressive sell  
            {'stability_w': 0.4, 'coherence_w': 0.4, 'entropy_w': 0.2,
             'buy_threshold': 0.65, 'sell_threshold': 0.45},
             
            # Entropy-focused
            {'stability_w': 0.2, 'coherence_w': 0.3, 'entropy_w': 0.5,
             'buy_threshold': 0.55, 'sell_threshold': 0.55},
             
            # Stability-focused
            {'stability_w': 0.7, 'coherence_w': 0.2, 'entropy_w': 0.1,
             'buy_threshold': 0.50, 'sell_threshold': 0.50}
        ]
        
        results = []
        for i, config in enumerate(advanced_configs):
            accuracy = self.run_testbed_with_params(**config)
            config['accuracy'] = accuracy
            results.append(config)
            
            print(f"Config {i+1}: {accuracy:.2f}% (gain: +{accuracy-self.current_baseline:.2f}%)")
            
        return results
    
    def generate_optimization_report(self, phase1_best, advanced_results):
        """Generate comprehensive optimization report"""
        print("\n📊 OPTIMIZATION REPORT")
        print("=" * 60)
        print(f"Baseline Accuracy: {self.current_baseline:.2f}%")
        print(f"Target Accuracy: {self.target_accuracy:.2f}%")
        
        if phase1_best:
            improvement = phase1_best['accuracy'] - self.current_baseline
            print(f"\n🎯 Phase 1 Best Result: {phase1_best['accuracy']:.2f}% (+{improvement:.2f}%)")
            print("Optimal Configuration:")
            for key, value in phase1_best.items():
                if key != 'accuracy':
                    print(f"  {key}: {value:.3f}")
        
        print(f"\n📈 Advanced Configuration Results:")
        for i, result in enumerate(advanced_results):
            improvement = result['accuracy'] - self.current_baseline
            print(f"  Config {i+1}: {result['accuracy']:.2f}% (+{improvement:.2f}%)")
        
        best_overall = max([phase1_best] + advanced_results, key=lambda x: x['accuracy'])
        total_improvement = best_overall['accuracy'] - self.current_baseline
        
        print(f"\n🏆 BEST OVERALL: {best_overall['accuracy']:.2f}%")
        print(f"Total Improvement: +{total_improvement:.2f}%")
        print(f"Progress to Target: {100 * total_improvement / (self.target_accuracy - self.current_baseline):.1f}%")
        
        # Save results
        with open('/sep/optimization_results.json', 'w') as f:
            json.dump({
                'baseline': self.current_baseline,
                'target': self.target_accuracy,
                'phase1_best': phase1_best,
                'advanced_results': advanced_results,
                'best_overall': best_overall,
                'timestamp': datetime.now().isoformat()
            }, f, indent=2)
        
        return best_overall

def main():
    print("🎯 SEP Engine Accuracy Optimization")
    print("Targeting 70%+ accuracy from 47.24% baseline")
    print("=" * 60)
    
    optimizer = AccuracyOptimizer()
    
    # Phase 1: Systematic weight optimization
    phase1_best = optimizer.phase1_pattern_optimization()
    
    # Test advanced configurations
    advanced_results = optimizer.test_advanced_configurations()
    
    # Generate report
    best_config = optimizer.generate_optimization_report(phase1_best, advanced_results)
    
    print(f"\n✅ Optimization Complete! Best accuracy: {best_config['accuracy']:.2f}%")
    print("Results saved to: /sep/optimization_results.json")
    
    return best_config

if __name__ == "__main__":
    main()
