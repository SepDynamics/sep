#!/usr/bin/env python3
"""
Multi-Ticker Training System for SEP Engine
Pulls cache from OANDA's historical API and prepares analysis for most recent week
Ready for market refinement before market open
"""

import os
import sys
import json
import subprocess
import time
import threading
from datetime import datetime, timedelta
from pathlib import Path
from dataclasses import dataclass, asdict
from typing import Dict, List, Optional, Tuple
import concurrent.futures

@dataclass
class TrainingConfig:
    """Configuration for multi-ticker training system"""
    # Primary trading pairs for analysis
    major_pairs: List[str] = None
    minor_pairs: List[str] = None
    exotic_pairs: List[str] = None
    
    # Time parameters
    training_window_days: int = 7  # Most recent week
    cache_window_hours: int = 168  # 1 week in hours (7 * 24)
    
    # API and cache settings
    cache_directory: str = "/sep/cache/multi_ticker/"
    oanda_api_key: str = ""
    oanda_account_id: str = ""
    
    # Analysis parameters
    enable_correlation_analysis: bool = True
    enable_quantum_analysis: bool = True
    enable_regime_detection: bool = True
    
    # Performance settings
    max_concurrent_downloads: int = 4
    cache_validation_enabled: bool = True
    
    def __post_init__(self):
        if self.major_pairs is None:
            self.major_pairs = [
                "EUR_USD", "GBP_USD", "USD_JPY", "USD_CHF", 
                "AUD_USD", "USD_CAD", "NZD_USD"
            ]
        if self.minor_pairs is None:
            self.minor_pairs = [
                "EUR_GBP", "EUR_JPY", "EUR_CHF", "EUR_AUD",
                "GBP_JPY", "GBP_CHF", "AUD_JPY", "CAD_JPY"
            ]
        if self.exotic_pairs is None:
            self.exotic_pairs = [
                "USD_TRY", "USD_ZAR", "USD_MXN", "EUR_TRY"
            ]

class MultiTickerTrainingSystem:
    """Enhanced multi-ticker training system with OANDA API integration"""
    
    def __init__(self, config: TrainingConfig):
        self.config = config
        self.cache_dir = Path(config.cache_directory)
        self.cache_dir.mkdir(parents=True, exist_ok=True)
        
        # Validation
        self._validate_environment()
        
        # Statistics tracking
        self.download_stats = {
            'total_pairs': 0,
            'successful_downloads': 0,
            'failed_downloads': 0,
            'cache_hits': 0,
            'analysis_completed': 0
        }
        
        print("🚀 Multi-Ticker Training System Initialized")
        print(f"📊 Major Pairs: {len(config.major_pairs)}")
        print(f"📈 Minor Pairs: {len(config.minor_pairs)}")  
        print(f"🌍 Exotic Pairs: {len(config.exotic_pairs)}")
        print(f"📅 Training Window: {config.training_window_days} days")
        print(f"💾 Cache Directory: {config.cache_directory}")
    
    def _validate_environment(self):
        """Validate environment setup and OANDA credentials"""
        # Check OANDA credentials
        api_key = os.getenv('OANDA_API_KEY') or self.config.oanda_api_key
        account_id = os.getenv('OANDA_ACCOUNT_ID') or self.config.oanda_account_id
        
        if not api_key or not account_id:
            print("⚠️  OANDA credentials not found in environment")
            print("   Please set OANDA_API_KEY and OANDA_ACCOUNT_ID")
            print("   Or run: source OANDA.env")
            sys.exit(1)
        
        # Check if build is up to date
        if not Path("/sep/build/examples/enhanced_cache_testbed").exists():
            print("⚠️  Enhanced cache testbed not found. Building...")
            self._run_build()
    
    def _run_build(self):
        """Run the build system to ensure latest code"""
        print("🔨 Building SEP Engine...")
        result = subprocess.run(['./build.sh'], cwd='/sep', capture_output=True, text=True)
        if result.returncode != 0:
            print(f"❌ Build failed: {result.stderr}")
            sys.exit(1)
        print("✅ Build completed successfully")
    
    def get_all_trading_pairs(self) -> List[str]:
        """Get comprehensive list of all trading pairs"""
        all_pairs = []
        all_pairs.extend(self.config.major_pairs)
        all_pairs.extend(self.config.minor_pairs)
        all_pairs.extend(self.config.exotic_pairs)
        return all_pairs
    
    def build_enhanced_cache_for_pair(self, pair: str) -> bool:
        """Build enhanced cache for a specific trading pair using C++ system"""
        print(f"📥 Building enhanced cache for {pair}...")
        
        try:
            # Use the enhanced cache testbed to build cache
            env = os.environ.copy()
            result = subprocess.run([
                '/sep/build/examples/enhanced_cache_testbed',
                '--instrument', pair,
                '--timeframe', 'M1',
                '--hours', str(self.config.cache_window_hours)
            ], 
            env=env, 
            capture_output=True, 
            text=True, 
            timeout=300  # 5 minute timeout
            )
            
            if result.returncode == 0:
                print(f"✅ Enhanced cache built for {pair}")
                self.download_stats['successful_downloads'] += 1
                return True
            else:
                print(f"❌ Failed to build cache for {pair}: {result.stderr}")
                self.download_stats['failed_downloads'] += 1
                return False
                
        except subprocess.TimeoutExpired:
            print(f"⏰ Timeout building cache for {pair}")
            self.download_stats['failed_downloads'] += 1
            return False
        except Exception as e:
            print(f"🔥 Exception building cache for {pair}: {e}")
            self.download_stats['failed_downloads'] += 1
            return False
    
    def build_all_caches_parallel(self) -> Dict[str, bool]:
        """Build enhanced caches for all pairs in parallel"""
        all_pairs = self.get_all_trading_pairs()
        self.download_stats['total_pairs'] = len(all_pairs)
        
        print(f"🚀 Starting parallel cache building for {len(all_pairs)} pairs...")
        print(f"⚡ Using {self.config.max_concurrent_downloads} concurrent workers")
        
        results = {}
        
        with concurrent.futures.ThreadPoolExecutor(max_workers=self.config.max_concurrent_downloads) as executor:
            # Submit all download tasks
            future_to_pair = {
                executor.submit(self.build_enhanced_cache_for_pair, pair): pair 
                for pair in all_pairs
            }
            
            # Collect results as they complete
            for future in concurrent.futures.as_completed(future_to_pair):
                pair = future_to_pair[future]
                try:
                    success = future.result()
                    results[pair] = success
                    
                    # Progress update
                    completed = len(results)
                    print(f"📊 Progress: {completed}/{len(all_pairs)} pairs completed "
                          f"({completed/len(all_pairs)*100:.1f}%)")
                          
                except Exception as e:
                    print(f"🔥 Exception processing {pair}: {e}")
                    results[pair] = False
                    self.download_stats['failed_downloads'] += 1
        
        return results
    
    def run_multi_asset_analysis(self, primary_pair: str = "EUR_USD") -> Dict:
        """Run comprehensive multi-asset analysis using Phase 2 fusion testbed"""
        print(f"🧠 Running multi-asset analysis with primary pair: {primary_pair}")
        
        try:
            env = os.environ.copy()
            result = subprocess.run([
                '/sep/build/examples/phase2_fusion_testbed',
                '--primary-asset', primary_pair,
                '--enable-regime-adaptation',
                '--verbose-logging',
                '--output-json'
            ], 
            env=env, 
            capture_output=True, 
            text=True,
            timeout=600  # 10 minute timeout
            )
            
            if result.returncode == 0:
                print("✅ Multi-asset analysis completed")
                self.download_stats['analysis_completed'] += 1
                
                # Try to parse JSON output
                try:
                    analysis_results = json.loads(result.stdout)
                    return analysis_results
                except json.JSONDecodeError:
                    # If JSON parsing fails, return basic info
                    return {
                        'status': 'completed',
                        'primary_pair': primary_pair,
                        'raw_output': result.stdout
                    }
            else:
                print(f"❌ Multi-asset analysis failed: {result.stderr}")
                return {'status': 'failed', 'error': result.stderr}
                
        except subprocess.TimeoutExpired:
            print("⏰ Multi-asset analysis timeout")
            return {'status': 'timeout'}
        except Exception as e:
            print(f"🔥 Exception in multi-asset analysis: {e}")
            return {'status': 'exception', 'error': str(e)}
    
    def validate_cache_quality(self) -> Dict[str, any]:
        """Validate the quality and completeness of cached data"""
        print("🔍 Validating cache quality...")
        
        validation_results = {
            'cache_files_found': 0,
            'cache_files_valid': 0,
            'total_data_points': 0,
            'quality_score': 0.0,
            'coverage_analysis': {}
        }
        
        # Check cache directory
        if not self.cache_dir.exists():
            print("❌ Cache directory does not exist")
            return validation_results
        
        # Count cache files
        cache_files = list(self.cache_dir.glob("**/*.json"))
        validation_results['cache_files_found'] = len(cache_files)
        
        # Quick validation of cache files
        valid_files = 0
        total_points = 0
        
        for cache_file in cache_files[:10]:  # Sample validation
            try:
                with open(cache_file, 'r') as f:
                    data = json.load(f)
                    if 'candles' in data and len(data['candles']) > 0:
                        valid_files += 1
                        total_points += len(data['candles'])
            except Exception:
                continue
        
        validation_results['cache_files_valid'] = valid_files
        validation_results['total_data_points'] = total_points
        
        if len(cache_files) > 0:
            validation_results['quality_score'] = valid_files / min(len(cache_files), 10)
        
        print(f"📊 Cache Validation: {valid_files} valid files, {total_points} data points")
        return validation_results
    
    def generate_training_report(self, cache_results: Dict[str, bool], 
                               analysis_results: Dict, 
                               validation_results: Dict) -> Dict:
        """Generate comprehensive training report"""
        
        # Calculate success rates
        successful_pairs = sum(1 for success in cache_results.values() if success)
        total_pairs = len(cache_results)
        success_rate = successful_pairs / total_pairs if total_pairs > 0 else 0
        
        # Get current time for market readiness
        now = datetime.now()
        next_market_open = self._get_next_market_open(now)
        
        report = {
            'timestamp': now.isoformat(),
            'training_window': f"{self.config.training_window_days} days",
            'cache_building': {
                'total_pairs': total_pairs,
                'successful_pairs': successful_pairs,
                'success_rate': f"{success_rate * 100:.1f}%",
                'detailed_results': cache_results
            },
            'multi_asset_analysis': analysis_results,
            'cache_validation': validation_results,
            'market_readiness': {
                'next_market_open': next_market_open.isoformat(),
                'hours_until_open': (next_market_open - now).total_seconds() / 3600,
                'system_ready': success_rate >= 0.8  # 80% success threshold
            },
            'performance_stats': self.download_stats,
            'recommendations': self._generate_recommendations(success_rate, analysis_results)
        }
        
        return report
    
    def _get_next_market_open(self, current_time: datetime) -> datetime:
        """Calculate next forex market open (Sunday 5 PM EST)"""
        # Forex market opens Sunday 5 PM EST (22:00 UTC)
        days_until_sunday = (6 - current_time.weekday()) % 7
        if days_until_sunday == 0 and current_time.hour < 22:
            # It's Sunday before market open
            next_open = current_time.replace(hour=22, minute=0, second=0, microsecond=0)
        else:
            # Next Sunday
            next_open = current_time + timedelta(days=days_until_sunday)
            next_open = next_open.replace(hour=22, minute=0, second=0, microsecond=0)
        
        return next_open
    
    def _generate_recommendations(self, success_rate: float, analysis_results: Dict) -> List[str]:
        """Generate recommendations based on training results"""
        recommendations = []
        
        if success_rate < 0.5:
            recommendations.append("⚠️ Low cache success rate - check OANDA API connectivity")
            recommendations.append("🔧 Consider running build.sh to update system")
        elif success_rate < 0.8:
            recommendations.append("⚡ Moderate success rate - retry failed pairs individually")
        else:
            recommendations.append("✅ Excellent cache success rate - system ready for trading")
        
        if analysis_results.get('status') == 'completed':
            recommendations.append("🧠 Multi-asset analysis completed successfully")
        else:
            recommendations.append("🔄 Consider rerunning multi-asset analysis")
        
        recommendations.append("📊 Monitor correlation strength before market open")
        recommendations.append("🎯 Validate signal quality on primary EUR_USD pair")
        
        return recommendations
    
    def run_complete_training_cycle(self) -> Dict:
        """Run the complete multi-ticker training cycle"""
        print("=" * 80)
        print("🚀 STARTING COMPLETE MULTI-TICKER TRAINING CYCLE")
        print("=" * 80)
        
        start_time = time.time()
        
        # Step 1: Build enhanced caches for all pairs
        print("\n📥 STEP 1: Building Enhanced Caches for All Pairs")
        cache_results = self.build_all_caches_parallel()
        
        # Step 2: Run multi-asset analysis  
        print("\n🧠 STEP 2: Running Multi-Asset Analysis")
        analysis_results = self.run_multi_asset_analysis()
        
        # Step 3: Validate cache quality
        if self.config.cache_validation_enabled:
            print("\n🔍 STEP 3: Validating Cache Quality")
            validation_results = self.validate_cache_quality()
        else:
            validation_results = {'status': 'skipped'}
        
        # Step 4: Generate comprehensive report
        print("\n📊 STEP 4: Generating Training Report")
        report = self.generate_training_report(cache_results, analysis_results, validation_results)
        
        # Save report
        report_file = self.cache_dir / f"training_report_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
        with open(report_file, 'w') as f:
            json.dump(report, f, indent=2)
        
        elapsed_time = time.time() - start_time
        
        print("\n" + "=" * 80)
        print("✅ MULTI-TICKER TRAINING CYCLE COMPLETED")
        print("=" * 80)
        print(f"⏱️  Total time: {elapsed_time:.1f} seconds")
        print(f"📊 Success rate: {report['cache_building']['success_rate']}")
        print(f"📄 Report saved: {report_file}")
        
        if report['market_readiness']['system_ready']:
            print("🎯 SYSTEM READY FOR MARKET OPEN")
        else:
            print("⚠️  System needs attention before market open")
        
        return report

def main():
    """Main entry point for multi-ticker training system"""
    
    # Configuration
    config = TrainingConfig(
        training_window_days=7,
        cache_window_hours=168,  # 1 week
        max_concurrent_downloads=4,
        enable_correlation_analysis=True,
        enable_quantum_analysis=True,
        enable_regime_detection=True
    )
    
    # Initialize and run training system
    training_system = MultiTickerTrainingSystem(config)
    
    try:
        # Run complete training cycle
        report = training_system.run_complete_training_cycle()
        
        # Print final summary
        print("\n🎯 FINAL SUMMARY:")
        for recommendation in report['recommendations']:
            print(f"   {recommendation}")
        
        return 0
        
    except KeyboardInterrupt:
        print("\n🛑 Training interrupted by user")
        return 1
    except Exception as e:
        print(f"\n🔥 Training failed with exception: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
