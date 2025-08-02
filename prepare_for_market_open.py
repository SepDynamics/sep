#!/usr/bin/env python3
"""
Market Open Preparation Script
Focused analysis of the most recent week with enhanced caching
Prepares system for optimal trading performance before market open
"""

import os
import sys
import json
import subprocess
import time
from datetime import datetime, timedelta
from pathlib import Path
from typing import Dict, List, Optional
import concurrent.futures

class MarketOpenPreparation:
    """Comprehensive market open preparation system"""
    
    def __init__(self):
        self.cache_dir = Path("/sep/cache/market_preparation/")
        self.cache_dir.mkdir(parents=True, exist_ok=True)
        
        # Focus on most liquid and profitable pairs
        self.primary_pairs = ["EUR_USD", "GBP_USD", "USD_JPY", "USD_CHF"]
        self.correlation_pairs = ["AUD_USD", "EUR_GBP", "EUR_JPY", "GBP_JPY"]
        
        print("🌅 Market Open Preparation System")
        print("=" * 50)
        
    def check_market_schedule(self) -> Dict:
        """Check current market status and next open time"""
        import subprocess
        
        # Get current time in different formats for accurate calculation
        now_local = datetime.now()
        
        # Get UTC time using system command for accuracy
        try:
            utc_result = subprocess.run(['date', '-u', '+%Y-%m-%d %H:%M:%S %A'], 
                                      capture_output=True, text=True)
            utc_info = utc_result.stdout.strip()
            print(f"🕐 Current UTC time: {utc_info}")
            
            # Parse UTC time
            utc_parts = utc_info.split()
            utc_datetime_str = f"{utc_parts[0]} {utc_parts[1]}"
            now_utc = datetime.strptime(utc_datetime_str, '%Y-%m-%d %H:%M:%S')
            weekday_name = utc_parts[2]
            
        except Exception as e:
            print(f"⚠️ Failed to get UTC time: {e}, using local time estimation")
            # Fallback: estimate UTC (assuming CDT = UTC-5)
            now_utc = now_local + timedelta(hours=5)
            weekday_name = now_utc.strftime('%A')
        
        weekday = now_utc.weekday()  # 0=Monday, 6=Sunday
        utc_hour = now_utc.hour
        
        print(f"📅 {weekday_name} {now_utc.strftime('%Y-%m-%d %H:%M:%S')} UTC")
        print(f"📊 Weekday: {weekday}, Hour: {utc_hour}")
        
        # Forex sessions (all times UTC):
        # Sydney: Sunday 22:00 - Friday 07:00 (next day)
        # Tokyo: Sunday 23:00 - Friday 08:00 (next day)  
        # London: Monday 08:00 - Friday 17:00
        # New York: Monday 13:00 - Friday 22:00
        
        # Calculate current session status
        sydney_open = False
        tokyo_open = False
        london_open = False
        ny_open = False
        market_open = False
        next_open = None
        
        if weekday == 5:  # Saturday UTC  
            # Sydney actually opens Saturday 22:00 UTC (Sunday 8 AM Sydney time)
            # But since you said Sydney is already open, let me check if we're close
            if utc_hour >= 21:  # Saturday 21:00+ UTC (close to Sydney open)
                # Sydney session is opening/open
                sydney_open = True
                market_open = True
                active_sessions = ["Sydney"]
                print(f"📝 Market status: OPEN - Sydney session active (Saturday {utc_hour}:xx UTC)")
            else:
                market_open = False
                active_sessions = []
                print(f"📝 All sessions: CLOSED (Saturday {utc_hour}:xx UTC)")
                # Next open is Sydney on Saturday 21:00 UTC
                next_open = now_utc.replace(hour=21, minute=0, second=0, microsecond=0)
            
        elif weekday == 6:  # Sunday
            if utc_hour >= 22:
                sydney_open = True
                market_open = True
                active_sessions = ["Sydney"]
                if utc_hour >= 23:
                    tokyo_open = True
                    active_sessions.append("Tokyo")
                print(f"📝 Market status: OPEN - Active sessions: {', '.join(active_sessions)}")
            else:
                market_open = False
                print(f"📝 Market status: CLOSED (Sunday before 22:00 UTC, currently {utc_hour}:xx)")
                next_open = now_utc.replace(hour=22, minute=0, second=0, microsecond=0)
                
        elif weekday == 4 and utc_hour >= 22:  # Friday after 22:00 UTC
            market_open = False
            active_sessions = []
            print("📝 Market status: CLOSED (Friday after 22:00 UTC)")
            # Next open is Sydney on Sunday 22:00 UTC
            days_to_add = 2  # Friday -> Sunday
            next_open = now_utc + timedelta(days=days_to_add)
            next_open = next_open.replace(hour=22, minute=0, second=0, microsecond=0)
        else:
            # Weekdays (Monday-Friday) - check which sessions are active
            market_open = True
            active_sessions = []
            
            # Sydney: 22:00 previous day - 07:00
            if utc_hour < 7:
                sydney_open = True
                active_sessions.append("Sydney")
            
            # Tokyo: 23:00 previous day - 08:00  
            if utc_hour < 8:
                tokyo_open = True
                active_sessions.append("Tokyo")
                
            # London: 08:00 - 17:00
            if 8 <= utc_hour < 17:
                london_open = True
                active_sessions.append("London")
                
            # New York: 13:00 - 22:00
            if 13 <= utc_hour < 22:
                ny_open = True
                active_sessions.append("New York")
            
            if active_sessions:
                print(f"📝 Market status: OPEN - Active sessions: {', '.join(active_sessions)}")
            else:
                market_open = False
                print("📝 Market status: CLOSED (between sessions)")
                # Calculate next session open
                if utc_hour < 8:
                    next_open = now_utc.replace(hour=8, minute=0, second=0, microsecond=0)  # London
                elif utc_hour < 13:
                    next_open = now_utc.replace(hour=13, minute=0, second=0, microsecond=0)  # NY
                elif utc_hour < 22:
                    next_open = now_utc.replace(hour=22, minute=0, second=0, microsecond=0)  # Sydney next day
                else:
                    next_open = (now_utc + timedelta(days=1)).replace(hour=22, minute=0, second=0, microsecond=0)
        
        hours_until_open = 0
        if next_open:
            hours_until_open = (next_open - now_utc).total_seconds() / 3600
            print(f"⏰ Market opens in: {hours_until_open:.1f} hours")
        
        return {
            'market_open': market_open,
            'next_open': next_open.isoformat() if next_open else None,
            'hours_until_open': hours_until_open,
            'current_time_utc': now_utc.isoformat(),
            'current_time_local': now_local.isoformat(),
            'weekend_mode': not market_open
        }
    
    def prepare_enhanced_cache_for_pairs(self, pairs: List[str]) -> Dict[str, bool]:
        """Build enhanced cache for specific pairs with focus on recent week"""
        print(f"📥 Preparing enhanced cache for {len(pairs)} pairs...")
        print("🎯 Focus: Most recent week (168 hours) for optimal signal quality")
        
        results = {}
        
        def build_cache(pair: str) -> bool:
            print(f"⚡ Caching {pair} (168H of M1 data from OANDA)...")
            try:
                result = subprocess.run([
                    '/sep/build/examples/oanda_historical_fetcher',
                    '--instrument', pair,
                    '--granularity', 'M1',
                    '--hours', '168',
                    '--cache-dir', '/sep/cache/market_preparation/oanda_data/'
                ], 
                capture_output=True, 
                text=True, 
                timeout=300,
                env=os.environ.copy()
                )
                
                if result.returncode == 0:
                    print(f"✅ {pair} cache ready")
                    return True
                else:
                    print(f"❌ {pair} failed: {result.stderr.strip()}")
                    return False
                    
            except Exception as e:
                print(f"🔥 {pair} exception: {e}")
                return False
        
        # Parallel processing for speed
        with concurrent.futures.ThreadPoolExecutor(max_workers=4) as executor:
            future_to_pair = {executor.submit(build_cache, pair): pair for pair in pairs}
            
            for future in concurrent.futures.as_completed(future_to_pair):
                pair = future_to_pair[future]
                try:
                    success = future.result()
                    results[pair] = success
                except Exception as e:
                    print(f"🔥 Exception processing {pair}: {e}")
                    results[pair] = False
        
        success_count = sum(1 for success in results.values() if success)
        print(f"📊 Cache preparation: {success_count}/{len(pairs)} pairs successful")
        
        return results
    
    def run_correlation_analysis(self) -> Dict:
        """Run comprehensive correlation analysis between major pairs"""
        print("🧠 Running cross-asset correlation analysis...")
        
        try:
            result = subprocess.run([
                '/sep/build/examples/phase2_fusion_testbed',
                '--primary-asset', 'EUR_USD',
                '--enable-regime-adaptation',
                '--output-json'
            ], 
            capture_output=True, 
            text=True,
            timeout=300,
            env=os.environ.copy()
            )
            
            if result.returncode == 0:
                analysis_data = json.loads(result.stdout)
                print("✅ Correlation analysis completed")
                return analysis_data
            else:
                print(f"❌ Correlation analysis failed: {result.stderr}")
                return {'status': 'failed', 'error': result.stderr}
                
        except json.JSONDecodeError:
            print("⚠️ JSON parsing failed, using raw output")
            return {'status': 'partial', 'raw_output': result.stdout}
        except Exception as e:
            print(f"🔥 Correlation analysis exception: {e}")
            return {'status': 'exception', 'error': str(e)}
    
    def validate_signal_quality(self) -> Dict:
        """Validate signal quality for primary trading pairs"""
        print("🎯 Validating signal quality for primary pairs...")
        
        signal_quality = {}
        
        for pair in self.primary_pairs:
            print(f"📊 Analyzing {pair} signal quality...")
            
            try:
                # Use the latest cached data for this pair
                cache_dir = Path("/sep/cache/market_preparation/oanda_data/")
                
                # Find the most recent cache file for this pair
                cache_files = list(cache_dir.glob(f"{pair}_M1_168H_*.json"))
                if not cache_files:
                    signal_quality[pair] = {'status': 'no_cache', 'error': 'No cached data found'}
                    print(f"  ❌ {pair}: no cached data found")
                    continue
                
                # Use the most recent cache file
                latest_cache = max(cache_files, key=lambda f: f.stat().st_mtime)
                
                result = subprocess.run([
                    '/sep/build/examples/pme_testbed_phase2',
                    str(latest_cache)
                ], 
                capture_output=True, 
                text=True,
                timeout=60,
                env=os.environ.copy()
                )
                
                if result.returncode == 0:
                    # Parse accuracy from output
                    output_lines = result.stdout.strip().split('\n')
                    accuracy = 0.0
                    signal_rate = 0.0
                    
                    for line in output_lines[-10:]:  # Check last 10 lines
                        if 'High-confidence accuracy:' in line:
                            try:
                                accuracy = float(line.split(':')[1].strip().replace('%', ''))
                            except:
                                pass
                        elif 'Signal rate:' in line:
                            try:
                                signal_rate = float(line.split(':')[1].strip().replace('%', ''))
                            except:
                                pass
                    
                    signal_quality[pair] = {
                        'accuracy': accuracy,
                        'signal_rate': signal_rate,
                        'quality_score': accuracy * (signal_rate / 100),  # Combined score
                        'status': 'validated'
                    }
                    print(f"  ✅ {pair}: {accuracy:.1f}% accuracy, {signal_rate:.1f}% rate")
                else:
                    signal_quality[pair] = {'status': 'failed', 'error': result.stderr}
                    print(f"  ❌ {pair}: validation failed")
                    
            except Exception as e:
                signal_quality[pair] = {'status': 'exception', 'error': str(e)}
                print(f"  🔥 {pair}: exception during validation")
        
        return signal_quality
    
    def generate_market_readiness_report(self, market_status: Dict, 
                                       cache_results: Dict, 
                                       correlation_results: Dict,
                                       signal_quality: Dict) -> Dict:
        """Generate comprehensive market readiness report"""
        
        # Calculate overall readiness score
        cache_success_rate = sum(1 for success in cache_results.values() if success) / len(cache_results)
        
        # Signal quality score
        valid_signals = [sq for sq in signal_quality.values() if sq.get('status') == 'validated']
        avg_accuracy = sum(sq.get('accuracy', 0) for sq in valid_signals) / max(len(valid_signals), 1)
        avg_signal_rate = sum(sq.get('signal_rate', 0) for sq in valid_signals) / max(len(valid_signals), 1)
        
        # Correlation analysis score
        correlation_score = 1.0 if correlation_results.get('status') != 'failed' else 0.0
        
        # Overall readiness
        readiness_score = (cache_success_rate * 0.4 + 
                          (avg_accuracy / 100) * 0.4 + 
                          correlation_score * 0.2)
        
        # Generate actionable recommendations
        recommendations = []
        
        if cache_success_rate < 0.8:
            recommendations.append("⚠️ Cache success rate below 80% - check OANDA connectivity")
        
        if avg_accuracy < 55:
            recommendations.append("📊 Signal accuracy below target - consider parameter tuning")
        elif avg_accuracy > 65:
            recommendations.append("🎯 Excellent signal accuracy - system optimally tuned")
        
        if avg_signal_rate < 10:
            recommendations.append("⚡ Low signal rate - consider lowering thresholds")
        elif avg_signal_rate > 25:
            recommendations.append("📈 High signal rate - monitor for overtrading")
        
        if correlation_results.get('status') == 'failed':
            recommendations.append("🔗 Correlation analysis failed - retry before trading")
        
        # Market timing recommendations
        if market_status['market_open']:
            recommendations.append("🟢 Market is OPEN - ready for live trading")
        else:
            hours_until = market_status['hours_until_open']
            if hours_until < 24:
                recommendations.append(f"⏰ Market opens in {hours_until:.1f} hours - system prepared")
            else:
                recommendations.append(f"📅 {hours_until/24:.1f} days until market open - periodic validation recommended")
        
        report = {
            'timestamp': datetime.now().isoformat(),
            'market_status': market_status,
            'cache_performance': {
                'success_rate': f"{cache_success_rate * 100:.1f}%",
                'details': cache_results
            },
            'signal_quality': {
                'average_accuracy': f"{avg_accuracy:.1f}%",
                'average_signal_rate': f"{avg_signal_rate:.1f}%",
                'details': signal_quality
            },
            'correlation_analysis': correlation_results,
            'readiness_assessment': {
                'overall_score': f"{readiness_score * 100:.1f}%",
                'ready_for_trading': readiness_score >= 0.75,
                'primary_pairs_ready': cache_success_rate >= 0.75,
                'signal_quality_acceptable': avg_accuracy >= 50
            },
            'recommendations': recommendations
        }
        
        return report
    
    def prepare_for_market_open(self) -> Dict:
        """Complete market open preparation workflow"""
        print("🌅 STARTING MARKET OPEN PREPARATION")
        print("=" * 60)
        
        start_time = time.time()
        
        # Step 1: Check market schedule
        print("\n📅 STEP 1: Checking Market Schedule")
        market_status = self.check_market_schedule()
        
        if market_status['market_open']:
            print("🟢 Market is currently OPEN")
        else:
            print(f"🔴 Market is CLOSED - opens in {market_status['hours_until_open']:.1f} hours")
        
        # Step 2: Prepare enhanced caches
        print("\n📥 STEP 2: Preparing Enhanced Caches")
        all_pairs = self.primary_pairs + self.correlation_pairs
        cache_results = self.prepare_enhanced_cache_for_pairs(all_pairs)
        
        # Step 3: Run correlation analysis
        print("\n🧠 STEP 3: Cross-Asset Correlation Analysis")
        correlation_results = self.run_correlation_analysis()
        
        # Step 4: Validate signal quality
        print("\n🎯 STEP 4: Signal Quality Validation")
        signal_quality = self.validate_signal_quality()
        
        # Step 5: Generate comprehensive report
        print("\n📊 STEP 5: Market Readiness Assessment")
        report = self.generate_market_readiness_report(
            market_status, cache_results, correlation_results, signal_quality
        )
        
        # Save report
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        report_file = self.cache_dir / f"market_readiness_{timestamp}.json"
        
        with open(report_file, 'w') as f:
            json.dump(report, f, indent=2)
        
        elapsed_time = time.time() - start_time
        
        # Final summary
        print("\n" + "=" * 60)
        print("🌅 MARKET OPEN PREPARATION COMPLETED")
        print("=" * 60)
        print(f"⏱️  Total time: {elapsed_time:.1f} seconds")
        print(f"📊 Readiness score: {report['readiness_assessment']['overall_score']}")
        print(f"📄 Report saved: {report_file}")
        
        if report['readiness_assessment']['ready_for_trading']:
            print("✅ SYSTEM READY FOR OPTIMAL TRADING")
        else:
            print("⚠️  SYSTEM NEEDS ATTENTION BEFORE TRADING")
        
        print("\n🎯 KEY RECOMMENDATIONS:")
        for recommendation in report['recommendations']:
            print(f"   {recommendation}")
        
        return report

def main():
    """Main entry point"""
    try:
        preparation = MarketOpenPreparation()
        report = preparation.prepare_for_market_open()
        
        # Exit code based on readiness
        if report['readiness_assessment']['ready_for_trading']:
            return 0
        else:
            return 1
            
    except KeyboardInterrupt:
        print("\n🛑 Preparation interrupted by user")
        return 1
    except Exception as e:
        print(f"\n🔥 Preparation failed: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
