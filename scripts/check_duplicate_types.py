import os
import re
import sys
from collections import defaultdict

def gather_files(root_dirs):
    for root_dir in root_dirs:
        for dirpath, _, files in os.walk(root_dir):
            for fname in files:
                if fname.endswith(('.h', '.hpp', '.c', '.cpp', '.cc', '.cu')):
                    yield os.path.join(dirpath, fname)

def extract_definitions(path):
    pattern = re.compile(r'^(?:template<[^>]+>\s*)?(struct|class)\s+(\w+)\s*(?:\{|$)')
    results = []
    with open(path, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            m = pattern.match(line.strip())
            if m and '{' in line:
                results.append(m.group(2))
    return results

def main():
    roots = sys.argv[1:] if len(sys.argv) > 1 else ['include', 'src', 'examples', 'tests', '_sep/testbed']
    defs = defaultdict(list)
    for file in gather_files(roots):
        for name in extract_definitions(file):
            defs[name].append(file)

    duplicates = {n: paths for n, paths in defs.items() if len(paths) > 1}
    if duplicates:
        print('Duplicate struct/class definitions found:')
        for name, paths in duplicates.items():
            print(f'{name}:')
            for p in paths:
                print(f'  - {p}')
        sys.exit(1)
    else:
        print('No duplicate struct/class definitions found.')

if __name__ == '__main__':
    main()
