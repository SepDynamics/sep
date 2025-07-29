import unittest
from _sep.testbed.portfolio_signal_aggregator import aggregate_signals


class PortfolioSignalAggregatorTest(unittest.TestCase):
    def test_buy_result(self):
        signals = [
            {"instrument": "EUR_USD", "action": "BUY", "confidence": 0.8},
            {"instrument": "GBP_USD", "action": "SELL", "confidence": 0.3},
        ]
        self.assertEqual(aggregate_signals(signals, threshold=0.2), "BUY")

    def test_hold_result(self):
        signals = [
            {"instrument": "EUR_USD", "action": "BUY", "confidence": 0.1},
            {"instrument": "USD_JPY", "action": "SELL", "confidence": 0.1},
        ]
        self.assertEqual(aggregate_signals(signals, threshold=0.5), "HOLD")

    def test_sell_result(self):
        signals = [
            {"instrument": "EUR_USD", "action": "SELL", "confidence": 0.7},
            {"instrument": "USD_JPY", "action": "SELL", "confidence": 0.6},
        ]
        self.assertEqual(aggregate_signals(signals, threshold=0.5), "SELL")


if __name__ == "__main__":
    unittest.main()
