#!/usr/bin/env python3
"""Helper script to verify diagram files match the source tree.

This worker scans the ``include`` and ``src`` directories and ensures a
corresponding Markdown file exists under ``docs/diagrams`` for each
module. Diagram files follow the pattern ``include-<name>.md`` or
``src-<name>.md``.

If ``--regen`` is supplied missing diagram files are generated with a
placeholder header so documentation can be filled in later.
"""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import Iterable, List, Tuple


def _gather_module_names(directory: Path) -> List[str]:
    return sorted(p.name for p in directory.iterdir() if p.is_dir())


def _gather_doc_names(prefix: str, docs_dir: Path) -> dict[str, Path]:
    docs: dict[str, Path] = {}
    for path in docs_dir.glob(f"{prefix}-*.md"):
        name = path.stem[len(prefix) + 1 :]
        docs[name] = path
    return docs


def _generate_baseline(path: Path, prefix: str, name: str) -> None:
    kind = "Header" if prefix == "include" else "Source"
    content = (
        f"# {name.capitalize()} {kind} Overview\n\n"
        f"This is an auto-generated placeholder for the `{prefix}/{name}` module.\n"
    )
    path.write_text(content, encoding="utf-8")


def _check_modules(
    prefix: str, modules: Iterable[str], docs_dir: Path, regen: bool
) -> Tuple[List[str], List[str]]:
    docs = _gather_doc_names(prefix, docs_dir)
    missing: List[str] = []
    extra: List[str] = []

    for mod in modules:
        if mod not in docs:
            missing.append(mod)
            if regen:
                _generate_baseline(docs_dir / f"{prefix}-{mod}.md", prefix, mod)

    for doc_name in docs.keys():
        if doc_name not in modules:
            extra.append(doc_name)

    return missing, extra


def main(argv: List[str] | None = None) -> None:
    parser = argparse.ArgumentParser(
        description="Validate docs/diagrams against module directories"
    )
    parser.add_argument(
        "--regen",
        action="store_true",
        help="Create placeholder docs for missing modules",
    )
    args = parser.parse_args(argv)

    root = Path(__file__).resolve().parents[2]
    docs_dir = root / "docs" / "diagrams"
    include_dir = root / "include"
    src_dir = root / "src"

    include_modules = _gather_module_names(include_dir)
    src_modules = [d.name for d in src_dir.iterdir() if d.is_dir() and d.name != "tests"]

    missing_includes, extra_includes = _check_modules(
        "include", include_modules, docs_dir, args.regen
    )
    missing_src, extra_src = _check_modules("src", src_modules, docs_dir, args.regen)

    if any([missing_includes, extra_includes, missing_src, extra_src]):
        print("Diagram sync issues found:")
        if missing_includes:
            print("  Missing include docs:", ", ".join(missing_includes))
        if extra_includes:
            print("  Extra include docs:", ", ".join(extra_includes))
        if missing_src:
            print("  Missing src docs:", ", ".join(missing_src))
        if extra_src:
            print("  Extra src docs:", ", ".join(extra_src))
    else:
        print("Diagrams are in sync.")


if __name__ == "__main__":
    main()
