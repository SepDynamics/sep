import argparse
import pandas as pd
from typing import Dict, Tuple

from performance_metrics import sharpe_ratio, max_drawdown
from data_quality_tools import detect_gaps, interpolate_missing


def load_dataset(path: str) -> pd.DataFrame:
    """Load the pme_testbed.json output as a DataFrame."""
    df = pd.read_csv(path)
    required_cols = {
        'timestamp',
        'open',
        'high',
        'low',
        'close',
        'volume',
        'pattern_id',
        'coherence',
        'stability',
        'entropy',
        'signal',
        'signal_confidence',
    }
    missing = required_cols - set(df.columns)
    if missing:
        raise ValueError(f"Dataset missing columns: {missing}")
    df['timestamp'] = pd.to_datetime(df['timestamp'])
    df = df.set_index('timestamp').sort_index()

    gaps = detect_gaps(df)
    if gaps:
        print(f"Found {len(gaps)} missing timestamps, interpolating...")
        df = interpolate_missing(df)
    return df


def run_backtest(df: pd.DataFrame, params: Dict[str, float]) -> Tuple[pd.Series, pd.Series, pd.Series, float]:
    """Simple backtest with configurable thresholds.

    Returns daily, weekly and monthly equity curves and final pips.
    """
    equity = []
    capital = 0.0
    for i in range(len(df) - 1):
        row = df.iloc[i]
        next_close = df.iloc[i + 1]['close']
        trade = False
        profit = 0.0
        if row['signal'] == 'BUY' and row['signal_confidence'] >= params['conf']:
            if row['coherence'] >= params['coh'] and row['stability'] >= params['stab']:
                trade = True
                profit = next_close - row['close']
        elif row['signal'] == 'SELL' and row['signal_confidence'] >= params['conf']:
            if row['coherence'] >= params['coh'] and row['stability'] >= params['stab']:
                trade = True
                profit = row['close'] - next_close
        if trade:
            capital += profit
        equity.append(capital)
    # align length
    equity.append(capital)
    df = df.iloc[:len(equity)].copy()
    df['equity'] = equity
    daily = df['equity'].resample('D').last().diff().fillna(0)
    weekly = df['equity'].resample('W').last().diff().fillna(0)
    monthly = df['equity'].resample('M').last().diff().fillna(0)
    return daily, weekly, monthly, capital


def performance_summary(equity: pd.Series) -> Dict[str, float]:
    """Return Sharpe ratio and max drawdown for an equity curve."""
    returns = equity.diff().fillna(0)
    sr = sharpe_ratio(returns)
    dd = max_drawdown(equity)
    return {"sharpe_ratio": sr, "max_drawdown": dd}


def compare_strategies(df: pd.DataFrame, params_a: Dict[str, float], params_b: Dict[str, float]):
    daily_a, weekly_a, monthly_a, final_a = run_backtest(df.copy(), params_a)
    daily_b, weekly_b, monthly_b, final_b = run_backtest(df.copy(), params_b)

    print("Strategy A final pips:", round(final_a, 5))
    print("Strategy B final pips:", round(final_b, 5))

    perf_a = performance_summary(daily_a.cumsum())
    perf_b = performance_summary(daily_b.cumsum())

    print("\nPerformance A: SR={:.2f} DD={:.2%}".format(
        perf_a["sharpe_ratio"], perf_a["max_drawdown"]
    ))
    print("Performance B: SR={:.2f} DD={:.2%}".format(
        perf_b["sharpe_ratio"], perf_b["max_drawdown"]
    ))

    print("\nDaily Returns A vs B")
    print(pd.DataFrame({'A': daily_a, 'B': daily_b}).tail())

    print("\nWeekly Returns A vs B")
    print(pd.DataFrame({'A': weekly_a, 'B': weekly_b}).tail())

    print("\nMonthly Returns A vs B")
    print(pd.DataFrame({'A': monthly_a, 'B': monthly_b}).tail())


def main():
    parser = argparse.ArgumentParser(description="Compare SEP strategies on pme_testbed.json")
    parser.add_argument("data", help="Path to pme_testbed.json output")
    parser.add_argument("--conf_a", type=float, default=0.6)
    parser.add_argument("--coh_a", type=float, default=0.9)
    parser.add_argument("--stab_a", type=float, default=0.0)
    parser.add_argument("--conf_b", type=float, default=0.7)
    parser.add_argument("--coh_b", type=float, default=0.9)
    parser.add_argument("--stab_b", type=float, default=0.0)
    args = parser.parse_args()

    df = load_dataset(args.data)

    params_a = {'conf': args.conf_a, 'coh': args.coh_a, 'stab': args.stab_a}
    params_b = {'conf': args.conf_b, 'coh': args.coh_b, 'stab': args.stab_b}

    compare_strategies(df, params_a, params_b)


if __name__ == "__main__":
    main()
