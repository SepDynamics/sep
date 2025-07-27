import pandas as pd
import numpy as np

def analyze_alpha(file_path, confidence_threshold=0.6, coherence_threshold=0.9, stability_threshold=0.0):
    """
    Analyzes the alpha generation from the pme_testbed.json file.

    Args:
        file_path (str): The path to the JSON file.
        confidence_threshold (float): The minimum signal confidence to execute a trade.
        coherence_threshold (float): The minimum coherence to execute a trade.
        stability_threshold (float): The minimum stability to execute a trade.
    """
    try:
        # 1. Load and prepare the data from the CSV-like JSON file
        df = pd.read_csv(file_path)
        
        # Rename columns to match the expected format
        df.rename(columns={
            'timestamp': 'Timestamp',
            'open': 'Open',
            'high': 'High',
            'low': 'Low',
            'close': 'Close',
            'volume': 'Volume',
            'pattern_id': 'PatternID',
            'coherence': 'Coherence',
            'stability': 'Stability',
            'signal': 'Action',
            'signal_confidence': 'SignalStrength'
        }, inplace=True)
        
        # Add a placeholder for ExecutionPrice if it's not present
        if 'ExecutionPrice' not in df.columns:
            df['ExecutionPrice'] = 0.0

        df['Timestamp'] = pd.to_datetime(df['Timestamp'], errors='coerce')
        df.dropna(subset=['Timestamp'], inplace=True)
        df = df.set_index('Timestamp')
        
        # Ensure numeric types for calculation
        for col in ['Open', 'High', 'Low', 'Close', 'ExecutionPrice', 'SignalStrength', 'Coherence', 'Stability']:
            df[col] = pd.to_numeric(df[col], errors='coerce')
        
        df.dropna(subset=['Close', 'ExecutionPrice', 'SignalStrength', 'Coherence', 'Stability'], inplace=True)

    except FileNotFoundError:
        print(f"Error: The file at {file_path} was not found.")
        return
    except Exception as e:
        print(f"An error occurred while loading the data: {e}")
        return

    # 2. Implement the trading simulation
    pips_gained = 0
    position = 0  # 0 for no position, 1 for long
    buy_price = 0

    for index, row in df.iterrows():
        trade_confidence = (
            row['SignalStrength'] >= confidence_threshold and
            row['Coherence'] >= coherence_threshold and
            row['Stability'] >= stability_threshold
        )

        if row['Action'] == 'BUY' and position == 0 and trade_confidence:
            position = 1
            buy_price = row['Close']
        elif row['Action'] == 'SELL' and position == 1:
            position = 0
            sell_price = row['Close']
            pips_gained += (sell_price - buy_price)

    # Handle open position at the end
    if position == 1:
        final_price = df['Close'].iloc[-1]
        pips_gained += (final_price - buy_price)

    # 3. Calculate benchmark
    if not df.empty:
        perfect_pips = df['Close'].iloc[-1] - df['Close'].iloc[0]
    else:
        perfect_pips = 0

    # 4. Extrapolate to 48 hours
    if not df.empty and len(df.index) > 1:
        time_delta_seconds = (df.index[-1] - df.index[0]).total_seconds()
        if time_delta_seconds > 0:
            pips_per_second = pips_gained / time_delta_seconds
            projected_48_hour_pips = pips_per_second * 48 * 3600
        else:
            projected_48_hour_pips = 0
    else:
        projected_48_hour_pips = 0

    # 5. Output results
    print("--- Alpha Analysis Results ---")
    print(f"Strategy Pips Gained: {pips_gained:.4f}")
    print(f"Perfect Pips (Benchmark): {perfect_pips:.4f}")
    print(f"Projected 48-hour Pips: {projected_48_hour_pips:.4f}")
    print("-----------------------------")

if __name__ == "__main__":
    analyze_alpha('docs/proofs/pme_testbed.json')
