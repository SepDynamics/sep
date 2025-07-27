import subprocess
import numpy as np

def run_testbed(stability_w, coherence_w, entropy_w, buy_score_threshold, sell_score_threshold):
    """Runs the pme_testbed executable with the given weights and returns the accuracy."""
    command = [
        './build/examples/pme_testbed',
        'assets/test_data/eur_usd_m1_48h.json',
        str(stability_w),
        str(coherence_w),
        str(entropy_w),
        str(buy_score_threshold),
        str(sell_score_threshold)
    ]
    result = subprocess.run(command, capture_output=True, text=True)
    
    for line in result.stdout.strip().split('\n'):
        if "Accuracy" in line:
            try:
                accuracy = float(line.split(': ')[1].replace('%', ''))
                return accuracy
            except (IndexError, ValueError):
                return 0.0
    return 0.0

def main():
    """
    Performs a grid search to find the optimal signal weights for the pme_testbed.
    """
    weight_range = np.arange(0.1, 1.0, 0.1)
    buy_score_threshold_range = np.arange(0.5, 0.95, 0.05)
    sell_score_threshold_range = np.arange(0.5, 0.95, 0.05)

    best_accuracy = 0.0
    best_params = {}

    for stability_w in weight_range:
        for coherence_w in weight_range:
            for entropy_w in weight_range:
                for buy_score_threshold in buy_score_threshold_range:
                    for sell_score_threshold in sell_score_threshold_range:
                        accuracy = run_testbed(stability_w, coherence_w, entropy_w, buy_score_threshold, sell_score_threshold)
                        print(f"Testing with stability_w={stability_w:.2f}, coherence_w={coherence_w:.2f}, entropy_w={entropy_w:.2f}, buy_score_threshold={buy_score_threshold:.2f}, sell_score_threshold={sell_score_threshold:.2f} -> Accuracy: {accuracy:.2f}%")
                        
                        if accuracy > best_accuracy:
                            best_accuracy = accuracy
                            best_params = {
                                'stability_w': stability_w,
                                'coherence_w': coherence_w,
                                'entropy_w': entropy_w,
                                'buy_score_threshold': buy_score_threshold,
                                'sell_score_threshold': sell_score_threshold
                            }

    print("\n--- Optimal Configuration ---")
    print(f"Best Accuracy: {best_accuracy:.2f}%")
    print("Parameters:")
    for param, value in best_params.items():
        print(f"  {param}: {value:.2f}")
    print("---------------------------")

if __name__ == "__main__":
    main()