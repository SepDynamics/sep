#!/usr/bin/env python3
"""Check diagram documentation freshness.

This worker scans documentation files in ``docs/diagrams`` and reports any that
appear outdated relative to the source tree. It compares the modification time
of each diagram sheet against the latest changes in the corresponding module's
``include`` or ``src`` directory. If a source file is newer than its diagram,
the script prints a warning so documentation can be updated.
"""

from __future__ import annotations

import datetime
import os
from pathlib import Path
from typing import Iterable

MODULES = [
    "api",
    "audio",
    "blender",
    "compat",
    "core",
    "crow",
    "memory",
    "quantum",
]

DOC_DIR = Path("docs/diagrams")
INCLUDE_DIR = Path("include")
SRC_DIR = Path("src")


def newest_mtime(paths: Iterable[Path]) -> float:
    times = [p.stat().st_mtime for p in paths if p.is_file()]
    return max(times) if times else 0.0


def scan_module(module: str) -> None:
    include_doc = DOC_DIR / f"include-{module}.md"
    src_doc = DOC_DIR / f"src-{module}.md"

    include_path = INCLUDE_DIR / module
    src_path = SRC_DIR / module

    if include_doc.exists() and include_path.exists():
        latest = newest_mtime(include_path.rglob("*"))
        if latest > include_doc.stat().st_mtime:
            ts = datetime.datetime.fromtimestamp(include_doc.stat().st_mtime)
            print(f"[outdated] {include_doc} last updated {ts:%Y-%m-%d}")

    if src_doc.exists() and src_path.exists():
        latest = newest_mtime(src_path.rglob("*"))
        if latest > src_doc.stat().st_mtime:
            ts = datetime.datetime.fromtimestamp(src_doc.stat().st_mtime)
            print(f"[outdated] {src_doc} last updated {ts:%Y-%m-%d}")


def main() -> None:
    for mod in MODULES:
        scan_module(mod)


if __name__ == "__main__":
    main()
