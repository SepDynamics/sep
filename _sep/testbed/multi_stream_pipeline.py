import asyncio
import random
from typing import List, Dict, Any

async def price_stream(
    pair: str,
    queue: asyncio.Queue,
    interval: float = 0.5,
    max_retries: int = 5,
) -> None:
    """Simulate a price stream for a currency pair with basic retry logic."""
    price = 1.0 + random.random() * 0.1
    retries = 0
    while True:
        try:
            await asyncio.sleep(interval)
            price += random.uniform(-0.001, 0.001)
            event = {
                "instrument": pair,
                "price": round(price, 5),
                "timestamp": asyncio.get_running_loop().time(),
            }
            await queue.put(event)
            retries = 0
        except Exception as exc:  # pragma: no cover - network simulation
            retries += 1
            if retries > max_retries:
                print(f"{pair} stream failed: {exc}. Giving up.")
                return
            wait = interval * retries
            print(f"{pair} stream error: {exc}. retry {retries}/{max_retries} in {wait:.2f}s")
            await asyncio.sleep(wait)

async def data_consumer(queue: asyncio.Queue) -> None:
    """Consume events from the queue and process them."""
    while True:
        try:
            event: Dict[str, Any] = await queue.get()
            print(
                f"{event['instrument']} | {event['price']:.5f} | {event['timestamp']:.2f}"
            )
            queue.task_done()
        except Exception as exc:  # pragma: no cover - debug output only
            print(f"Consumer error: {exc}")

async def run_pipeline(instruments: List[str]) -> None:
    queue: asyncio.Queue = asyncio.Queue()
    producers = [asyncio.create_task(price_stream(pair, queue)) for pair in instruments]
    consumer = asyncio.create_task(data_consumer(queue))
    await asyncio.gather(*producers, consumer)

if __name__ == "__main__":
    instruments = ["EUR_USD", "GBP_USD", "USD_JPY", "AUD_USD"]
    try:
        asyncio.run(run_pipeline(instruments))
    except KeyboardInterrupt:
        pass

