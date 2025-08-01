#!/usr/bin/env python3
"""
Systematic Threshold Tuning Script for SEP Engine
Finds the optimal confidence and coherence thresholds to maximize profitability.

Profitability Score = (High-Conf Accuracy - 50) * High-Conf Rate
This metric directly models trading profitability.
"""
import subprocess
import re
import numpy as np

PME_TESTBED_PATH = "/sep/examples/pme_testbed_phase2.cpp"

def modify_thresholds(confidence_t: float, coherence_t: float):
    """Modifies the filtering thresholds in the C++ source file."""
    with open(PME_TESTBED_PATH, 'r') as f:
        content = f.read()

    # Find and replace confidence_threshold
    content = re.sub(r"double confidence_threshold = [0-9.]+;", 
                     f"double confidence_threshold = {confidence_t:.2f};", content)
    
    # Find and replace coherence_threshold  
    content = re.sub(r"double coherence_threshold = [0-9.]+;", 
                     f"double coherence_threshold = {coherence_t:.2f};", content)

    with open(PME_TESTBED_PATH, 'w') as f:
        f.write(content)

def run_backtest():
    """Builds and runs the backtest, then parses the results."""
    # Build the project
    build_proc = subprocess.run(['./build.sh'], capture_output=True, text=True, cwd='/sep')
    if build_proc.returncode != 0:
        print(f"Build failed: {build_proc.stderr}")
        return None

    # Run the backtest
    test_proc = subprocess.run(
        ['./build/examples/pme_testbed_phase2', 'Testing/OANDA/O-test-2.json'],
        capture_output=True, text=True, cwd='/sep'
    )
    output = test_proc.stdout + test_proc.stderr

    try:
        # Parse high-confidence accuracy
        high_conf_match = re.search(r'High Confidence Accuracy: ([\d.]+)%', output)
        high_conf = float(high_conf_match.group(1)) if high_conf_match else 0.0
        
        # Parse high-confidence signal rate
        rate_match = re.search(r'High Confidence Signals: \d+ \(([\d.]+)%\)', output)
        high_conf_rate = float(rate_match.group(1)) if rate_match else 0.0
        
        return {"high_conf": high_conf, "rate": high_conf_rate}
    except (AttributeError, ValueError):
        print(f"Failed to parse output: {output[-500:]}")  # Show last 500 chars for debugging
        return None

def main():
    print("🚀 Starting Systematic Threshold Optimization...")
    print("   Objective: Maximize Profitability Score = (High-Conf Accuracy - 50) * Signal Rate")
    
    # Define search space
    conf_thresholds = np.arange(0.50, 0.75, 0.05)  # 0.50 to 0.70 in steps of 0.05
    coh_thresholds = np.arange(0.30, 0.65, 0.05)   # 0.30 to 0.60 in steps of 0.05
    
    best_score = -1000
    best_thresholds = {}
    results = []
    
    # Save original content
    with open(PME_TESTBED_PATH, 'r') as f:
        original_content = f.read()

    try:
        total_tests = len(conf_thresholds) * len(coh_thresholds)
        test_count = 0
        
        for conf_t in conf_thresholds:
            for coh_t in coh_thresholds:
                test_count += 1
                conf_t, coh_t = round(conf_t, 2), round(coh_t, 2)
                
                print(f"\n🧪 Test {test_count}/{total_tests}: [Conf ≥ {conf_t}, Coh ≥ {coh_t}]")
                
                modify_thresholds(conf_t, coh_t)
                metrics = run_backtest()
                
                if metrics and metrics['rate'] > 0:
                    score = (metrics['high_conf'] - 50) * metrics['rate']
                    results.append({
                        'conf_t': conf_t, 'coh_t': coh_t, 
                        'accuracy': metrics['high_conf'], 'rate': metrics['rate'], 'score': score
                    })
                    
                    print(f"  📊 High-Conf Acc: {metrics['high_conf']:.2f}% | Rate: {metrics['rate']:.1f}% | Score: {score:.2f}")
                    
                    if score > best_score:
                        best_score = score
                        best_thresholds = {"conf_t": conf_t, "coh_t": coh_t, **metrics, "score": score}
                        print(f"  🏆 NEW BEST PROFITABILITY SCORE!")
                else:
                    print(f"  ❌ Failed to parse results or zero signal rate")
                    
    finally:
        # Restore original file
        with open(PME_TESTBED_PATH, 'w') as f:
            f.write(original_content)
        print("\n✅ Restored original source file.")

    print("\n" + "="*60)
    print("🏁 THRESHOLD OPTIMIZATION COMPLETE 🏁")
    print("="*60)
    
    if best_thresholds:
        print("\n🏆 OPTIMAL CONFIGURATION FOUND:")
        print(f"   Confidence Threshold: {best_thresholds['conf_t']:.2f}")
        print(f"   Coherence Threshold: {best_thresholds['coh_t']:.2f}")
        print(f"   High-Confidence Accuracy: {best_thresholds['high_conf']:.2f}%")
        print(f"   Signal Rate: {best_thresholds['rate']:.1f}%")
        print(f"   Profitability Score: {best_thresholds['score']:.2f}")
        
        print(f"\n📋 TOP 5 CONFIGURATIONS:")
        top_5 = sorted(results, key=lambda x: x['score'], reverse=True)[:5]
        for i, config in enumerate(top_5, 1):
            print(f"   {i}. Conf: {config['conf_t']:.2f}, Coh: {config['coh_t']:.2f} → "
                  f"Acc: {config['accuracy']:.2f}%, Rate: {config['rate']:.1f}%, Score: {config['score']:.2f}")
        
        print(f"\n💡 RECOMMENDATION:")
        print(f"   Update pme_testbed_phase2.cpp with these optimal thresholds:")
        print(f"   double confidence_threshold = {best_thresholds['conf_t']:.2f};")
        print(f"   double coherence_threshold = {best_thresholds['coh_t']:.2f};")
    else:
        print("❌ No profitable configurations were found.")
        
if __name__ == "__main__":
    main()
