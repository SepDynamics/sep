#!/usr/bin/env python3
"""Simple strategy comparison using parameters from a YAML file."""

import argparse
import json
from pathlib import Path

import yaml
from _sep.testbed.backtest_compare import load_dataset, compare_strategies


def main() -> None:
    parser = argparse.ArgumentParser(description="Compare two SEP strategies")
    parser.add_argument("data", help="CSV dataset from pme_testbed")
    parser.add_argument("config", help="YAML file with strategyA and strategyB params")
    args = parser.parse_args()

    df = load_dataset(args.data)
    cfg = yaml.safe_load(Path(args.config).read_text())

    params_a = cfg.get("strategyA", {"conf": 0.6, "coh": 0.9, "stab": 0.0})
    params_b = cfg.get("strategyB", {"conf": 0.7, "coh": 0.9, "stab": 0.0})

    compare_strategies(df, params_a, params_b)


if __name__ == "__main__":
    main()
