#!/usr/bin/env python3
"""
Test different configurations of the enhanced SEP Engine
"""

import subprocess
import json
from datetime import datetime

def run_testbed_with_config(stability_w, coherence_w, entropy_w, buy_thresh, sell_thresh):
    """Run testbed with specific configuration"""
    try:
        result = subprocess.run([
            './build/examples/pme_testbed', 'Testing/OANDA/O-test-2.json',
            str(stability_w), str(coherence_w), str(entropy_w), 
            str(buy_thresh), str(sell_thresh)
        ], capture_output=True, text=True, timeout=30, cwd='/sep')
        
        output = result.stdout + result.stderr
        for line in output.split('\n'):
            if 'Accuracy:' in line:
                accuracy_str = line.split('Accuracy: ')[1].replace('%', '')
                
                # Also extract predictions count
                total_preds = 0
                for pred_line in output.split('\n'):
                    if 'Total Predictions:' in pred_line:
                        total_preds = int(pred_line.split(': ')[1])
                        break
                
                return float(accuracy_str), total_preds
        return 0.0, 0
        
    except Exception as e:
        print(f"Error: {e}")
        return 0.0, 0

def main():
    print("🔬 Enhanced SEP Engine Configuration Testing")
    print("=" * 60)
    
    # Test current enhanced baseline
    baseline_acc, baseline_preds = run_testbed_with_config(0.4, 0.4, 0.2, 0.50, 0.52)
    print(f"Enhanced Baseline: {baseline_acc:.2f}% ({baseline_preds} predictions)")
    
    # Test variations
    configs = [
        # Description, stability_w, coherence_w, entropy_w, buy_thresh, sell_thresh
        ("Coherence Focus", 0.3, 0.5, 0.2, 0.48, 0.50),
        ("Stability Focus", 0.5, 0.3, 0.2, 0.52, 0.54),
        ("Entropy Balance", 0.35, 0.35, 0.3, 0.49, 0.51),
        ("Aggressive Trade", 0.4, 0.4, 0.2, 0.45, 0.47),
        ("Conservative", 0.4, 0.4, 0.2, 0.55, 0.57),
        ("Asymmetric Bull", 0.45, 0.35, 0.2, 0.47, 0.55),
        ("Asymmetric Bear", 0.35, 0.45, 0.2, 0.53, 0.48),
        ("High Entropy", 0.3, 0.3, 0.4, 0.50, 0.52),
    ]
    
    best_config = None
    best_accuracy = baseline_acc
    results = []
    
    print(f"\nTesting {len(configs)} configurations...")
    print("=" * 60)
    
    for desc, s_w, c_w, e_w, buy_t, sell_t in configs:
        accuracy, predictions = run_testbed_with_config(s_w, c_w, e_w, buy_t, sell_t)
        improvement = accuracy - baseline_acc
        
        result = {
            'description': desc,
            'accuracy': accuracy,
            'predictions': predictions,
            'improvement': improvement,
            'config': {'stability_w': s_w, 'coherence_w': c_w, 'entropy_w': e_w,
                      'buy_threshold': buy_t, 'sell_threshold': sell_t}
        }
        results.append(result)
        
        status = "✅" if improvement > 0 else "❌" if improvement < -0.5 else "➖"
        print(f"{status} {desc:15s}: {accuracy:5.2f}% ({improvement:+.2f}%) | {predictions:4d} trades")
        
        if accuracy > best_accuracy:
            best_accuracy = accuracy
            best_config = result
    
    print("\n" + "=" * 60)
    if best_config:
        print(f"🏆 BEST CONFIGURATION: {best_config['description']}")
        print(f"   Accuracy: {best_config['accuracy']:.2f}% (+{best_config['improvement']:.2f}%)")
        print(f"   Predictions: {best_config['predictions']}")
        print(f"   Config: {best_config['config']}")
    else:
        print("💡 Baseline configuration remains optimal")
    
    # Save results
    test_results = {
        'timestamp': datetime.now().isoformat(),
        'baseline_accuracy': baseline_acc,
        'best_accuracy': best_accuracy,
        'improvement': best_accuracy - baseline_acc,
        'best_config': best_config,
        'all_results': results
    }
    
    with open('/sep/configuration_test_results.json', 'w') as f:
        json.dump(test_results, f, indent=2)
    
    print(f"\n📊 Results saved to: /sep/configuration_test_results.json")
    print(f"Total improvement from original 47.24%: +{best_accuracy - 47.24:.2f}%")
    
    return best_config

if __name__ == "__main__":
    main()
