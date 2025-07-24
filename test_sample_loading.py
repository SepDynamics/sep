#!/usr/bin/env python3
"""
Test script to verify the sample data can be loaded by the SEP workbench
"""

import subprocess
import time
import signal
import sys

def test_sample_loading():
    print("Testing SEP workbench sample data loading...")
    
    # Start the workbench process
    try:
        proc = subprocess.Popen(
            ["./build/src/apps/workbench/sep_workbench"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            cwd="/sep",
            text=True,
            bufsize=1
        )
        
        sample_data_loaded = False
        sep_metrics_calculated = False
        
        # Read output for a few seconds to see initialization
        start_time = time.time()
        timeout = 10  # 10 seconds timeout
        
        while time.time() - start_time < timeout:
            line = proc.stdout.readline()
            if not line:
                break
                
            print(f"[WORKBENCH] {line.strip()}")
            
            # Check for our key messages
            if "Successfully loaded" in line and "sample candles" in line:
                sample_data_loaded = True
                print("✅ Sample data loading detected!")
                
            if "SEP engine calculated metrics" in line or "SEP engine processing complete" in line:
                sep_metrics_calculated = True
                print("✅ SEP metrics calculation detected!")
                
            if ("pattern_pattern_" in line and ("_coherence" in line or "_stability" in line or "_entropy" in line)):
                if not sep_metrics_calculated:  # Only print this once
                    sep_metrics_calculated = True
                    print("✅ Quantum pattern metrics detected!")
                
            if "Average Coherence:" in line or "Average Stability:" in line or "Average Entropy:" in line:
                print("✅ System-wide quantum metrics detected!")
                
            # Break early if we got what we need
            if sample_data_loaded and sep_metrics_calculated:
                break
        
        # Terminate the process
        proc.terminate()
        try:
            proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            proc.kill()
            proc.wait()
            
        print(f"\nTest Results:")
        print(f"Sample data loaded: {'✅ YES' if sample_data_loaded else '❌ NO'}")
        print(f"SEP metrics calculated: {'✅ YES' if sep_metrics_calculated else '❌ NO'}")
        
        return sample_data_loaded and sep_metrics_calculated
        
    except Exception as e:
        print(f"Error running test: {e}")
        return False

if __name__ == "__main__":
    success = test_sample_loading()
    sys.exit(0 if success else 1)
