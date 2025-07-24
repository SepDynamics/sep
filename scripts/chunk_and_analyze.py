import os
import subprocess
import sys
import json
import re
from pathlib import Path

def parse_metrics(output):
    """Parses the output of the pattern_metric_example tool and returns a dict."""
    metrics = {}
    try:
        coherence_match = re.search(r"Average Coherence:\s+([\d\.]+)", output)
        stability_match = re.search(r"Average Stability:\s+([\d\.]+)", output)
        entropy_match = re.search(r"Average Entropy:\s+([\d\.]+)", output)
        patterns_match = re.search(r"Total Patterns:\s+(\d+)", output)

        if coherence_match:
            metrics['coherence'] = float(coherence_match.group(1))
        if stability_match:
            metrics['stability'] = float(stability_match.group(1))
        if entropy_match:
            metrics['entropy'] = float(entropy_match.group(1))
        if patterns_match:
            metrics['patterns'] = int(patterns_match.group(1))
            
    except Exception as e:
        print(f"Error parsing metrics: {e}\nOutput was:\n{output}")
    return metrics

def analyze_chunk(executable_path, chunk_path):
    """Runs the pattern_metric_example on a single chunk file."""
    cmd = [str(executable_path), str(chunk_path)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error processing {chunk_path}: {result.stderr}")
        return None
    return parse_metrics(result.stdout)

def create_chunks(input_file, num_chunks, output_dir):
    """Splits the input file into a specified number of chunks."""
    input_path = Path(input_file)
    output_path = Path(output_dir)
    output_path.mkdir(parents=True, exist_ok=True)

    file_size = input_path.stat().st_size
    chunk_size = file_size // num_chunks
    
    print(f"Input file size: {file_size} bytes")
    print(f"Creating {num_chunks} chunks of ~{chunk_size} bytes each in {output_dir}")

    chunk_paths = []
    with open(input_path, 'rb') as f:
        for i in range(num_chunks):
            print(f"  Creating chunk {i+1}/{num_chunks}...")
            chunk_data = f.read(chunk_size)
            if not chunk_data:
                break
            
            # Handle the last chunk to include the remainder
            if i == num_chunks - 1:
                remainder = f.read()
                if remainder:
                    chunk_data += remainder

            chunk_file_path = output_path / f"chunk_{i:04d}.bin"
            with open(chunk_file_path, 'wb') as chunk_f:
                chunk_f.write(chunk_data)
            chunk_paths.append(chunk_file_path)
    
    print(f"Created {len(chunk_paths)} chunks.")
    return chunk_paths

def main():
    if len(sys.argv) != 3:
        print("Usage: python chunk_and_analyze.py <input_file> <num_chunks>")
        sys.exit(1)

    input_file = sys.argv[1]
    num_chunks = int(sys.argv[2])
    
    executable_path = Path("./build/examples/pattern_metric_example")
    if not executable_path.exists():
        print(f"Error: Executable not found at {executable_path}")
        print("Please build the project first using ./build.sh")
        sys.exit(1)

    output_dir = Path(f"./temp_chunks_{num_chunks}")
    
    # 1. Create chunks
    chunk_files = create_chunks(input_file, num_chunks, output_dir)

    # 2. Analyze each chunk
    all_metrics = {}
    for i, chunk_path in enumerate(chunk_files):
        print(f"Analyzing chunk {i+1}/{len(chunk_files)}: {chunk_path.name}...")
        metrics = analyze_chunk(executable_path, chunk_path)
        if metrics:
            all_metrics[str(chunk_path)] = metrics

    # 3. Save results
    results_file = Path(f"docs/proofs/poc_4_results_{num_chunks}_chunks.json")
    results_file.parent.mkdir(exist_ok=True)
    with open(results_file, 'w') as f:
        json.dump(all_metrics, f, indent=2)
    print(f"\nAnalysis complete. Results for {num_chunks} chunks saved to {results_file}")

    # 4. Clean up
    for chunk_path in chunk_files:
        chunk_path.unlink()
    output_dir.rmdir()
    print(f"Cleaned up temporary directory: {output_dir}")


if __name__ == "__main__":
    main()