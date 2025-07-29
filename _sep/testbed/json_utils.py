import json
import re
from typing import Any

def parse_first_json(text: str) -> Any:
    """Extract and parse the first JSON object found in text."""
    match = re.search(r"\{.*\}", text, re.DOTALL)
    if not match:
        raise ValueError("No JSON object found")
    return json.loads(match.group(0))
