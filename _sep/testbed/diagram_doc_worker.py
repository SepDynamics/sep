#!/usr/bin/env python3
"""Diagram Documentation Worker

This script scans module directories under `include/` and `src/` and verifies
that corresponding diagram documentation files exist under `docs/diagrams`.
Missing or outdated files are reported for manual review.

Usage:
    python _sep/testbed/diagram_doc_worker.py
"""
from __future__ import annotations

import os
from pathlib import Path


def list_modules(base: Path) -> list[str]:
    """Return directory names under *base* that represent modules."""
    modules = []
    for item in sorted(base.iterdir()):
        if item.is_dir() and not item.is_symlink():
            modules.append(item.name)
    return modules


def collect_docs(doc_dir: Path, prefix: str) -> dict[str, Path]:
    """Return a mapping of module name to documentation path."""
    docs = {}
    for doc in doc_dir.glob(f"{prefix}-*.md"):
        name = doc.stem[len(prefix) + 1 :]
        docs[name] = doc
    return docs


def main() -> None:
    repo_root = Path(__file__).resolve().parents[2]
    docs_dir = repo_root / "docs" / "diagrams"

    include_modules = list_modules(repo_root / "include")
    src_modules = list_modules(repo_root / "src")

    include_docs = collect_docs(docs_dir, "include")
    src_docs = collect_docs(docs_dir, "src")

    missing = []
    for name in include_modules:
        if name not in include_docs:
            missing.append(f"docs/diagrams/include-{name}.md")
    for name in src_modules:
        if name not in src_docs:
            missing.append(f"docs/diagrams/src-{name}.md")

    outdated = []
    for name, path in include_docs.items():
        if name not in include_modules:
            outdated.append(str(path.relative_to(repo_root)))
    for name, path in src_docs.items():
        if name not in src_modules:
            outdated.append(str(path.relative_to(repo_root)))

    if not missing and not outdated:
        print("Diagram documentation is up to date.")
        return

    if missing:
        print("Missing diagram docs:")
        for p in missing:
            print(f"  {p}")

    if outdated:
        print("Outdated diagram docs:")
        for p in outdated:
            print(f"  {p}")


if __name__ == "__main__":
    main()
