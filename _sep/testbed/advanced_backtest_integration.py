import argparse
import json
from pathlib import Path

from backtest_compare import load_dataset
from threshold_optimizer import frange, grid_search


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Automate backtesting using pme_testbed.json data"
    )
    parser.add_argument("data", help="Path to pme_testbed.json output")
    parser.add_argument(
        "--conf", default="0.6,0.9,0.05",
        help="start,end,step for confidence threshold"
    )
    parser.add_argument(
        "--coh", default="0.6,0.9,0.05",
        help="start,end,step for coherence threshold"
    )
    parser.add_argument(
        "--stab", default="0.0,0.5,0.1",
        help="start,end,step for stability threshold"
    )
    parser.add_argument(
        "--output", type=Path, default=None,
        help="Optional JSON file to save results"
    )
    args = parser.parse_args()

    df = load_dataset(args.data)

    conf_start, conf_end, conf_step = (float(x) for x in args.conf.split(","))
    coh_start, coh_end, coh_step = (float(x) for x in args.coh.split(","))
    stab_start, stab_end, stab_step = (float(x) for x in args.stab.split(","))

    conf_range = list(frange(conf_start, conf_end, conf_step))
    coh_range = list(frange(coh_start, coh_end, coh_step))
    stab_range = list(frange(stab_start, stab_end, stab_step))

    best_params, best_pips = grid_search(df, conf_range, coh_range, stab_range)

    result = {
        "best_params": best_params,
        "final_pips": round(best_pips, 5),
    }

    print(json.dumps(result, indent=2))
    if args.output:
        with open(args.output, "w") as f:
            json.dump(result, f, indent=2)


if __name__ == "__main__":
    main()
