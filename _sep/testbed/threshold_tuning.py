import argparse
import numpy as np
from typing import Tuple, Dict
from backtest_compare import load_dataset, run_backtest


def parse_range(arg: str) -> Tuple[float, float, float]:
    start, end, step = [float(x) for x in arg.split(',')]
    return start, end, step


def main() -> None:
    parser = argparse.ArgumentParser(description="Grid search threshold tuning")
    parser.add_argument("data", help="Path to pme_testbed.csv")
    parser.add_argument("--conf-range", default="0.6,0.9,0.05")
    parser.add_argument("--coh-range", default="0.6,0.9,0.05")
    parser.add_argument("--stab-range", default="0.0,0.5,0.05")
    args = parser.parse_args()

    df = load_dataset(args.data)

    conf_range = parse_range(args.conf_range)
    coh_range = parse_range(args.coh_range)
    stab_range = parse_range(args.stab_range)

    best_pips = -1e9
    best_params: Dict[str, float] = {}

    for conf in np.arange(*conf_range):
        for coh in np.arange(*coh_range):
            for stab in np.arange(*stab_range):
                _, _, _, pips = run_backtest(df, {"conf": conf, "coh": coh, "stab": stab})
                if pips > best_pips:
                    best_pips = pips
                    best_params = {"conf": conf, "coh": coh, "stab": stab}

    print("--- Optimal Thresholds ---")
    print(best_params)
    print(f"Final pips: {best_pips:.2f}")


if __name__ == "__main__":
    main()
