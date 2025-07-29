import pytest
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from _sep.testbed.json_utils import parse_first_json

def test_parse_first_json_simple():
    text = "prefix {\"a\": 1} suffix"
    assert parse_first_json(text) == {"a": 1}


def test_parse_first_json_extra_data():
    text = "noise {\"a\": 1} trailing {\"b\":2}"
    assert parse_first_json(text) == {"a": 1}


def test_parse_first_json_missing():
    with pytest.raises(ValueError):
        parse_first_json("no json here")
