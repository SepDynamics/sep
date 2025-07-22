# TODO.md - SEP Financial Tool Development Priorities

## Immediate Priorities (Fix Today)
- Stabilize OANDA connection to prevent crashes (debug connectToOanda, add error handling, manage streaming thread).
- Implement static UI layout with fixed panels for usability (use ImGui::SetNextWindowPos/Size).
- Integrate real-time OANDA data into SEP engine (format data, pass to engine, display output metrics).
- Map SEP outputs to financial metrics (volatility to coherence, volume to stability, order book to entropy).

## Code Implementation TODOs
From src/apps/oanda_trader/oanda_trader_app.cpp:
- Implement order placement logic.
- Display actual positions from OANDA.
- Display order history from OANDA.

From src/memory/memory_tier_manager.cpp:
- Implement cleanupExpiredData().
- Implement pruneDataByPriority().
- Implement pruneWeakRelationships().
- Implement calculateRelationshipScores().
- Implement calculateRelationshipCoherence().
- Implement cleanupExpiredPatterns().
- Implement prunePatternsByPriority().
- Implement registerGenericData() with proper cloning.
- Implement loadDataFromPersistence().
- Implement storeDataToPersistence().
- Implement processMemoryBlocks().

## Future Tasks
- Implement backtesting framework (event-driven, performance metrics).
- Develop pattern-based trading strategies.
- Add risk management (position sizing, stop-loss, portfolio risk).
- Expand data sources (Yahoo Finance, Alpha Vantage).
- Optimize performance (GPU acceleration, memory pruning).
- Implement API endpoints and monitoring dashboard.

## Notes
- Focus on dynamic, quick shifts to resolve issues.
- Test each component before integration.
- Document implementations.
- Reuse existing SEP engine; prioritize data pipeline.