#!/usr/bin/env python3

import json
import random
from datetime import datetime, timedelta
from pathlib import Path
import sys

def generate_realistic_forex_data(start_time, count=2880, base_price=1.0850):
    """
    Generate realistic EUR/USD M1 candle data
    
    Args:
        start_time: Starting datetime
        count: Number of M1 candles (default 2880 = 48 hours)
        base_price: Starting price for EUR/USD
    """
    
    candles = []
    current_time = start_time
    current_price = base_price
    
    # Market parameters
    avg_spread = 0.00015  # 1.5 pips typical spread
    volatility = 0.0001   # Base volatility per minute
    trend_strength = 0.00001  # Slight upward trend
    
    for i in range(count):
        # Add some realistic market dynamics
        
        # Weekend detection (forex is closed weekends)
        is_weekend = current_time.weekday() >= 5  # Saturday=5, Sunday=6
        
        if is_weekend:
            # Weekend: minimal movement, wider spreads
            price_change = random.gauss(0, volatility * 0.1)
            volume = random.randint(1, 5)
            spread_multiplier = 3.0
        else:
            # Market hours detection (more volatile during London/NY overlap)
            hour = current_time.hour
            
            if 8 <= hour <= 17:  # London session
                vol_mult = 1.5
                volume = random.randint(50, 200)
            elif 13 <= hour <= 22:  # NY session (overlap with London)
                vol_mult = 2.0
                volume = random.randint(100, 300)
            else:  # Asian session or off-hours
                vol_mult = 0.8
                volume = random.randint(20, 80)
            
            # Price movement with trend and volatility
            price_change = random.gauss(trend_strength, volatility * vol_mult)
            spread_multiplier = 1.0
        
        # Update price
        current_price += price_change
        
        # Generate OHLC for this minute
        minute_volatility = volatility * 0.5
        
        open_price = current_price
        close_price = current_price + price_change * 0.1  # Small additional move
        
        # High/Low based on volatility
        high_offset = abs(random.gauss(0, minute_volatility))
        low_offset = abs(random.gauss(0, minute_volatility))
        
        high_price = max(open_price, close_price) + high_offset
        low_price = min(open_price, close_price) - low_offset
        
        # Calculate bid/ask
        current_spread = avg_spread * spread_multiplier
        mid_price = (open_price + close_price) / 2
        
        bid_open = open_price - current_spread/2
        bid_high = high_price - current_spread/2
        bid_low = low_price - current_spread/2
        bid_close = close_price - current_spread/2
        
        ask_open = open_price + current_spread/2
        ask_high = high_price + current_spread/2
        ask_low = low_price + current_spread/2
        ask_close = close_price + current_spread/2
        
        # Create candle in OANDA format
        candle = {
            "complete": True,
            "volume": volume,
            "time": current_time.strftime("%Y-%m-%dT%H:%M:%S.000000000Z"),
            "mid": {
                "o": f"{open_price:.5f}",
                "h": f"{high_price:.5f}",
                "l": f"{low_price:.5f}",
                "c": f"{close_price:.5f}"
            },
            "bid": {
                "o": f"{bid_open:.5f}",
                "h": f"{bid_high:.5f}",
                "l": f"{bid_low:.5f}",
                "c": f"{bid_close:.5f}"
            },
            "ask": {
                "o": f"{ask_open:.5f}",
                "h": f"{ask_high:.5f}",
                "l": f"{ask_low:.5f}",
                "c": f"{ask_close:.5f}"
            }
        }
        
        candles.append(candle)
        
        # Move to next minute
        current_time += timedelta(minutes=1)
        current_price = close_price
    
    return candles

def create_mock_oanda_response(candles):
    """Create a response matching OANDA API format"""
    
    return {
        "instrument": "EUR_USD",
        "granularity": "M1",
        "candles": candles,
        "fetch_time": datetime.utcnow().isoformat() + "Z",
        "total_candles": len(candles),
        "data_source": "mock_generator",
        "note": "This is realistic mock data generated for testing purposes"
    }

def save_sample_data(data, output_path):
    """Save the mock data to JSON file"""
    output_path = Path(output_path)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    
    with open(output_path, 'w') as f:
        json.dump(data, f, indent=2)
    
    print(f"Mock data saved to {output_path}")
    print(f"File size: {output_path.stat().st_size / 1024:.1f} KB")

def main():
    try:
        print("=== SEP Engine Mock OANDA Sample Data Generator ===")
        
        # Generate 48 hours of mock data ending now
        end_time = datetime.utcnow()
        start_time = end_time - timedelta(hours=48)
        
        print(f"Generating 48 hours of EUR/USD M1 mock data...")
        print(f"Time range: {start_time.isoformat()}Z to {end_time.isoformat()}Z")
        
        # Generate realistic candles
        candles = generate_realistic_forex_data(start_time, count=2880)
        
        # Create OANDA-format response
        data = create_mock_oanda_response(candles)
        
        print(f"Generated {len(candles)} realistic M1 candles")
        
        # Save data
        output_path = "Testing/OANDA/sample_48h.json"
        save_sample_data(data, output_path)
        
        # Basic validation
        print(f"\n📊 Mock Data Summary:")
        print(f"   Instrument: {data['instrument']}")
        print(f"   Granularity: {data['granularity']} ({len(candles)} candles)")
        print(f"   Time span: {candles[0]['time']} to {candles[-1]['time']}")
        
        first_mid = candles[0]['mid']
        last_mid = candles[-1]['mid']
        price_change = float(last_mid['c']) - float(first_mid['o'])
        price_change_pips = price_change * 10000
        
        print(f"   Price range: {first_mid['o']} to {last_mid['c']}")
        print(f"   Price change: {price_change_pips:+.1f} pips")
        
        total_volume = sum(c['volume'] for c in candles)
        print(f"   Total volume: {total_volume:,}")
        
        print(f"\n✅ Mock 48-hour EUR/USD M1 dataset created successfully!")
        print(f"   Ready for workbench integration testing")
        print(f"   Note: This is realistic mock data, not live market data")
        
        return 0
        
    except Exception as e:
        print(f"❌ Error: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
