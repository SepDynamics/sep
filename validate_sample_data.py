#!/usr/bin/env python3

import json
import sys
from datetime import datetime
from pathlib import Path

def load_sample_data(file_path):
    """Load and parse the sample JSON data"""
    path = Path(file_path)
    if not path.exists():
        raise FileNotFoundError(f"Sample data file not found: {file_path}")
    
    with open(path, 'r') as f:
        data = json.load(f)
    
    return data

def validate_oanda_format(data):
    """Validate that the data matches expected OANDA JSON format"""
    
    required_fields = ["instrument", "granularity", "candles"]
    
    for field in required_fields:
        if field not in data:
            return False, f"Missing required field: {field}"
    
    candles = data["candles"]
    if not isinstance(candles, list) or len(candles) == 0:
        return False, "Candles should be a non-empty list"
    
    # Validate first candle structure
    first_candle = candles[0]
    required_candle_fields = ["time", "mid", "volume"]
    
    for field in required_candle_fields:
        if field not in first_candle:
            return False, f"Missing candle field: {field}"
    
    # Validate mid price structure
    mid = first_candle["mid"]
    required_mid_fields = ["o", "h", "l", "c"]  # OANDA format uses single letters
    
    for field in required_mid_fields:
        if field not in mid:
            return False, f"Missing mid price field: {field}"
    
    return True, "Format validation passed"

def analyze_data_quality(data):
    """Analyze the quality and characteristics of the data"""
    
    candles = data["candles"]
    
    analysis = {
        "total_candles": len(candles),
        "instrument": data["instrument"],
        "granularity": data["granularity"],
        "time_analysis": {},
        "price_analysis": {},
        "volume_analysis": {},
        "market_hours": {}
    }
    
    # Time analysis
    times = [datetime.fromisoformat(c["time"].replace('Z', '+00:00')) for c in candles]
    analysis["time_analysis"] = {
        "start_time": times[0].isoformat(),
        "end_time": times[-1].isoformat(),
        "duration_hours": (times[-1] - times[0]).total_seconds() / 3600,
        "expected_candles": int((times[-1] - times[0]).total_seconds() / 60),  # M1 = 1 minute
        "actual_candles": len(candles),
        "completeness": len(candles) / max(1, int((times[-1] - times[0]).total_seconds() / 60))
    }
    
    # Price analysis
    prices = []
    spreads = []
    
    for candle in candles:
        mid = candle["mid"]
        o, h, l, c = float(mid["o"]), float(mid["h"]), float(mid["l"]), float(mid["c"])
        prices.extend([o, h, l, c])
        
        # Calculate typical spread if bid/ask available
        if "bid" in candle and "ask" in candle:
            bid_c = float(candle["bid"]["c"])
            ask_c = float(candle["ask"]["c"])
            spreads.append(ask_c - bid_c)
    
    analysis["price_analysis"] = {
        "min_price": min(prices),
        "max_price": max(prices),
        "price_range": max(prices) - min(prices),
        "avg_price": sum(prices) / len(prices),
        "price_pips": (max(prices) - min(prices)) * 10000  # EUR/USD pip calculation
    }
    
    if spreads:
        analysis["price_analysis"]["avg_spread_pips"] = (sum(spreads) / len(spreads)) * 10000
    
    # Volume analysis
    volumes = [int(candle["volume"]) for candle in candles]
    analysis["volume_analysis"] = {
        "total_volume": sum(volumes),
        "avg_volume": sum(volumes) / len(volumes),
        "min_volume": min(volumes),
        "max_volume": max(volumes),
        "zero_volume_candles": sum(1 for v in volumes if v == 0)
    }
    
    # Market hours analysis (UTC-based)
    weekday_candles = 0
    weekend_candles = 0
    
    for time_obj in times:
        if time_obj.weekday() < 5:  # Monday=0 to Friday=4
            weekday_candles += 1
        else:
            weekend_candles += 1
    
    analysis["market_hours"] = {
        "weekday_candles": weekday_candles,
        "weekend_candles": weekend_candles,
        "weekend_percentage": (weekend_candles / len(candles)) * 100
    }
    
    return analysis

def check_data_gaps(data):
    """Check for gaps in the time series data"""
    
    candles = data["candles"]
    gaps = []
    
    for i in range(1, len(candles)):
        prev_time = datetime.fromisoformat(candles[i-1]["time"].replace('Z', '+00:00'))
        curr_time = datetime.fromisoformat(candles[i]["time"].replace('Z', '+00:00'))
        
        gap_minutes = (curr_time - prev_time).total_seconds() / 60
        
        if gap_minutes > 1.1:  # Allow small timing variations
            gaps.append({
                "index": i,
                "gap_minutes": gap_minutes,
                "prev_time": candles[i-1]["time"],
                "curr_time": candles[i]["time"],
                "gap_type": "weekend" if prev_time.weekday() >= 4 else "intraday"
            })
    
    return gaps

def print_validation_report(data, analysis, gaps):
    """Print a comprehensive validation report"""
    
    print("=== OANDA Sample Data Validation Report ===\n")
    
    # Basic info
    print(f"📊 Dataset Overview:")
    print(f"   Instrument: {analysis['instrument']}")
    print(f"   Granularity: {analysis['granularity']}")
    print(f"   Total Candles: {analysis['total_candles']:,}")
    print(f"   File Size: {Path('Testing/OANDA/sample_48h.json').stat().st_size / 1024:.1f} KB")
    
    # Time analysis
    print(f"\n⏰ Time Analysis:")
    time_info = analysis['time_analysis']
    print(f"   Start: {time_info['start_time']}")
    print(f"   End: {time_info['end_time']}")
    print(f"   Duration: {time_info['duration_hours']:.1f} hours")
    print(f"   Data Completeness: {time_info['completeness']*100:.1f}%")
    
    # Price analysis
    print(f"\n💰 Price Analysis:")
    price_info = analysis['price_analysis']
    print(f"   Price Range: {price_info['min_price']:.5f} - {price_info['max_price']:.5f}")
    print(f"   Range (pips): {price_info['price_pips']:.1f}")
    print(f"   Average Price: {price_info['avg_price']:.5f}")
    if 'avg_spread_pips' in price_info:
        print(f"   Average Spread: {price_info['avg_spread_pips']:.1f} pips")
    
    # Volume analysis
    print(f"\n📈 Volume Analysis:")
    volume_info = analysis['volume_analysis']
    print(f"   Total Volume: {volume_info['total_volume']:,}")
    print(f"   Average Volume: {volume_info['avg_volume']:.0f}")
    print(f"   Zero Volume Candles: {volume_info['zero_volume_candles']}")
    
    # Market hours
    print(f"\n🕐 Market Hours:")
    market_info = analysis['market_hours']
    print(f"   Weekday Candles: {market_info['weekday_candles']:,}")
    print(f"   Weekend Candles: {market_info['weekend_candles']:,}")
    print(f"   Weekend %: {market_info['weekend_percentage']:.1f}%")
    
    # Gaps analysis
    print(f"\n🔍 Data Gaps:")
    if gaps:
        print(f"   Found {len(gaps)} gaps:")
        for gap in gaps[:5]:  # Show first 5 gaps
            print(f"     {gap['gap_minutes']:.0f}min gap at {gap['curr_time']} ({gap['gap_type']})")
        if len(gaps) > 5:
            print(f"     ... and {len(gaps) - 5} more")
    else:
        print("   ✅ No significant gaps found")
    
    # Overall assessment
    print(f"\n🎯 Quality Assessment:")
    
    quality_score = 0
    max_score = 5
    
    # Completeness check
    if time_info['completeness'] > 0.95:
        print("   ✅ Data completeness: Excellent (>95%)")
        quality_score += 1
    elif time_info['completeness'] > 0.90:
        print("   ⚠️  Data completeness: Good (>90%)")
        quality_score += 0.5
    else:
        print("   ❌ Data completeness: Poor (<90%)")
    
    # Price validity
    if price_info['price_range'] > 0:
        print("   ✅ Price range: Valid")
        quality_score += 1
    
    # Volume check
    if volume_info['zero_volume_candles'] < len(data['candles']) * 0.1:
        print("   ✅ Volume data: Good (<10% zero volume)")
        quality_score += 1
    else:
        print("   ⚠️  Volume data: Many zero volume candles")
    
    # Gap analysis
    significant_gaps = len([g for g in gaps if g['gap_minutes'] > 5 and g['gap_type'] != 'weekend'])
    if significant_gaps == 0:
        print("   ✅ Time continuity: Excellent")
        quality_score += 1
    elif significant_gaps < 5:
        print("   ⚠️  Time continuity: Good (few gaps)")
        quality_score += 0.5
    
    # Format validation
    print("   ✅ OANDA format: Valid")
    quality_score += 1
    
    print(f"\n   Overall Quality Score: {quality_score}/{max_score} ({quality_score/max_score*100:.0f}%)")
    
    if quality_score >= 4:
        print("   🎉 Dataset ready for workbench integration testing!")
    elif quality_score >= 3:
        print("   ✅ Dataset suitable for basic testing")
    else:
        print("   ⚠️  Dataset may need improvement for reliable testing")

def main():
    if len(sys.argv) > 1:
        file_path = sys.argv[1]
    else:
        file_path = "Testing/OANDA/sample_48h.json"
    
    try:
        # Load data
        print(f"Loading sample data from {file_path}...")
        data = load_sample_data(file_path)
        
        # Validate format
        print("Validating OANDA format...")
        format_valid, format_msg = validate_oanda_format(data)
        if not format_valid:
            print(f"❌ Format validation failed: {format_msg}")
            return 1
        
        # Analyze data quality
        print("Analyzing data quality...")
        analysis = analyze_data_quality(data)
        
        # Check for gaps
        print("Checking for data gaps...")
        gaps = check_data_gaps(data)
        
        # Print report
        print_validation_report(data, analysis, gaps)
        
        return 0
        
    except Exception as e:
        print(f"❌ Validation failed: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
