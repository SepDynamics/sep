import asyncio
from collections import defaultdict
from typing import Dict, Any

class MarketDataPipeline:
    """Asynchronous multi-currency data pipeline demo."""

    def __init__(self, instruments):
        self.instruments = instruments
        self.data_queues: Dict[str, asyncio.Queue] = {
            inst: asyncio.Queue() for inst in instruments
        }
        self.store: Dict[str, list] = defaultdict(list)
        self.running = False

    async def fetch_market_data(self, instrument: str):
        """Mock data fetcher simulating async I/O."""
        for i in range(5):
            await asyncio.sleep(0.1)
            # Simulated data point
            point: Dict[str, Any] = {
                "instrument": instrument,
                "bid": 1.0 + i/100,
                "ask": 1.0 + i/100 + 0.0002,
                "timestamp": i
            }
            await self.data_queues[instrument].put(point)
        await self.data_queues[instrument].put(None)  # Sentinel

    async def process_stream(self, instrument: str):
        """Consume data and store in thread-safe list."""
        queue = self.data_queues[instrument]
        while True:
            data = await queue.get()
            if data is None:
                break
            self.store[instrument].append(data)

    async def run(self):
        self.running = True
        producers = [self.fetch_market_data(inst) for inst in self.instruments]
        consumers = [self.process_stream(inst) for inst in self.instruments]
        await asyncio.gather(*(producers + consumers))
        self.running = False

if __name__ == "__main__":
    pipeline = MarketDataPipeline([
        "EUR_USD",
        "GBP_USD",
        "USD_JPY",
        "AUD_USD",
    ])
    asyncio.run(pipeline.run())
    for inst, data in pipeline.store.items():
        print(inst, len(data))
