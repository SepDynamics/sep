import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from _sep.testbed.engine_metrics_parser import parse_metrics_files, average_metrics


def test_parser_and_average(tmp_path):
    file1 = tmp_path / "m1.json"
    file2 = tmp_path / "m2.json"
    file1.write_text(json.dumps({
        "metrics": {"coherence": 0.5, "stability": 0.6, "entropy": 0.3, "energy": 1.2},
        "pattern_count": 4
    }))
    file2.write_text(json.dumps({
        "metrics": {"coherence": 0.7, "stability": 0.8, "entropy": 0.2, "energy": 2.8},
        "pattern_count": 6
    }))

    rows = parse_metrics_files([file1, file2])
    assert len(rows) == 2
    avg = average_metrics(rows)
    assert avg["coherence"] == (0.5 + 0.7) / 2
    assert avg["stability"] == (0.6 + 0.8) / 2
    assert avg["entropy"] == (0.3 + 0.2) / 2
    assert avg["energy"] == (1.2 + 2.8) / 2
    assert avg["pattern_count"] == (4 + 6) / 2
