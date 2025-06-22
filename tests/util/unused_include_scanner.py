#!/usr/bin/env python3
"""Simple include usage scanner.

Scans source files for `#include` directives and attempts to identify
includes that may be unused. The heuristic is intentionally lightweight:
if the basename of an included header (without extension) only appears on
the `#include` line, the include is flagged as potentially unused.

The script also reports header files that are not referenced anywhere in
the scanned directories.

Usage:
    python unused_include_scanner.py <path> [<path> ...]

Each path may be a directory or a single file. Directories are scanned
recursively for `*.cpp`, `*.hpp` and `*.h` files.
"""

from __future__ import annotations

import argparse
import os
import re
from typing import Iterable, Set, Tuple

INCLUDE_RE = re.compile(r'^\s*#include\s+[<"]([^">]+)[">]')


def iter_source_files(paths: Iterable[str]) -> Iterable[str]:
    for p in paths:
        if os.path.isfile(p):
            yield p
        else:
            for root, _, files in os.walk(p):
                for f in files:
                    if f.endswith(('.cpp', '.hpp', '.h', '.cu')):
                        yield os.path.join(root, f)


def find_unused_includes(path: str) -> Set[str]:
    unused: Set[str] = set()
    try:
        with open(path, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
    except OSError:
        return unused

    includes: Tuple[Tuple[int, str], ...] = tuple(
        (idx, m.group(1))
        for idx, line in enumerate(lines)
        if (m := INCLUDE_RE.match(line))
    )
    if not includes:
        return unused

    text = ''.join(lines)
    for idx, inc in includes:
        base = os.path.basename(inc)
        name, _ = os.path.splitext(base)
        if text.count(name) == 1:
            unused.add(inc)
    return unused


def gather_all_includes(paths: Iterable[str]) -> Set[str]:
    includes: Set[str] = set()
    for src in iter_source_files(paths):
        try:
            with open(src, 'r', encoding='utf-8', errors='ignore') as f:
                for line in f:
                    m = INCLUDE_RE.match(line)
                    if m:
                        includes.add(m.group(1))
        except OSError:
            continue
    return includes


def gather_all_headers(paths: Iterable[str]) -> Set[str]:
    headers: Set[str] = set()
    for p in paths:
        for root, _, files in os.walk(p):
            for f in files:
                if f.endswith(('.h', '.hpp')):
                    headers.add(os.path.relpath(os.path.join(root, f)))
    return headers


def main() -> None:
    parser = argparse.ArgumentParser(description='Scan for unused includes.')
    parser.add_argument('paths', nargs='+', help='Paths to scan')
    args = parser.parse_args()

    for src in iter_source_files(args.paths):
        unused = find_unused_includes(src)
        if unused:
            print(f'In {src}:')
            for inc in sorted(unused):
                print(f'  Possibly unused include: {inc}')

    all_headers = gather_all_headers(args.paths)
    all_includes = gather_all_includes(args.paths)
    unreferenced = {
        h for h in all_headers
        if h not in all_includes and os.path.basename(h) not in {
            os.path.basename(i) for i in all_includes
        }
    }
    if unreferenced:
        print('\nUnreferenced headers:')
        for h in sorted(unreferenced):
            print(f'  {h}')


if __name__ == '__main__':
    main()
