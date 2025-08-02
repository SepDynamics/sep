#!/usr/bin/env python3
"""
Generate M5 and M15 timeframe data from M1 OANDA data
"""
import json
from datetime import datetime, timedelta
from typing import List, Dict, Any

def parse_timestamp(time_str: str) -> datetime:
    """Parse OANDA timestamp format"""
    return datetime.fromisoformat(time_str.replace('Z', '+00:00'))

def format_timestamp(dt: datetime) -> str:
    """Format datetime back to OANDA format"""
    return dt.strftime('%Y-%m-%dT%H:%M:%S.%f')[:-3] + '000Z'

def aggregate_candles(candles: List[Dict], timeframe_minutes: int) -> List[Dict]:
    """Aggregate M1 candles into higher timeframes"""
    if not candles:
        return []
    
    aggregated = []
    current_group = []
    
    # Get the first candle's timestamp and round down to the timeframe boundary
    first_time = parse_timestamp(candles[0]['time'])
    # Round down to nearest timeframe boundary
    minutes_since_midnight = first_time.hour * 60 + first_time.minute
    boundary_minutes = (minutes_since_midnight // timeframe_minutes) * timeframe_minutes
    current_boundary = first_time.replace(hour=boundary_minutes // 60, 
                                         minute=boundary_minutes % 60, 
                                         second=0, microsecond=0)
    
    for candle in candles:
        candle_time = parse_timestamp(candle['time'])
        
        # Check if this candle belongs to the next timeframe boundary
        next_boundary = current_boundary + timedelta(minutes=timeframe_minutes)
        
        if candle_time >= next_boundary:
            # Finalize the current group
            if current_group:
                aggregated.append(create_aggregated_candle(current_group, current_boundary))
            
            # Start new group
            current_group = [candle]
            current_boundary = next_boundary
            
            # Handle case where candle is much later (gap in data)
            while candle_time >= current_boundary + timedelta(minutes=timeframe_minutes):
                current_boundary += timedelta(minutes=timeframe_minutes)
        else:
            current_group.append(candle)
    
    # Don't forget the last group
    if current_group:
        aggregated.append(create_aggregated_candle(current_group, current_boundary))
    
    return aggregated

def create_aggregated_candle(candles: List[Dict], boundary_time: datetime) -> Dict:
    """Create a single aggregated candle from a group of M1 candles"""
    if not candles:
        raise ValueError("Cannot aggregate empty candle list")
    
    # Extract OHLC values
    opens = [float(c['mid']['o']) for c in candles]
    highs = [float(c['mid']['h']) for c in candles]
    lows = [float(c['mid']['l']) for c in candles]
    closes = [float(c['mid']['c']) for c in candles]
    volumes = [c.get('volume', 0) for c in candles]
    
    # Aggregate: Open=first, High=max, Low=min, Close=last, Volume=sum
    aggregated_candle = {
        "complete": True,
        "volume": sum(volumes),
        "time": format_timestamp(boundary_time),
        "mid": {
            "o": f"{opens[0]:.5f}",
            "h": f"{max(highs):.5f}",
            "l": f"{min(lows):.5f}",
            "c": f"{closes[-1]:.5f}"
        }
    }
    
    return aggregated_candle

def main():
    # Load M1 data
    print("Loading M1 data from O-test-2.json...")
    with open('/sep/Testing/OANDA/O-test-2.json', 'r') as f:
        m1_data = json.load(f)
    
    print(f"Loaded {len(m1_data['candles'])} M1 candles")
    
    # Create M5 data
    print("Creating M5 timeframe data...")
    m5_candles = aggregate_candles(m1_data['candles'], 5)
    m5_data = {"candles": m5_candles}
    
    with open('/sep/Testing/OANDA/O-test-M5.json', 'w') as f:
        json.dump(m5_data, f, indent=2)
    print(f"Created M5 data with {len(m5_candles)} candles")
    
    # Create M15 data
    print("Creating M15 timeframe data...")
    m15_candles = aggregate_candles(m1_data['candles'], 15)
    m15_data = {"candles": m15_candles}
    
    with open('/sep/Testing/OANDA/O-test-M15.json', 'w') as f:
        json.dump(m15_data, f, indent=2)
    print(f"Created M15 data with {len(m15_candles)} candles")
    
    print("Multi-timeframe data generation complete!")
    
    # Show some sample data
    print(f"\nSample M1 candle: {m1_data['candles'][0]['time']} OHLC: {m1_data['candles'][0]['mid']}")
    print(f"Sample M5 candle: {m5_candles[0]['time']} OHLC: {m5_candles[0]['mid']}")
    print(f"Sample M15 candle: {m15_candles[0]['time']} OHLC: {m15_candles[0]['mid']}")

if __name__ == "__main__":
    main()
