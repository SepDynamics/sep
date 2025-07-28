import pandas as pd
from typing import Dict, Iterable

# Mapping of standardized column names to possible variants
DEFAULT_MAPPING: Dict[str, Iterable[str]] = {
    "timestamp": ["timestamp", "time", "date"],
    "open": ["open", "Open", "o"],
    "high": ["high", "High", "h"],
    "low": ["low", "Low", "l"],
    "close": ["close", "Close", "c", "price"],
    "volume": ["volume", "Volume", "vol", "v"],
}


def normalize_dataframe(
    df: pd.DataFrame,
    mapping: Dict[str, Iterable[str]] | None = None,
) -> pd.DataFrame:
    """Normalize common market data columns.

    Parameters
    ----------
    df : pd.DataFrame
        Input dataframe with arbitrary column names.
    mapping : dict, optional
        Mapping of standard columns to alternative names.

    Returns
    -------
    pd.DataFrame
        Normalized dataframe sorted by timestamp.
    """
    mapping = mapping or DEFAULT_MAPPING
    rename_map: Dict[str, str] = {}
    for std, aliases in mapping.items():
        for a in aliases:
            if a in df.columns:
                rename_map[a] = std
                break
    df = df.rename(columns=rename_map)

    required = {"timestamp", "open", "high", "low", "close"}
    missing = required - set(df.columns)
    if missing:
        raise ValueError(f"Missing required columns: {missing}")

    df["timestamp"] = pd.to_datetime(df["timestamp"])
    return df.sort_values("timestamp").reset_index(drop=True)


def load_csv(
    path: str,
    mapping: Dict[str, Iterable[str]] | None = None,
    instrument: str | None = None,
) -> pd.DataFrame:
    """Load a CSV file and normalize columns."""
    df = pd.read_csv(path)
    df = normalize_dataframe(df, mapping)
    if instrument is not None:
        df["instrument"] = instrument
    return df
