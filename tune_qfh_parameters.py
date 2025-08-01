#!/usr/bin/env python3
"""
QFH Parameter Tuning Script
Systematically tune QFH trajectory damping parameters for optimal accuracy.
Based on TASK.md Priority 1 recommendations.
"""

import subprocess
import re
import json
import os
from typing import Dict, Tuple, List

def modify_qfh_parameters(k1: float, k2: float, trajectory_weight: float = 0.3):
    """Modify the QFH parameters in qfh.cpp"""
    qfh_path = "/sep/src/quantum/bitspace/qfh.cpp"
    
    # Read the file
    with open(qfh_path, 'r') as f:
        content = f.read()
    
    # Replace k1 parameter (around line 130)
    content = re.sub(
        r'const double k1 = [0-9.]+;',
        f'const double k1 = {k1:.2f};',
        content
    )
    
    # Replace k2 parameter (around line 131)
    content = re.sub(
        r'const double k2 = [0-9.]+;',
        f'const double k2 = {k2:.2f};',
        content
    )
    
    # Replace trajectory weight (around line 343)
    pattern_weight = 1.0 - trajectory_weight
    content = re.sub(
        r'result\.coherence = [0-9.]+f \* trajectory_coherence \+ [0-9.]+f \* pattern_coherence;',
        f'result.coherence = {trajectory_weight:.1f}f * trajectory_coherence + {pattern_weight:.1f}f * pattern_coherence;',
        content
    )
    
    # Write back
    with open(qfh_path, 'w') as f:
        f.write(content)

def build_system() -> bool:
    """Build the system and return success status"""
    result = subprocess.run(['./build.sh'], 
                          capture_output=True, 
                          text=True,
                          cwd='/sep')
    return result.returncode == 0

def run_test() -> Dict[str, float]:
    """Run the testbed and extract accuracy metrics"""
    result = subprocess.run([
        './build/examples/pme_testbed_phase2', 
        'Testing/OANDA/O-test-2.json'
    ], capture_output=True, text=True, cwd='/sep')
    
    if result.returncode != 0:
        return {'overall_accuracy': 0.0, 'high_conf_accuracy': 0.0, 'high_conf_rate': 0.0}
    
    output = result.stdout
    
    # Extract accuracy metrics
    overall_match = re.search(r'Overall Accuracy: ([0-9.]+)%', output)
    high_conf_match = re.search(r'High Confidence Accuracy: ([0-9.]+)%', output)
    high_conf_signals_match = re.search(r'High Confidence Signals: (\d+) \(([0-9.]+)%\)', output)
    
    metrics = {
        'overall_accuracy': float(overall_match.group(1)) if overall_match else 0.0,
        'high_conf_accuracy': float(high_conf_match.group(1)) if high_conf_match else 0.0,
        'high_conf_rate': float(high_conf_signals_match.group(2)) if high_conf_signals_match else 0.0
    }
    
    return metrics

def tune_parameters() -> List[Dict]:
    """Systematically tune QFH parameters"""
    results = []
    
    # Parameter ranges based on TASK.md recommendations
    k1_values = [0.2, 0.3, 0.4, 0.5, 0.6, 0.7]  # Entropy sensitivity
    k2_values = [0.1, 0.2, 0.3, 0.4, 0.5]        # Coherence sensitivity
    trajectory_weights = [0.2, 0.3, 0.4, 0.5]    # Trajectory vs pattern blend
    
    total_configs = len(k1_values) * len(k2_values) * len(trajectory_weights)
    current_config = 0
    
    print(f"Starting QFH parameter tuning with {total_configs} configurations...")
    print("Target: >45% accuracy on both overall and high-confidence signals")
    print("=" * 70)
    
    for k1 in k1_values:
        for k2 in k2_values:
            for traj_weight in trajectory_weights:
                current_config += 1
                
                print(f"Config {current_config}/{total_configs}: k1={k1:.1f}, k2={k2:.1f}, traj_weight={traj_weight:.1f}")
                
                # Modify parameters
                modify_qfh_parameters(k1, k2, traj_weight)
                
                # Build system
                if not build_system():
                    print("  Build failed - skipping")
                    continue
                
                # Run test
                metrics = run_test()
                
                # Store result
                result = {
                    'k1': k1,
                    'k2': k2,
                    'trajectory_weight': traj_weight,
                    'overall_accuracy': metrics['overall_accuracy'],
                    'high_conf_accuracy': metrics['high_conf_accuracy'],
                    'high_conf_rate': metrics['high_conf_rate'],
                    'score': metrics['overall_accuracy'] + metrics['high_conf_accuracy']  # Combined score
                }
                results.append(result)
                
                print(f"  Overall: {metrics['overall_accuracy']:.2f}%, High-Conf: {metrics['high_conf_accuracy']:.2f}% ({metrics['high_conf_rate']:.1f}%)")
                
                # Check if we hit target
                if metrics['overall_accuracy'] > 45.0 and metrics['high_conf_accuracy'] > 45.0:
                    print(f"  *** TARGET ACHIEVED! ***")
    
    return results

def find_best_parameters(results: List[Dict]) -> Dict:
    """Find the best performing parameter combination"""
    if not results:
        return {}
    
    # Sort by combined score (overall + high-conf accuracy)
    best = max(results, key=lambda x: x['score'])
    return best

def save_results(results: List[Dict], filename: str = '/sep/qfh_tuning_results.json'):
    """Save tuning results to file"""
    with open(filename, 'w') as f:
        json.dump(results, f, indent=2)

def main():
    """Main tuning workflow"""
    print("QFH Parameter Tuning - Phase 1 Priority")
    print("Based on TASK.md recommendations")
    print()
    
    # Run systematic tuning
    results = tune_parameters()
    
    if not results:
        print("No valid results obtained")
        return
    
    # Find best configuration
    best = find_best_parameters(results)
    
    print("\n" + "=" * 70)
    print("TUNING COMPLETE")
    print("=" * 70)
    print(f"Best Configuration:")
    print(f"  k1 (entropy weight): {best['k1']:.1f}")
    print(f"  k2 (coherence weight): {best['k2']:.1f}")
    print(f"  Trajectory weight: {best['trajectory_weight']:.1f}")
    print(f"  Overall accuracy: {best['overall_accuracy']:.2f}%")
    print(f"  High-conf accuracy: {best['high_conf_accuracy']:.2f}%")
    print(f"  High-conf rate: {best['high_conf_rate']:.1f}%")
    
    # Apply best configuration
    print(f"\nApplying best configuration...")
    modify_qfh_parameters(best['k1'], best['k2'], best['trajectory_weight'])
    
    if build_system():
        print("Build successful - best parameters applied")
    else:
        print("Build failed - please check configuration")
    
    # Save results
    save_results(results)
    print(f"Results saved to qfh_tuning_results.json")
    
    # Show improvement
    baseline_accuracy = 41.35  # From TASK.md
    improvement = best['overall_accuracy'] - baseline_accuracy
    print(f"\nImprovement over baseline: {improvement:+.2f}%")

if __name__ == "__main__":
    main()
