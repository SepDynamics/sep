#!/usr/bin/env python3
"""
Systematic Weight Tuning Script for SEP Engine
Optimizes the balance between stability, coherence, and entropy in signal scoring.
"""

import subprocess
import re
import time
import numpy as np

PME_TESTBED_PATH = "/sep/examples/pme_testbed_phase2.cpp"

def modify_scoring_weights(stability_w: float, coherence_w: float, entropy_w: float):
    """Modifies the scoring weights in the C++ source file."""
    with open(PME_TESTBED_PATH, 'r') as f:
        content = f.read()

    # Use regex to replace the weight definitions
    content = re.sub(r"double stability_w = [0-9.]+;", f"double stability_w = {stability_w:.2f};", content)
    content = re.sub(r"double coherence_w = [0-9.]+;", f"double coherence_w = {coherence_w:.2f};", content)
    content = re.sub(r"double entropy_w = [0-9.]+;", f"double entropy_w = {entropy_w:.2f};", content)

    with open(PME_TESTBED_PATH, 'w') as f:
        f.write(content)

def run_backtest():
    """Builds and runs the backtest, then parses the results."""
    # Build the system
    build_proc = subprocess.run(['./build.sh'], capture_output=True, text=True, cwd='/sep')
    if build_proc.returncode != 0:
        print("  [ERROR] Build failed. Skipping this configuration.")
        print(build_proc.stderr)
        return None

    # Run the test
    test_proc = subprocess.run(
        ['./build/examples/pme_testbed_phase2', 'Testing/OANDA/O-test-2.json'],
        capture_output=True, text=True, cwd='/sep'
    )
    output = test_proc.stdout + test_proc.stderr

    # Parse results
    try:
        overall = float(re.search(r'Overall Accuracy: ([\d.]+)%', output).group(1))
        high_conf = float(re.search(r'High Confidence Accuracy: ([\d.]+)%', output).group(1))
        high_conf_rate = float(re.search(r'High Confidence Signals: \d+ \(([\d.]+)%\)', output).group(1))
        return {"overall": overall, "high_conf": high_conf, "rate": high_conf_rate}
    except (AttributeError, IndexError):
        print("  [ERROR] Could not parse output. Skipping.")
        return None

def main():
    print("🚀 Starting Systematic Weight Optimization...")
    
    # Define parameter grid
    weight_steps = np.arange(0.1, 0.9, 0.1)
    best_score = -1
    best_weights = {}
    results_log = []

    # Keep the original file content
    with open(PME_TESTBED_PATH, 'r') as f:
        original_content = f.read()

    try:
        for s_w in weight_steps:
            for c_w in weight_steps:
                e_w = 1.0 - s_w - c_w
                if e_w >= 0.05: # Ensure entropy has at least some weight
                    s_w, c_w, e_w = round(s_w, 2), round(c_w, 2), round(e_w, 2)
                    print(f"\n🧪 Testing weights: [S:{s_w}, C:{c_w}, E:{e_w}]")
                    
                    modify_scoring_weights(s_w, c_w, e_w)
                    
                    metrics = run_backtest()
                    if metrics:
                        score = (metrics['high_conf'] * 0.7) + (metrics['overall'] * 0.2) + (metrics['rate'] * 0.1)
                        print(f"  📊 Results -> Overall: {metrics['overall']:.2f}% | High-Conf: {metrics['high_conf']:.2f}% | Rate: {metrics['rate']:.1f}% | Score: {score:.2f}")
                        
                        result_entry = {"s_w": s_w, "c_w": c_w, "e_w": e_w, **metrics, "score": score}
                        results_log.append(result_entry)

                        if score > best_score:
                            best_score = score
                            best_weights = result_entry
                            print(f"  🏆 New Best Score!")

    finally:
        # Restore original file
        with open(PME_TESTBED_PATH, 'w') as f:
            f.write(original_content)
        print("\n✅ Restored original source file.")

    print("\n" + "="*50)
    print("🏁 Optimization Complete 🏁")
    print("="*50)
    
    if best_weights:
        print("\n🏆 Best Configuration Found:")
        print(f"   Weights (S/C/E): {best_weights['s_w']} / {best_weights['c_w']} / {best_weights['e_w']}")
        print(f"   High-Confidence Accuracy: {best_weights['high_conf']:.2f}%")
        print(f"   Overall Accuracy: {best_weights['overall']:.2f}%")
        print(f"   Signal Rate: {best_weights['rate']:.1f}%")
        print(f"   Performance Score: {best_weights['score']:.2f}")
        
        # Write summary to file
        with open("/sep/weight_optimization_results.txt", "w") as f:
            f.write(f"Best Weights: S={best_weights['s_w']}, C={best_weights['c_w']}, E={best_weights['e_w']}\n")
            f.write(f"Performance: {best_weights['high_conf']:.2f}% high-conf, {best_weights['overall']:.2f}% overall\n")
    else:
        print("No successful configurations were found.")
        
if __name__ == "__main__":
    main()
