"""Compare multiple SEP strategies using backtest metrics.

This script loads a dataset produced by `pme_testbed.json` and evaluates
several strategy parameter sets defined in a YAML or JSON configuration
file. Results include final pips, Sharpe ratio, and maximum drawdown for
each strategy.
"""

import argparse
import json
from pathlib import Path
from typing import Dict, Any

import yaml

from backtest_compare import load_dataset, run_backtest, performance_summary


def evaluate_strategy(df, params: Dict[str, float]) -> Dict[str, Any]:
    """Run backtest on a copy of df and return key metrics."""
    daily, _, _, final_pips = run_backtest(df.copy(), params)
    perf = performance_summary(daily.cumsum())
    return {
        "final_pips": round(final_pips, 5),
        "sharpe_ratio": round(perf["sharpe_ratio"], 4),
        "max_drawdown": round(perf["max_drawdown"], 4),
    }


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Compare multiple SEP strategies on a dataset"
    )
    parser.add_argument("data", help="CSV dataset from pme_testbed")
    parser.add_argument("config", help="YAML or JSON file with strategy params")
    parser.add_argument(
        "--output",
        type=Path,
        default=None,
        help="Optional JSON file to save comparison results",
    )
    args = parser.parse_args()

    df = load_dataset(args.data)

    text = Path(args.config).read_text()
    if args.config.endswith(".json"):
        cfg = json.loads(text)
    else:
        cfg = yaml.safe_load(text)

    strategies = cfg.get("strategies")
    if not strategies:
        raise ValueError("Config must define a 'strategies' list")

    results = {}
    for idx, strat in enumerate(strategies, 1):
        name = strat.get("name", f"strategy_{idx}")
        params = {
            "conf": float(strat.get("conf", 0.6)),
            "coh": float(strat.get("coh", 0.9)),
            "stab": float(strat.get("stab", 0.0)),
        }
        results[name] = evaluate_strategy(df, params)

    print(json.dumps(results, indent=2))
    if args.output:
        with open(args.output, "w") as f:
            json.dump(results, f, indent=2)


if __name__ == "__main__":
    main()
