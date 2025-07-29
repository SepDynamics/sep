import json
from pathlib import Path
from typing import List, Dict


def parse_metrics_files(files: List[Path]) -> List[Dict[str, float]]:
    """Parse JSON metrics produced by pattern_metric_example."""
    results = []
    for fp in files:
        try:
            text = fp.read_text()
            try:
                data = json.loads(text)
            except json.JSONDecodeError:
                # Handle files that contain extra text before/after JSON
                from .json_utils import parse_first_json
                data = parse_first_json(text)
            metrics = data.get("metrics", {})
            results.append({
                "file": fp.name,
                "coherence": float(metrics.get("coherence", 0.0)),
                "stability": float(metrics.get("stability", 0.0)),
                "entropy": float(metrics.get("entropy", 0.0)),
                "energy": float(metrics.get("energy", 0.0)),
                "pattern_count": int(data.get("pattern_count", 0)),
            })
        except Exception as exc:
            print(f"Failed to parse {fp}: {exc}")
    return results


def average_metrics(metrics: List[Dict[str, float]]) -> Dict[str, float]:
    """Return the average metrics across all parsed files."""
    if not metrics:
        return {
            "coherence": 0.0,
            "stability": 0.0,
            "entropy": 0.0,
            "energy": 0.0,
            "pattern_count": 0.0,
        }

    total = {
        "coherence": 0.0,
        "stability": 0.0,
        "entropy": 0.0,
        "energy": 0.0,
        "pattern_count": 0.0,
    }

    for m in metrics:
        total["coherence"] += m.get("coherence", 0.0)
        total["stability"] += m.get("stability", 0.0)
        total["entropy"] += m.get("entropy", 0.0)
        total["energy"] += m.get("energy", 0.0)
        total["pattern_count"] += m.get("pattern_count", 0)

    n = len(metrics)
    return {
        "coherence": total["coherence"] / n,
        "stability": total["stability"] / n,
        "entropy": total["entropy"] / n,
        "energy": total["energy"] / n,
        "pattern_count": total["pattern_count"] / n,
    }


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Summarize SEP engine metrics")
    parser.add_argument("files", nargs="+", type=Path, help="JSON metric files")
    args = parser.parse_args()

    rows = parse_metrics_files(args.files)
    for row in rows:
        print(
            f"{row['file']}: coherence={row['coherence']:.4f} "
            f"stability={row['stability']:.4f} entropy={row['entropy']:.4f}"
        )
    avg = average_metrics(rows)
    print(
        f"Average coherence={avg['coherence']:.4f} "
        f"stability={avg['stability']:.4f} entropy={avg['entropy']:.4f} "
        f"energy={avg['energy']:.4f}"
    )
