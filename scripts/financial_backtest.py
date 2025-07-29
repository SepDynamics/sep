import argparse
import json
import pandas as pd
import numpy as np
from performance_metrics import (
    sharpe_ratio,
    max_drawdown,
    calculate_advanced_metrics,
)

def calculate_gauge(metrics_df):
    """
    Calculates the predictive gauge from the primary metrics.
    """
    # Normalize metrics using standard Z-score
    metrics_df['coherence_norm'] = (
        metrics_df['coherence'] - metrics_df['coherence'].mean()
    ) / metrics_df['coherence'].std(ddof=0)
    metrics_df['stability_norm'] = (
        metrics_df['stability'] - metrics_df['stability'].mean()
    ) / metrics_df['stability'].std(ddof=0)
    metrics_df['entropy_norm'] = (
        metrics_df['entropy'] - metrics_df['entropy'].mean()
    ) / metrics_df['entropy'].std(ddof=0)

    # Fill NaNs that result from rolling calculations
    metrics_df.fillna(0, inplace=True)

    # Define weights
    w_c = 0.5
    w_s = 0.3
    w_e = 0.2

    # Calculate gauge
    gauge = (w_c * metrics_df['coherence_norm']) + \
            (w_s * metrics_df['stability_norm']) - \
            (w_e * metrics_df['entropy_norm'])

    # Smooth the gauge
    smoothed_gauge = gauge.rolling(window=20).mean()
    
    return smoothed_gauge.fillna(0)

def run_analysis_a(df):
    """
    Analysis A: Leading Breakout Strategy
    """
    print("--- Running Analysis A: Leading Breakout Strategy ---")
    
    # --- Strategy Logic ---
    # Signal: 1 for buy (gauge > 0.7), -1 for sell (gauge < 0.3), 0 for hold
    df['signal'] = np.where(df['gauge'] > 0.7, 1, np.where(df['gauge'] < 0.3, -1, 0))
    
    # --- Calculate Returns ---
    # Benchmark (buy and hold) returns
    df['benchmark_returns'] = df['close'].pct_change()
    
    # Strategy returns (based on the signal from the previous day)
    df['strategy_returns'] = df['signal'].shift(1) * df['benchmark_returns']
    
    # --- Performance Analysis ---
    # Calculate cumulative returns
    df['benchmark_cumulative_returns'] = (1 + df['benchmark_returns']).cumprod()
    df['strategy_cumulative_returns'] = (1 + df['strategy_returns']).cumprod()
    
    print("--- Backtest Results ---")
    print(df[['date', 'close', 'gauge', 'signal', 'benchmark_cumulative_returns', 'strategy_cumulative_returns']].tail())
    print("\n")
    
    # --- Key Performance Indicators (KPIs) ---
    strat_returns = df['strategy_returns'].dropna()
    bench_returns = df['benchmark_returns'].dropna()

    metrics = calculate_advanced_metrics(strat_returns, bench_returns)
    strategy_drawdown = max_drawdown(df['strategy_cumulative_returns'])
    benchmark_drawdown = max_drawdown(df['benchmark_cumulative_returns'])

    # Alpha (simplified version)
    alpha = df['strategy_returns'].mean() - df['benchmark_returns'].mean()
    alpha_annualized = alpha * 252

    print("--- Performance Metrics ---")
    print(f"Strategy Total Return: {df['strategy_cumulative_returns'].iloc[-1]:.2%}")
    print(f"Benchmark Total Return: {df['benchmark_cumulative_returns'].iloc[-1]:.2%}")
    print(f"Sharpe Ratio: {metrics['sharpe']:.2f}")
    print(f"Max Drawdown: {strategy_drawdown:.2%} (Benchmark: {benchmark_drawdown:.2%})")
    print(f"Calmar Ratio: {metrics['calmar']:.2f}")
    print(f"95% VaR: {metrics['var_95']:.4f}")
    print(f"Information Ratio: {metrics['info_ratio']:.2f}")
    print(f"Profit Factor: {metrics['profit_factor']:.2f}")
    print(f"Annualized Alpha: {alpha_annualized:.2%}")
    print("\n")


def run_analysis_b(df):
    """
    Analysis B: Iterative Learning / Walk-Forward
    """
    print("--- Running Analysis B: Iterative Learning / Walk-Forward ---")
    
    # --- Walk-Forward Parameters ---
    # Using months for simplicity. A real scenario might use days.
    train_window = 12 # months
    test_window = 1   # month
    
    all_test_results = []
    
    # Create a date range to iterate over
    start_dates = pd.to_datetime(df['date']).dt.to_period('M').unique()
    
    for i in range(0, len(start_dates) - train_window - test_window + 1, test_window):
        train_start = start_dates[i]
        train_end = start_dates[i + train_window - 1]
        test_start = start_dates[i + train_window]
        test_end = start_dates[i + train_window + test_window - 1]

        # --- Isolate Training and Testing Data ---
        train_df = df[(df['date'].dt.to_period('M') >= train_start) & (df['date'].dt.to_period('M') <= train_end)].copy()
        test_df = df[(df['date'].dt.to_period('M') >= test_start) & (df['date'].dt.to_period('M') <= test_end)].copy()

        if train_df.empty or test_df.empty:
            continue

        # --- "Re-fit" Gauge on Training Data ---
        # In a real scenario, you would re-run the SEP engine on the training data.
        # Here, we simulate this by recalculating the gauge on the training slice.
        test_df['gauge'] = calculate_gauge(pd.concat([train_df, test_df]))[-len(test_df):]

        # --- Apply Strategy on Test Data ---
        test_df['signal'] = np.where(test_df['gauge'] > 0.7, 1, np.where(test_df['gauge'] < 0.3, -1, 0))
        test_df['strategy_returns'] = test_df['signal'].shift(1) * test_df['close'].pct_change()
        
        all_test_results.append(test_df)

    # --- Aggregate and Analyze Results ---
    if not all_test_results:
        print("Not enough data to run walk-forward analysis.")
        return
        
    results_df = pd.concat(all_test_results)
    results_df['benchmark_returns'] = results_df['close'].pct_change()
    
    results_df['benchmark_cumulative_returns'] = (1 + results_df['benchmark_returns']).cumprod()
    results_df['strategy_cumulative_returns'] = (1 + results_df['strategy_returns'].fillna(0)).cumprod()

    print("--- Walk-Forward Backtest Results ---")
    print(f"Total periods tested: {len(all_test_results)}")
    print(f"Strategy Total Return: {results_df['strategy_cumulative_returns'].iloc[-1]:.2%}")
    print(f"Benchmark Total Return: {results_df['benchmark_cumulative_returns'].iloc[-1]:.2%}")
    print("\n")


def run_analysis_c(df_daily, df_minute):
    """
    Analysis C: Multi-Resolution Correlation
    """
    print("--- Running Analysis C: Multi-Resolution Correlation ---")

    if df_daily.empty or df_minute.empty:
        print("Not enough data for multi-resolution analysis.")
        return

    # Generate signals from daily data
    df_daily['gauge'] = calculate_gauge(df_daily)
    df_daily['signal'] = np.where(df_daily['gauge'] > 0.7, 1, np.where(df_daily['gauge'] < 0.3, -1, 0))

    # Get the last signal for each day
    daily_signals = df_daily[df_daily['signal'] != 0].copy()
    daily_signals['trade_date'] = daily_signals['date'].dt.date + pd.Timedelta(days=1)

    # Correlate with minute data
    results = []
    for _, row in daily_signals.iterrows():
        trade_day_data = df_minute[df_minute['date'].dt.date == row['trade_date']]
        if not trade_day_data.empty:
            # Calculate return in the first hour after market open
            market_open_price = trade_day_data.iloc[0]['close']
            first_hour_price = trade_day_data[trade_day_data['date'].dt.hour <= trade_day_data.iloc[0]['date'].dt.hour + 1].iloc[-1]['close']
            
            hourly_return = (first_hour_price - market_open_price) / market_open_price
            results.append(hourly_return * row['signal'])

    if not results:
        print("No trading signals found to correlate with minute data.")
        return
        
    avg_return = np.mean(results)
    
    print("--- Multi-Resolution Analysis Results ---")
    print(f"Average return in the first hour after a signal: {avg_return:.4%}")
    print("\n")


def main():
    parser = argparse.ArgumentParser(description="Run financial backtesting analysis on SEP Engine metrics.")
    parser.add_argument("metrics_file", help="Path to the metrics.json file generated by the SEP Engine.")
    args = parser.parse_args()

    # Load the metrics data
    with open(args.metrics_file, 'r') as f:
        metrics_data = json.load(f)
    
    if not metrics_data:
        print("Metrics file is empty. No analysis to run.")
        return

    metrics_df = pd.DataFrame(metrics_data)

    # Use actual timestamps if provided, fall back to synthetic range
    if 'timestamp' in metrics_df.columns:
        metrics_df['date'] = pd.to_datetime(metrics_df['timestamp'])
    elif 'time' in metrics_df.columns:
        metrics_df['date'] = pd.to_datetime(metrics_df['time'])
    else:
        metrics_df['date'] = pd.to_datetime(
            pd.date_range(start='2021-01-01', periods=len(metrics_df), freq='D')
        )

    # --- Synthetic Market Data Generation ---
    # Create a synthetic price series based on the metrics
    np.random.seed(42)
    price_changes = (metrics_df['coherence'] - metrics_df['coherence'].mean()) * 0.01 + \
                    (metrics_df['stability'] - metrics_df['stability'].mean()) * 0.005 + \
                    np.random.randn(len(metrics_df)) * 0.01
    
    initial_price = 1.5000
    metrics_df['close'] = initial_price * (1 + price_changes).cumprod()


    # Calculate the predictive gauge for daily data
    metrics_df['gauge'] = calculate_gauge(metrics_df)

    # --- Run Analyses ---
    # Analysis C is disabled as it requires multi-resolution data which we are not generating.
    run_analysis_a(metrics_df.copy())
    run_analysis_b(metrics_df.copy())

if __name__ == "__main__":
    main()