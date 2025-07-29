import os
import subprocess
import sys
import json

import pandas as pd
import numpy as np
from performance_metrics import sharpe_ratio
from pathlib import Path
import shutil
import tempfile
from _sep.testbed.json_utils import parse_first_json

# Removed parse_metrics_from_stream function - now using direct JSON parsing

def create_chunks(input_file, num_chunks, output_dir):
    """Splits the input file into a specified number of chunks."""
    input_path = Path(input_file)
    output_path = Path(output_dir)
    if output_path.exists():
        shutil.rmtree(output_path)
    output_path.mkdir(parents=True)

    file_size = input_path.stat().st_size
    chunk_size = file_size // num_chunks
    
    chunk_paths = []
    with open(input_path, 'rb') as f:
        for i in range(num_chunks):
            chunk_data = f.read(chunk_size)
            if not chunk_data:
                break
            
            if i == num_chunks - 1:
                remainder = f.read()
                if remainder:
                    chunk_data += remainder

            chunk_file_path = output_path / f"chunk_{i:04d}.bin"
            with open(chunk_file_path, 'wb') as chunk_f:
                chunk_f.write(chunk_data)
            chunk_paths.append(chunk_file_path)
    
    return sorted(chunk_paths) # Ensure order is correct

def analyze_chunks_statefully(chunk_list, temp_dir):
    """Runs the metric engine statefully over a list of chunks."""
    # For stateful processing, we need to process all chunks through a single file
    # Concatenate all chunks into a single file
    combined_file = temp_dir / "combined_data.bin"
    
    with open(combined_file, 'wb') as outf:
        for chunk_path in chunk_list:
            with open(chunk_path, 'rb') as inf:
                outf.write(inf.read())

    executable_path = Path("./examples/pattern_metric_example")
    cmd = [str(executable_path), str(combined_file), "--json"]
    
    result = subprocess.run(cmd, capture_output=True, text=True, check=True)
    return parse_first_json(result.stdout)

def run_backtest(metrics_data):
    """Runs the financial_backtest.py script on a list of metrics."""
    if not metrics_data:
        print("No metrics data to backtest.")
        return 0.0

    # Use a temporary file for the backtest
    with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as tmp:
        json.dump(metrics_data, tmp)
        tmp_path = tmp.name

    cmd = ["python3", "./financial_backtest.py", tmp_path]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        # Look for "Annualized Alpha: X.XX%" in the output
        import re
        alpha_match = re.search(r"Annualized Alpha:\s+([-\d\.]+)%", result.stdout)
        if alpha_match:
            return float(alpha_match.group(1))
    except subprocess.CalledProcessError as e:
        print(f"Backtest failed. STDERR:\n{e.stderr}")
    finally:
        os.unlink(tmp_path) # Clean up the temp file
    
    return 0.0

def main():
    import argparse
    parser = argparse.ArgumentParser(description='Run alpha prediction experiment')
    parser.add_argument('--input', type=str, help='Input metrics JSON file from pattern_metric_example')
    parser.add_argument('--output', type=str, help='Output JSON file for alpha results')
    args = parser.parse_args()
    
    # If --input is provided, use direct metrics instead of generating them
    if args.input and Path(args.input).exists():
        print(f"--- Using pre-generated metrics from {args.input} ---")
        with open(args.input, 'r') as f:
            metrics_data = json.load(f)
        
        alpha = run_backtest(metrics_data)
        result = {
            "baseline_alpha": alpha,
            "total_return_alpha": alpha,
            "sharpe_ratio": sharpe_ratio([m.get('pnl', 0.0) for m in metrics_data]),
            "experiment_type": "direct_metrics"
        }
        
        if args.output:
            with open(args.output, 'w') as f:
                json.dump(result, f, indent=2)
            print(f"Results saved to {args.output}")
        else:
            print(f"Alpha: {alpha:.4f}%")
        return
    
    # --- Setup for full experiment ---
    train_file = Path("Testing/OANDA/O-train-1.json")
    test_file = Path("Testing/OANDA/O-test-2.json")
    num_chunks = 10 # Use 10 chunks for faster processing
    
    print("--- Preparing Data Chunks ---")
    train_chunks = create_chunks(train_file, num_chunks, Path("./temp_train_chunks"))
    test_chunks = create_chunks(test_file, num_chunks, Path("./temp_test_chunks"))

    # --- 1. Establish Baseline ---
    print("\n--- Establishing Baseline Alpha on Test Data ---")
    with tempfile.TemporaryDirectory() as temp_dir:
        baseline_metrics = analyze_chunks_statefully(test_chunks, Path(temp_dir))
        baseline_alpha = run_backtest(baseline_metrics)
    print(f"Baseline Alpha (untrained model): {baseline_alpha:.4f}%")

    # --- 2. Iterative Training ---
    print("\n--- Starting Iterative Training and Evaluation ---")
    training_iterations = [1, 2, 5]
    results = []

    for i in training_iterations:
        print(f"\n--- Evaluating after {i} training iteration(s) ---")
        
        # Create the full processing list: N passes over train chunks, then 1 pass over test chunks
        processing_list = (train_chunks * i) + test_chunks
        
        with tempfile.TemporaryDirectory() as temp_dir:
            all_metrics = analyze_chunks_statefully(processing_list, Path(temp_dir))
            
            # We only care about the metrics from the test set
            test_metrics = all_metrics[-len(test_chunks):]
            
            alpha = run_backtest(test_metrics)
            print(f"Alpha after {i} training iteration(s): {alpha:.4f}%")
            results.append({"iterations": i, "alpha": alpha})

    # --- 3. Report Results ---
    print("\n--- Experiment Complete ---")
    print(f"Baseline Alpha: {baseline_alpha:.4f}%")
    
    results_df = pd.DataFrame(results)
    print("\nAlpha Improvement Curve:")
    print(results_df)

    # --- Save results if output specified ---
    final_result = {
        "baseline_alpha": baseline_alpha,
        "total_return_alpha": results[-1]["alpha"] if results else baseline_alpha,
        "sharpe_ratio": sharpe_ratio([r["alpha"] for r in results]) if results else 0.0,
        "training_iterations": results,
        "experiment_type": "full_experiment"
    }
    
    if args.output:
        with open(args.output, 'w') as f:
            json.dump(final_result, f, indent=2)
        print(f"Results saved to {args.output}")

    # --- Cleanup ---
    if Path("./temp_train_chunks").exists():
        shutil.rmtree("./temp_train_chunks")
    if Path("./temp_test_chunks").exists():
        shutil.rmtree("./temp_test_chunks")
    print("\nCleaned up temporary chunk directories.")

if __name__ == "__main__":
    main()