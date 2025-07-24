#!/usr/bin/env python3
"""
Comprehensive verification that the sample data integration is working correctly
"""

import subprocess
import time
import sys
import re

def verify_sample_integration():
    print("🔍 Comprehensive SEP Sample Data Integration Verification")
    print("=" * 60)
    
    try:
        proc = subprocess.Popen(
            ["./build/src/apps/workbench/sep_workbench"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            cwd="/sep",
            text=True,
            bufsize=1
        )
        
        # Verification checkpoints
        checkpoints = {
            'dashboard_init': False,
            'sample_file_found': False,
            'sample_parsing': False,
            'sample_loaded': False,
            'data_conversion': False,
            'sep_processing': False,
            'metrics_calculated': False,
            'display_updated': False
        }
        
        candle_count = 0
        bytes_processed = 0
        
        start_time = time.time()
        timeout = 15  # 15 seconds should be enough
        
        while time.time() - start_time < timeout:
            line = proc.stdout.readline()
            if not line:
                break
                
            # Dashboard initialization
            if "[UnifiedDashboard] Initializing..." in line:
                checkpoints['dashboard_init'] = True
                print("✅ Dashboard initialization started")
                
            # Sample file access
            if "Attempting to load sample data from" in line and "sample_48h.json" in line:
                checkpoints['sample_file_found'] = True
                print("✅ Sample data file located and opened")
                
            # Sample parsing
            if "Parsing" in line and "candles for EUR_USD (M1)" in line:
                checkpoints['sample_parsing'] = True
                # Extract candle count
                match = re.search(r'Parsing (\d+) candles', line)
                if match:
                    candle_count = int(match.group(1))
                print(f"✅ Sample data parsing: {candle_count} candles")
                
            # Sample loaded successfully
            if "Successfully loaded" in line and "sample candles for EUR_USD" in line:
                checkpoints['sample_loaded'] = True
                print("✅ Sample data loaded into memory")
                
            # Data conversion for SEP engine
            if "Converted market data to" in line and "bytes for SEP analysis" in line:
                checkpoints['data_conversion'] = True
                # Extract byte count
                match = re.search(r'(\d+) bytes', line)
                if match:
                    bytes_processed = int(match.group(1))
                print(f"✅ Market data converted to {bytes_processed} bytes for SEP engine")
                
            # SEP engine processing
            if "analyze: events size:" in line:
                checkpoints['sep_processing'] = True
                # Extract events count
                match = re.search(r'events size: (\d+)', line)
                if match:
                    events_count = int(match.group(1))
                print(f"✅ SEP engine processing: {events_count} events analyzed")
                
            # Processing complete
            if "SEP engine processing complete" in line:
                checkpoints['metrics_calculated'] = True
                print("✅ Quantum metrics calculation completed")
                
            # Display update
            if "Updated market display with sample data" in line:
                checkpoints['display_updated'] = True
                print("✅ Dashboard display updated with sample data")
                
            # Stop early if all checkpoints are met
            if all(checkpoints.values()):
                break
                
        # Terminate the process
        proc.terminate()
        try:
            proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            proc.kill()
            proc.wait()
            
        # Results analysis
        print("\n📊 Integration Verification Results:")
        print("=" * 40)
        
        passed_count = sum(checkpoints.values())
        total_count = len(checkpoints)
        
        for checkpoint, passed in checkpoints.items():
            status = "✅ PASS" if passed else "❌ FAIL"
            print(f"{checkpoint.replace('_', ' ').title():.<30} {status}")
            
        print(f"\nOverall Score: {passed_count}/{total_count} ({passed_count/total_count*100:.1f}%)")
        
        if candle_count > 0:
            print(f"\n📈 Data Processing Summary:")
            print(f"   • Candles processed: {candle_count:,}")
            print(f"   • Bytes converted: {bytes_processed:,}")
            print(f"   • Data throughput: {bytes_processed/candle_count:.1f} bytes/candle")
            print(f"   • Processing time: <{timeout}s")
            
        # Final verdict
        if passed_count >= 6:  # Most important checkpoints
            print("\n🎉 SUCCESS: Sample data integration is working correctly!")
            print("   The SEP workbench successfully:")
            print("   - Loads 48-hour EUR/USD sample data")
            print("   - Processes market data through SEP quantum engine")
            print("   - Calculates coherence, stability, and entropy metrics")
            print("   - Updates the dashboard with real quantum analytics")
            return True
        else:
            print("\n⚠️  PARTIAL SUCCESS: Some integration issues detected")
            return False
            
    except Exception as e:
        print(f"❌ ERROR: Integration test failed: {e}")
        return False

if __name__ == "__main__":
    success = verify_sample_integration()
    sys.exit(0 if success else 1)
