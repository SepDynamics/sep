import json
from pathlib import Path
from typing import List, Dict


def parse_metrics_files(files: List[Path]) -> List[Dict[str, float]]:
    """Parse JSON metrics produced by pattern_metric_example."""
    results = []
    for fp in files:
        try:
            data = json.loads(fp.read_text())
            metrics = data.get("metrics", {})
            results.append({
                "file": fp.name,
                "coherence": float(metrics.get("coherence", 0.0)),
                "stability": float(metrics.get("stability", 0.0)),
                "entropy": float(metrics.get("entropy", 0.0)),
                "pattern_count": int(data.get("pattern_count", 0)),
            })
        except Exception as exc:
            print(f"Failed to parse {fp}: {exc}")
    return results


def average_metrics(metrics: List[Dict[str, float]]) -> Dict[str, float]:
    if not metrics:
        return {"coherence": 0.0, "stability": 0.0, "entropy": 0.0}
    total = {"coherence": 0.0, "stability": 0.0, "entropy": 0.0}
    for m in metrics:
        total["coherence"] += m["coherence"]
        total["stability"] += m["stability"]
        total["entropy"] += m["entropy"]
    n = len(metrics)
    return {k: v / n for k, v in total.items()}


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
        f"stability={avg['stability']:.4f} entropy={avg['entropy']:.4f}"
    )
