"""Cross-asset correlation analysis for SEP Engine."""

import argparse
import pandas as pd
from _sep.testbed.market_data_normalizer import load_csv


def load_returns(file_arg: str) -> pd.Series:
    """Load a CSV file in the form INSTRUMENT:PATH and return its returns."""
    if ':' not in file_arg:
        raise ValueError("Input must be in INSTRUMENT:PATH format")
    instrument, path = file_arg.split(':', 1)
    df = load_csv(path, instrument=instrument)
    df['return'] = df['close'].pct_change()
    return df.set_index('timestamp')['return'].rename(instrument)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Cross-asset return correlation analysis",
    )
    parser.add_argument(
        'files',
        metavar='INSTRUMENT:PATH',
        nargs='+',
        help='CSV files with instrument name prefix',
    )
    args = parser.parse_args()

    series_list = []
    for file_arg in args.files:
        series_list.append(load_returns(file_arg))

    data = pd.concat(series_list, axis=1).dropna()
    corr = data.corr()
    print(corr.to_string())


if __name__ == '__main__':
    main()
