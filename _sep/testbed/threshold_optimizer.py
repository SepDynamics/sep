import argparse
import numpy as np
from itertools import product

from backtest_compare import load_dataset, run_backtest


def frange(start: float, stop: float, step: float):
    """Generate a range of floating point numbers inclusive of stop."""
    while start <= stop + 1e-9:
        yield round(start, 10)
        start += step


def grid_search(df, conf_range, coh_range, stab_range):
    best_params = None
    best_pips = float('-inf')

    for conf, coh, stab in product(conf_range, coh_range, stab_range):
        params = {'conf': conf, 'coh': coh, 'stab': stab}
        _, _, _, final_pips = run_backtest(df.copy(), params)
        if final_pips > best_pips:
            best_pips = final_pips
            best_params = params

    return best_params, best_pips


def main():
    parser = argparse.ArgumentParser(description="Optimize strategy thresholds using backtest data")
    parser.add_argument("data", help="Path to pme_testbed.json output")
    parser.add_argument("--conf_start", type=float, default=0.6)
    parser.add_argument("--conf_end", type=float, default=0.9)
    parser.add_argument("--conf_step", type=float, default=0.05)
    parser.add_argument("--coh_start", type=float, default=0.6)
    parser.add_argument("--coh_end", type=float, default=0.9)
    parser.add_argument("--coh_step", type=float, default=0.05)
    parser.add_argument("--stab_start", type=float, default=0.0)
    parser.add_argument("--stab_end", type=float, default=0.5)
    parser.add_argument("--stab_step", type=float, default=0.1)
    args = parser.parse_args()

    df = load_dataset(args.data)

    conf_range = list(frange(args.conf_start, args.conf_end, args.conf_step))
    coh_range = list(frange(args.coh_start, args.coh_end, args.coh_step))
    stab_range = list(frange(args.stab_start, args.stab_end, args.stab_step))

    best_params, best_pips = grid_search(df, conf_range, coh_range, stab_range)

    print("Best Parameters:", best_params)
    print("Final Pips:", round(best_pips, 5))


if __name__ == "__main__":
    main()
