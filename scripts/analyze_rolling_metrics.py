#!/usr/bin/env python3
"""
Direct SEP Engine Analysis of 48-Hour OANDA Data
Generates time-series plots of coherence, stability, entropy with proper rolling windows
"""

import json
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import subprocess
import os
import sys
from datetime import datetime, timedelta
import tempfile

def load_sample_data():
    """Load the 48-hour EUR/USD sample data"""
    sample_file = '/sep/Testing/OANDA/sample_48h.json'
    if not os.path.exists(sample_file):
        print(f"Sample data not found at {sample_file}")
        return None
    
    with open(sample_file, 'r') as f:
        data = json.load(f)
    
    # Extract candles from OANDA format
    if 'candles' in data:
        candles = []
        for candle in data['candles']:
            candles.append({
                'timestamp': candle['time'],
                'open': float(candle['mid']['o']),
                'high': float(candle['mid']['h']),
                'low': float(candle['mid']['l']),
                'close': float(candle['mid']['c']),
                'volume': candle['volume']
            })
        print(f"Loaded {len(candles)} candles from sample")
        return candles
    else:
        print(f"Loaded {len(data)} data points from sample")
        return data

def create_time_chunks(data, chunk_minutes=60):
    """Split data into time chunks for rolling analysis"""
    if not data:
        return []
    
    # Convert to DataFrame for easier manipulation
    df = pd.DataFrame(data)
    df['timestamp'] = pd.to_datetime(df['timestamp'])
    df = df.sort_values('timestamp')
    
    # Create chunks
    chunks = []
    start_time = df['timestamp'].min()
    end_time = df['timestamp'].max()
    
    current_time = start_time
    while current_time < end_time:
        chunk_end = current_time + timedelta(minutes=chunk_minutes)
        chunk_data = df[(df['timestamp'] >= current_time) & (df['timestamp'] < chunk_end)]
        
        if len(chunk_data) > 0:
            chunks.append({
                'start_time': current_time,
                'end_time': chunk_end,
                'data': chunk_data.to_dict('records')
            })
        
        current_time = chunk_end
    
    print(f"Created {len(chunks)} time chunks of {chunk_minutes} minutes each")
    return chunks

def process_chunk_with_engine(chunk_data):
    """Process a chunk of data through the SEP engine"""
    try:
        # Convert data to JSON-serializable format
        serializable_data = []
        for item in chunk_data:
            serializable_item = {}
            for key, value in item.items():
                if hasattr(value, 'isoformat'):  # Timestamp
                    serializable_item[key] = value.isoformat()
                else:
                    serializable_item[key] = value
            serializable_data.append(serializable_item)
        
        # Create temporary file for this chunk
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
            json.dump(serializable_data, f, indent=2)
            temp_file = f.name
        
        # Run the pattern_metric_example with JSON output
        cmd = ['/sep/build/examples/pattern_metric_example', temp_file, '--json', '--no-clear']
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        
        # Clean up temp file
        os.unlink(temp_file)
        
        if result.returncode != 0:
            print(f"Engine error: {result.stderr}")
            return None
        
        # Parse JSON output
        output_lines = result.stdout.strip().split('\n')
        json_output = None
        
        for line in output_lines:
            if line.strip().startswith('{'):
                try:
                    json_output = json.loads(line)
                    break
                except json.JSONDecodeError:
                    continue
        
        if not json_output:
            print("No valid JSON output from engine")
            return None
        
        return json_output
        
    except subprocess.TimeoutExpired:
        print("Engine timeout")
        return None
    except Exception as e:
        print(f"Error processing chunk: {e}")
        return None

def extract_metrics(engine_output):
    """Extract system metrics from engine output"""
    if not engine_output or 'system_metrics' not in engine_output:
        return None
    
    metrics = engine_output['system_metrics']
    return {
        'coherence': metrics.get('avg_coherence', 0),
        'stability': metrics.get('avg_stability', 0),
        'entropy': metrics.get('avg_entropy', 0),
        'total_patterns': metrics.get('total_patterns', 0),
        'active_patterns': metrics.get('active_patterns', 0)
    }

def calculate_rolling_averages(metrics_list, window_hours=4):
    """Calculate rolling averages for metrics"""
    if len(metrics_list) < window_hours:
        return metrics_list
    
    rolling_metrics = []
    
    for i in range(len(metrics_list)):
        start_idx = max(0, i - window_hours + 1)
        window_data = metrics_list[start_idx:i+1]
        
        if window_data:
            avg_coherence = np.mean([m['coherence'] for m in window_data])
            avg_stability = np.mean([m['stability'] for m in window_data])
            avg_entropy = np.mean([m['entropy'] for m in window_data])
            
            rolling_metrics.append({
                'timestamp': metrics_list[i]['timestamp'],
                'coherence_raw': metrics_list[i]['coherence'],
                'stability_raw': metrics_list[i]['stability'],
                'entropy_raw': metrics_list[i]['entropy'],
                'coherence_4h': avg_coherence,
                'stability_4h': avg_stability,
                'entropy_4h': avg_entropy,
                'total_patterns': metrics_list[i]['total_patterns']
            })
    
    return rolling_metrics

def plot_metrics(rolling_metrics):
    """Generate comprehensive plots of the metrics"""
    if not rolling_metrics:
        print("No metrics to plot")
        return
    
    # Convert to DataFrame for easier plotting
    df = pd.DataFrame(rolling_metrics)
    df['timestamp'] = pd.to_datetime(df['timestamp'])
    
    # Create a large figure with subplots
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    fig.suptitle('SEP Engine Metrics: 48-Hour EUR/USD Analysis', fontsize=16)
    
    # Plot 1: Raw vs Rolling Coherence
    axes[0,0].plot(df['timestamp'], df['coherence_raw'], alpha=0.3, label='Raw Coherence', color='blue')
    axes[0,0].plot(df['timestamp'], df['coherence_4h'], label='4h Rolling Avg', color='darkblue', linewidth=2)
    axes[0,0].set_title('Coherence (Pattern Consistency)')
    axes[0,0].set_ylabel('Coherence')
    axes[0,0].legend()
    axes[0,0].grid(True, alpha=0.3)
    axes[0,0].axhline(y=0.3, color='orange', linestyle='--', alpha=0.7, label='Low Threshold')
    axes[0,0].axhline(y=0.7, color='green', linestyle='--', alpha=0.7, label='High Threshold')
    
    # Plot 2: Raw vs Rolling Stability
    axes[0,1].plot(df['timestamp'], df['stability_raw'], alpha=0.3, label='Raw Stability', color='green')
    axes[0,1].plot(df['timestamp'], df['stability_4h'], label='4h Rolling Avg', color='darkgreen', linewidth=2)
    axes[0,1].set_title('Stability (Pattern Persistence)')
    axes[0,1].set_ylabel('Stability')
    axes[0,1].legend()
    axes[0,1].grid(True, alpha=0.3)
    axes[0,1].axhline(y=0.3, color='red', linestyle='--', alpha=0.7, label='Unstable')
    axes[0,1].axhline(y=0.6, color='orange', linestyle='--', alpha=0.7, label='Moderate')
    
    # Plot 3: Raw vs Rolling Entropy
    axes[1,0].plot(df['timestamp'], df['entropy_raw'], alpha=0.3, label='Raw Entropy', color='red')
    axes[1,0].plot(df['timestamp'], df['entropy_4h'], label='4h Rolling Avg', color='darkred', linewidth=2)
    axes[1,0].set_title('Entropy (Market Chaos/Volatility)')
    axes[1,0].set_ylabel('Entropy')
    axes[1,0].legend()
    axes[1,0].grid(True, alpha=0.3)
    axes[1,0].axhline(y=0.7, color='orange', linestyle='--', alpha=0.7, label='High Volatility')
    axes[1,0].axhline(y=0.3, color='green', linestyle='--', alpha=0.7, label='Low Volatility')
    
    # Plot 4: Trading Signal Analysis
    axes[1,1].plot(df['timestamp'], df['stability_4h'], label='Stability (4h)', color='green', linewidth=2)
    axes[1,1].plot(df['timestamp'], df['entropy_4h'], label='Entropy (4h)', color='red', linewidth=2)
    axes[1,1].set_title('Trading Signal Overlay')
    axes[1,1].set_ylabel('Metric Value')
    axes[1,1].legend()
    axes[1,1].grid(True, alpha=0.3)
    
    # Add trading zones
    axes[1,1].axhline(y=0.3, color='red', linestyle='--', alpha=0.5)
    axes[1,1].axhline(y=0.7, color='orange', linestyle='--', alpha=0.5)
    axes[1,1].fill_between(df['timestamp'], 0, 0.3, alpha=0.1, color='red', label='SELL Zone (Low Stability)')
    axes[1,1].fill_between(df['timestamp'], 0.7, 1.0, alpha=0.1, color='orange', label='VOLATILITY Zone (High Entropy)')
    
    # Format x-axes
    for ax in axes.flat:
        ax.tick_params(axis='x', rotation=45)
    
    plt.tight_layout()
    plt.savefig('/sep/output/rolling_metrics_analysis.png', dpi=300, bbox_inches='tight')
    print("Plot saved to /sep/output/rolling_metrics_analysis.png")
    plt.show()

def identify_trading_signals(rolling_metrics):
    """Identify potential trading signals based on metric thresholds"""
    signals = []
    
    for i, metrics in enumerate(rolling_metrics):
        if i < 4:  # Need some history
            continue
            
        stability_4h = metrics['stability_4h']
        entropy_4h = metrics['entropy_4h']
        coherence_4h = metrics['coherence_4h']
        
        # Signal conditions
        low_stability = stability_4h < 0.3
        high_entropy = entropy_4h > 0.7
        coherence_drop = coherence_4h < 0.2 if i > 0 else False
        
        signal_type = "HOLD"
        confidence = 0
        reasons = []
        
        if low_stability and high_entropy:
            signal_type = "SELL"
            confidence = 80
            reasons.append("Low stability + High entropy = Market uncertainty")
        elif low_stability:
            signal_type = "SELL"
            confidence = 50
            reasons.append("Low stability = Unreliable patterns")
        elif high_entropy:
            signal_type = "CAUTION"
            confidence = 60
            reasons.append("High entropy = Increased volatility")
        elif stability_4h > 0.7 and entropy_4h < 0.3:
            signal_type = "BUY"
            confidence = 70
            reasons.append("High stability + Low entropy = Stable patterns")
        
        if confidence > 40:
            signals.append({
                'timestamp': metrics['timestamp'],
                'signal': signal_type,
                'confidence': confidence,
                'reasons': reasons,
                'stability': stability_4h,
                'entropy': entropy_4h,
                'coherence': coherence_4h
            })
    
    return signals

def main():
    print("SEP Engine Direct Analysis - 48 Hour Rolling Metrics")
    print("=" * 60)
    
    # Load sample data
    data = load_sample_data()
    if not data:
        return
    
    # Create time chunks (1-hour windows)
    chunks = create_time_chunks(data, chunk_minutes=60)
    if not chunks:
        return
    
    print(f"Processing {len(chunks)} time chunks through SEP engine...")
    
    # Process each chunk
    metrics_timeline = []
    for i, chunk in enumerate(chunks):
        print(f"Processing chunk {i+1}/{len(chunks)} ({chunk['start_time']})")
        
        # Process through engine
        engine_result = process_chunk_with_engine(chunk['data'])
        if not engine_result:
            continue
        
        # Extract metrics
        metrics = extract_metrics(engine_result)
        if metrics:
            metrics['timestamp'] = chunk['start_time']
            metrics_timeline.append(metrics)
    
    if not metrics_timeline:
        print("No metrics extracted")
        return
    
    print(f"Successfully processed {len(metrics_timeline)} time periods")
    
    # Calculate rolling averages
    rolling_metrics = calculate_rolling_averages(metrics_timeline, window_hours=4)
    
    # Generate plots
    plot_metrics(rolling_metrics)
    
    # Identify trading signals
    signals = identify_trading_signals(rolling_metrics)
    
    print(f"\nIdentified {len(signals)} potential trading signals:")
    for signal in signals:
        print(f"{signal['timestamp']}: {signal['signal']} ({signal['confidence']}%) - {'; '.join(signal['reasons'])}")

if __name__ == "__main__":
    main()
