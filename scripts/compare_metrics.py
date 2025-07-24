import json
from pathlib import Path
import numpy as np

def load_results(filepath):
    """Loads a JSON results file and returns the data."""
    path = Path(filepath)
    if not path.exists():
        print(f"Error: Results file not found at {path}")
        return None
    with open(path, 'r') as f:
        return json.load(f)

def average_metrics(metric_list):
    """Averages a list of metric dictionaries."""
    if not metric_list:
        return {}
    
    avg_coherence = np.mean([m['coherence'] for m in metric_list if 'coherence' in m])
    avg_stability = np.mean([m['stability'] for m in metric_list if 'stability' in m])
    avg_entropy = np.mean([m['entropy'] for m in metric_list if 'entropy' in m])
    total_patterns = np.sum([m['patterns'] for m in metric_list if 'patterns' in m])
    
    return {
        "coherence": avg_coherence,
        "stability": avg_stability,
        "entropy": avg_entropy,
        "patterns": total_patterns
    }

def main():
    results_20_path = Path("docs/proofs/poc_4_results_20_chunks.json")
    results_100_path = Path("docs/proofs/poc_4_results_100_chunks.json")

    results_20 = load_results(results_20_path)
    results_100 = load_results(results_100_path)

    if not results_20 or not results_100:
        sys.exit(1)

    # Sort the results by chunk name to ensure correct order
    results_100_list = [results_100[key] for key in sorted(results_100.keys())]
    results_20_list = [results_20[key] for key in sorted(results_20.keys())]
    
    print("--- Metric Compositionality Analysis ---")
    print(f"Comparing {len(results_20_list)} large chunks against {len(results_100_list)} small chunks.\n")

    group_size = 5 # 5 small chunks should equal 1 large chunk (100 / 20 = 5)

    for i in range(len(results_20_list)):
        start_index = i * group_size
        end_index = start_index + group_size
        
        small_chunk_group = results_100_list[start_index:end_index]
        
        if not small_chunk_group:
            continue

        # Combine the metrics of the 5 small chunks
        combined_metrics = average_metrics(small_chunk_group)
        
        # Get the metrics of the corresponding large chunk
        large_chunk_metrics = results_20_list[i]

        print(f"--- Comparing Large Chunk {i} with Small Chunks {start_index}-{end_index-1} ---")
        print(f"Large Chunk Metrics:  Coherence={large_chunk_metrics.get('coherence', 0):.4f}, Patterns={large_chunk_metrics.get('patterns', 0)}")
        print(f"Combined 5-Chunk Avg: Coherence={combined_metrics.get('coherence', 0):.4f}, Patterns={combined_metrics.get('patterns', 0)}")
        
        coherence_diff = abs(large_chunk_metrics.get('coherence', 0) - combined_metrics.get('coherence', 0))
        pattern_diff = abs(large_chunk_metrics.get('patterns', 0) - combined_metrics.get('patterns', 0))
        
        print(f"  -> Coherence Difference: {coherence_diff:.4f}")
        print(f"  -> Pattern Count Difference: {pattern_diff}\n")

if __name__ == "__main__":
    main()