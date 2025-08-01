#!/usr/bin/env python3
"""
Quick QFH Parameter Tuning - Test key parameter combinations
Based on TASK.md Priority 1 recommendations.
"""

import subprocess
import re
import time

def modify_qfh_parameters(k1: float, k2: float, trajectory_weight: float = 0.3):
    """Modify the QFH parameters in qfh.cpp"""
    qfh_path = "/sep/src/quantum/bitspace/qfh.cpp"
    
    # Read the file
    with open(qfh_path, 'r') as f:
        content = f.read()
    
    # Replace k1 parameter
    content = re.sub(
        r'const double k1 = [0-9.]+;',
        f'const double k1 = {k1:.2f};',
        content
    )
    
    # Replace k2 parameter
    content = re.sub(
        r'const double k2 = [0-9.]+;',
        f'const double k2 = {k2:.2f};',
        content
    )
    
    # Replace trajectory weight
    pattern_weight = 1.0 - trajectory_weight
    content = re.sub(
        r'result\.coherence = [0-9.]+f \* trajectory_coherence \+ [0-9.]+f \* pattern_coherence;',
        f'result.coherence = {trajectory_weight:.1f}f * trajectory_coherence + {pattern_weight:.1f}f * pattern_coherence;',
        content
    )
    
    # Write back
    with open(qfh_path, 'w') as f:
        f.write(content)

def run_test():
    """Run the testbed and extract accuracy metrics"""
    result = subprocess.run([
        './build/examples/pme_testbed_phase2', 
        'Testing/OANDA/O-test-2.json'
    ], capture_output=True, text=True, cwd='/sep')
    
    if result.returncode != 0:
        return {'overall_accuracy': 0.0, 'high_conf_accuracy': 0.0}
    
    output = result.stdout + result.stderr
    
    # Extract accuracy metrics from the end summary
    overall_match = re.search(r'Overall Accuracy: ([0-9.]+)%', output)
    high_conf_match = re.search(r'High Confidence Accuracy: ([0-9.]+)%', output)
    
    metrics = {
        'overall_accuracy': float(overall_match.group(1)) if overall_match else 0.0,
        'high_conf_accuracy': float(high_conf_match.group(1)) if high_conf_match else 0.0,
    }
    
    return metrics

def test_configurations():
    """Test key parameter configurations"""
    # Key configurations to test based on TASK.md
    configs = [
        {'k1': 0.3, 'k2': 0.2, 'traj_weight': 0.3, 'name': 'Current baseline'},
        {'k1': 0.4, 'k2': 0.3, 'traj_weight': 0.4, 'name': 'Higher sensitivity'},
        {'k1': 0.2, 'k2': 0.1, 'traj_weight': 0.2, 'name': 'Lower sensitivity'},
        {'k1': 0.5, 'k2': 0.2, 'traj_weight': 0.3, 'name': 'Higher entropy weight'},
        {'k1': 0.3, 'k2': 0.4, 'traj_weight': 0.3, 'name': 'Higher coherence weight'},
        {'k1': 0.3, 'k2': 0.2, 'traj_weight': 0.5, 'name': 'Higher trajectory weight'},
    ]
    
    print("Quick QFH Parameter Tuning")
    print("Target: >45% accuracy on both overall and high-confidence signals")
    print("=" * 70)
    
    results = []
    
    for i, config in enumerate(configs, 1):
        print(f"Config {i}/{len(configs)}: {config['name']}")
        print(f"  k1={config['k1']:.1f}, k2={config['k2']:.1f}, traj_weight={config['traj_weight']:.1f}")
        
        # Modify parameters
        modify_qfh_parameters(config['k1'], config['k2'], config['traj_weight'])
        
        # Build system
        build_result = subprocess.run(['./build.sh'], 
                                    capture_output=True, 
                                    text=True,
                                    cwd='/sep')
        
        if build_result.returncode != 0:
            print("  Build failed - skipping")
            continue
        
        # Run test
        metrics = run_test()
        
        # Store result
        result = {
            **config,
            **metrics,
            'score': metrics['overall_accuracy'] + metrics['high_conf_accuracy']
        }
        results.append(result)
        
        print(f"  Overall: {metrics['overall_accuracy']:.2f}%, High-Conf: {metrics['high_conf_accuracy']:.2f}%")
        
        # Check if we hit target
        if metrics['overall_accuracy'] > 45.0 and metrics['high_conf_accuracy'] > 45.0:
            print(f"  *** TARGET ACHIEVED! ***")
        
        print()
    
    return results

def main():
    print("Starting quick QFH parameter tuning...")
    start_time = time.time()
    
    results = test_configurations()
    
    if not results:
        print("No valid results obtained")
        return
    
    # Find best configuration
    best = max(results, key=lambda x: x['score'])
    
    print("=" * 70)
    print("TUNING RESULTS")
    print("=" * 70)
    
    print("\nAll Configurations:")
    for result in sorted(results, key=lambda x: x['score'], reverse=True):
        print(f"{result['name']:25} | Overall: {result['overall_accuracy']:5.2f}% | High-Conf: {result['high_conf_accuracy']:5.2f}% | Score: {result['score']:5.2f}")
    
    print(f"\nBest Configuration: {best['name']}")
    print(f"  k1 (entropy weight): {best['k1']:.1f}")
    print(f"  k2 (coherence weight): {best['k2']:.1f}")
    print(f"  Trajectory weight: {best['traj_weight']:.1f}")
    print(f"  Overall accuracy: {best['overall_accuracy']:.2f}%")
    print(f"  High-conf accuracy: {best['high_conf_accuracy']:.2f}%")
    
    # Apply best configuration
    print(f"\nApplying best configuration...")
    modify_qfh_parameters(best['k1'], best['k2'], best['traj_weight'])
    
    # Build with best config
    build_result = subprocess.run(['./build.sh'], 
                                capture_output=True, 
                                text=True,
                                cwd='/sep')
    
    if build_result.returncode == 0:
        print("Build successful - best parameters applied")
    else:
        print("Build failed - please check configuration")
    
    # Show improvement
    baseline_accuracy = 41.35  # From TASK.md
    improvement = best['overall_accuracy'] - baseline_accuracy
    print(f"\nImprovement over baseline: {improvement:+.2f}%")
    
    elapsed = time.time() - start_time
    print(f"Tuning completed in {elapsed:.1f} seconds")

if __name__ == "__main__":
    main()
