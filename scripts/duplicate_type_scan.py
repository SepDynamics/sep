#!/usr/bin/env python3
import os
import re
import sys

SEARCH_ROOTS = ['src', 'examples', 'tests', '_sep/testbed']
EXTS = {'.h', '.hpp', '.c', '.cpp', '.cc', '.cu', '.ts', '.js'}

pattern = re.compile(r'\b(struct|class)\s+([A-Za-z_][A-Za-z0-9_]*)')

def scan_file(path):
    names = set()
    try:
        with open(path, 'r', encoding='utf-8', errors='ignore') as f:
            for line in f:
                m = pattern.search(line)
                if m:
                    names.add(m.group(2))
    except Exception:
        pass
    return names

def main():
    occurrences = {}
    duplicates = {}
    for root in SEARCH_ROOTS:
        if not os.path.isdir(root):
            continue
        for dirpath, _, files in os.walk(root):
            for fname in files:
                if any(fname.endswith(ext) for ext in EXTS):
                    path = os.path.join(dirpath, fname)
                    for name in scan_file(path):
                        if name in occurrences:
                            duplicates.setdefault(name, set()).update({occurrences[name], path})
                        else:
                            occurrences[name] = path
    if duplicates:
        print("Duplicate struct/class definitions found:")
        for name, paths in sorted(duplicates.items()):
            print(f"{name}:")
            for p in sorted(paths):
                print(f"  {p}")
        sys.exit(1)
    else:
        print("No duplicate struct/class definitions found.")

if __name__ == '__main__':
    main()
