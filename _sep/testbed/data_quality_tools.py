import pandas as pd
from collections import deque
from typing import Deque


def detect_gaps(df: pd.DataFrame, freq: str = "T") -> list:
    """Return a list of missing timestamps at the given frequency."""
    if df.empty or not isinstance(df.index, pd.DatetimeIndex):
        return []
    df = df.sort_index()
    expected = pd.date_range(df.index[0], df.index[-1], freq=freq)
    missing = expected.difference(df.index)
    return list(missing)


def interpolate_missing(df: pd.DataFrame, freq: str = "T") -> pd.DataFrame:
    """Fill missing rows by interpolation."""
    if df.empty:
        return df
    df = df.sort_index().asfreq(freq)
    return df.interpolate()


def trim_history(history: Deque, max_size: int) -> None:
    """Trim a deque to the specified maximum size."""
    while len(history) > max_size:
        history.popleft()

