#!/usr/bin/env python3
import os
import re
import sys
from collections import defaultdict

SEARCH_DIRS = ['src', 'examples', 'tests', '_sep/testbed']
EXTS = ('.h', '.hpp', '.c', '.cpp', '.cu', '.cc', '.cxx')

def gather_definitions():
    pattern = re.compile(r'^\s*(struct|class)\s+([A-Za-z_]\w*)\s*[{:]')
    defs = defaultdict(list)
    for d in SEARCH_DIRS:
        if not os.path.isdir(d):
            continue
        for root, _, files in os.walk(d):
            for name in files:
                if name.endswith(EXTS):
                    path = os.path.join(root, name)
                    try:
                        with open(path, 'r', encoding='utf-8', errors='ignore') as f:
                            for i, line in enumerate(f):
                                m = pattern.match(line)
                                if m:
                                    defs[m.group(2)].append(f"{path}:{i+1}")
                    except Exception:
                        pass
    return defs

def load_baseline(path):
    if not os.path.exists(path):
        return set()
    with open(path, 'r', encoding='utf-8') as f:
        return {line.strip() for line in f if line.strip()}

def main():
    defs = gather_definitions()
    duplicates = {n: locs for n, locs in defs.items() if len(locs) > 1}
    baseline = load_baseline('scripts/duplicate_type_baseline.txt')
    new_dups = {n: locs for n, locs in duplicates.items() if n not in baseline}
    if new_dups:
        print('Duplicate type definitions found:')
        for name, locs in sorted(new_dups.items()):
            print(f'- {name}:')
            for loc in locs:
                print(f'  - {loc}')
        sys.exit(1)
    else:
        print('No new duplicate types found.')

if __name__ == '__main__':
    main()
