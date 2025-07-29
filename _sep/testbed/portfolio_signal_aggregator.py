"""Utility to aggregate quantum trading signals across multiple instruments."""
from typing import List, Dict


def aggregate_signals(signals: List[Dict[str, float]], threshold: float = 0.2) -> str:
    """Return portfolio-level action ('BUY', 'SELL', or 'HOLD').

    Each signal dict must contain ``action`` ('BUY', 'SELL', or 'HOLD') and
    ``confidence`` in the range [0, 1]. The actions are combined into a net
    confidence score which is compared against ``threshold``.
    """
    net = 0.0
    for s in signals:
        action = s.get("action")
        conf = float(s.get("confidence", 0.0))
        if action == "BUY":
            net += conf
        elif action == "SELL":
            net -= conf
    if net > threshold:
        return "BUY"
    if net < -threshold:
        return "SELL"
    return "HOLD"


if __name__ == "__main__":  # pragma: no cover - manual demonstration
    demo_signals = [
        {"instrument": "EUR_USD", "action": "BUY", "confidence": 0.8},
        {"instrument": "GBP_USD", "action": "SELL", "confidence": 0.6},
        {"instrument": "USD_JPY", "action": "BUY", "confidence": 0.55},
    ]
    print("Portfolio Action:", aggregate_signals(demo_signals))
