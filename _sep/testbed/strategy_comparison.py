import argparse
import json
from pathlib import Path

from backtest_compare import load_dataset, run_backtest, performance_summary


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Compare two SEP strategies on pme_testbed.json"
    )
    parser.add_argument("data", help="Path to pme_testbed.json output")
    parser.add_argument("--conf-a", type=float, default=0.6)
    parser.add_argument("--coh-a", type=float, default=0.9)
    parser.add_argument("--stab-a", type=float, default=0.0)
    parser.add_argument("--conf-b", type=float, default=0.7)
    parser.add_argument("--coh-b", type=float, default=0.9)
    parser.add_argument("--stab-b", type=float, default=0.0)
    parser.add_argument("--output", type=Path, help="Optional JSON file to save results")
    args = parser.parse_args()

    df = load_dataset(args.data)

    params_a = {"conf": args.conf_a, "coh": args.coh_a, "stab": args.stab_a}
    params_b = {"conf": args.conf_b, "coh": args.coh_b, "stab": args.stab_b}

    daily_a, _, _, final_a = run_backtest(df.copy(), params_a)
    daily_b, _, _, final_b = run_backtest(df.copy(), params_b)

    summary_a = performance_summary(daily_a.cumsum())
    summary_b = performance_summary(daily_b.cumsum())

    result = {
        "strategy_a": {
            "params": params_a,
            "final_pips": round(final_a, 5),
            "sharpe_ratio": summary_a["sharpe_ratio"],
            "max_drawdown": summary_a["max_drawdown"],
        },
        "strategy_b": {
            "params": params_b,
            "final_pips": round(final_b, 5),
            "sharpe_ratio": summary_b["sharpe_ratio"],
            "max_drawdown": summary_b["max_drawdown"],
        },
    }

    print(json.dumps(result, indent=2))
    if args.output:
        with open(args.output, "w") as f:
            json.dump(result, f, indent=2)


if __name__ == "__main__":
    main()
