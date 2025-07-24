#!/usr/bin/env python3

import json
import requests
import time
from datetime import datetime, timedelta
from pathlib import Path
import os
import sys

def load_oanda_credentials():
    """Load OANDA credentials from keys.txt file"""
    keys_file = Path(__file__).parent / "keys.txt"
    if not keys_file.exists():
        raise FileNotFoundError("keys.txt file not found")
    
    credentials = {}
    with open(keys_file, 'r') as f:
        for line in f:
            line = line.strip()
            # Handle both 'export VAR=' and 'VAR=' formats
            if 'OANDA_API_KEY=' in line:
                credentials['api_key'] = line.split('=', 1)[1].strip('"')
            elif 'OANDA_ACCOUNT_ID=' in line:
                credentials['account_id'] = line.split('=', 1)[1].strip('"')
    
    if 'api_key' not in credentials or 'account_id' not in credentials:
        raise ValueError("Could not find OANDA credentials in keys.txt")
    
    return credentials

def fetch_oanda_data(api_key, instrument="EUR_USD", granularity="M1", count=2880):
    """
    Fetch historical M1 candle data from OANDA
    
    Args:
        api_key: OANDA API key
        instrument: Currency pair (default: EUR_USD)
        granularity: Time granularity (default: M1 for 1-minute candles)
        count: Number of candles to fetch (default: 2880 for 48 hours)
    """
    
    # Use practice/sandbox environment
    base_url = "https://api-fxpractice.oanda.com"
    
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    
    # Calculate time range for last 48 hours
    end_time = datetime.utcnow()
    start_time = end_time - timedelta(hours=48)
    
    url = f"{base_url}/v3/instruments/{instrument}/candles"
    params = {
        "granularity": granularity,
        "count": count,
        "price": "MBA",  # Mid, Bid, Ask prices
        "includeFirst": "true"
    }
    
    print(f"Fetching {count} {granularity} candles for {instrument}...")
    print(f"Time range: {start_time.isoformat()}Z to {end_time.isoformat()}Z")
    
    try:
        response = requests.get(url, headers=headers, params=params, timeout=30)
        response.raise_for_status()
        
        data = response.json()
        
        if 'candles' not in data:
            raise ValueError("No candles data in response")
        
        candles = data['candles']
        print(f"Successfully fetched {len(candles)} candles")
        
        return {
            "instrument": instrument,
            "granularity": granularity,
            "candles": candles,
            "fetch_time": datetime.utcnow().isoformat() + "Z",
            "total_candles": len(candles)
        }
        
    except requests.exceptions.RequestException as e:
        raise Exception(f"Failed to fetch data from OANDA: {e}")
    
    except json.JSONDecodeError as e:
        raise Exception(f"Invalid JSON response from OANDA: {e}")

def validate_candle_data(data):
    """
    Validate the integrity of the fetched candle data
    
    Args:
        data: Dictionary containing OANDA candle data
    
    Returns:
        dict: Validation results
    """
    candles = data.get('candles', [])
    
    if not candles:
        return {"valid": False, "error": "No candles found"}
    
    validation_results = {
        "valid": True,
        "total_candles": len(candles),
        "time_range": {},
        "price_validation": {},
        "gaps": []
    }
    
    # Time range validation
    first_candle = candles[0]
    last_candle = candles[-1]
    
    validation_results["time_range"] = {
        "start": first_candle["time"],
        "end": last_candle["time"],
        "duration_hours": len(candles) / 60  # M1 candles per hour
    }
    
    # Price validation
    valid_prices = 0
    invalid_prices = 0
    
    for i, candle in enumerate(candles):
        if "mid" in candle:
            mid = candle["mid"]
            
            # Check OHLC validity
            if (mid["open"] > 0 and mid["high"] > 0 and 
                mid["low"] > 0 and mid["close"] > 0 and
                mid["high"] >= max(mid["open"], mid["close"]) and
                mid["low"] <= min(mid["open"], mid["close"])):
                valid_prices += 1
            else:
                invalid_prices += 1
                
        # Check for time gaps (more than 1 minute between consecutive candles)
        if i > 0:
            prev_time = datetime.fromisoformat(candles[i-1]["time"].replace('Z', '+00:00'))
            curr_time = datetime.fromisoformat(candle["time"].replace('Z', '+00:00'))
            gap_minutes = (curr_time - prev_time).total_seconds() / 60
            
            if gap_minutes > 1.5:  # Allow small timing variations
                validation_results["gaps"].append({
                    "index": i,
                    "gap_minutes": gap_minutes,
                    "prev_time": candles[i-1]["time"],
                    "curr_time": candle["time"]
                })
    
    validation_results["price_validation"] = {
        "valid_candles": valid_prices,
        "invalid_candles": invalid_prices,
        "valid_percentage": (valid_prices / len(candles)) * 100 if candles else 0
    }
    
    # Mark as invalid if too many issues
    if invalid_prices > len(candles) * 0.1:  # More than 10% invalid
        validation_results["valid"] = False
        validation_results["error"] = f"Too many invalid price candles: {invalid_prices}"
    
    return validation_results

def save_sample_data(data, output_path):
    """Save the fetched data to JSON file"""
    output_path = Path(output_path)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    
    with open(output_path, 'w') as f:
        json.dump(data, f, indent=2)
    
    print(f"Data saved to {output_path}")
    print(f"File size: {output_path.stat().st_size / 1024:.1f} KB")

def main():
    try:
        print("=== SEP Engine OANDA Sample Data Fetcher ===")
        
        # Load credentials
        print("Loading OANDA credentials...")
        credentials = load_oanda_credentials()
        
        # Fetch data
        data = fetch_oanda_data(credentials['api_key'])
        
        # Validate data
        print("\nValidating data integrity...")
        validation = validate_candle_data(data)
        
        if validation["valid"]:
            print("✅ Data validation passed")
            print(f"   Total candles: {validation['total_candles']}")
            print(f"   Time range: {validation['time_range']['start']} to {validation['time_range']['end']}")
            print(f"   Duration: {validation['time_range']['duration_hours']:.1f} hours")
            print(f"   Valid prices: {validation['price_validation']['valid_percentage']:.1f}%")
            
            if validation["gaps"]:
                print(f"   ⚠️  Found {len(validation['gaps'])} time gaps")
        else:
            print(f"❌ Data validation failed: {validation.get('error', 'Unknown error')}")
            return 1
        
        # Save data
        output_path = "Testing/OANDA/sample_48h.json"
        save_sample_data(data, output_path)
        
        print(f"\n✅ Successfully created 48-hour EUR/USD M1 sample dataset")
        print(f"   File: {output_path}")
        print(f"   Ready for workbench integration testing")
        
        return 0
        
    except Exception as e:
        print(f"❌ Error: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
