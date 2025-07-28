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


def smooth_metrics(df: pd.DataFrame, window: int = 5) -> pd.DataFrame:
    """Return a DataFrame with smoothed coherence and stability values."""
    if df.empty:
        return df

    df = df.copy()
    df['coherence_smooth'] = (
        df['coherence'].rolling(window=window, min_periods=1).mean()
    )
    df['stability_smooth'] = (
        df['stability'].rolling(window=window, min_periods=1).mean()
    )
    return df


def signal_quality(df: pd.DataFrame, weight_c: float = 0.7, weight_s: float = 0.3) -> pd.Series:
    """Calculate a composite signal quality score using smoothed metrics."""
    if 'coherence_smooth' not in df.columns or 'stability_smooth' not in df.columns:
        df = smooth_metrics(df)

    return weight_c * df['coherence_smooth'] + weight_s * df['stability_smooth']

