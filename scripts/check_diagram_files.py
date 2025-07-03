#!/usr/bin/env python3
import os
import re
import sys
from collections import defaultdict

HEADER_EXTS = {'.h', '.hpp'}
SOURCE_EXTS = {'.c', '.cpp', '.cu'}


def gather_files(root, exts):
    mapping = defaultdict(list)
    for dirpath, _, files in os.walk(root):
        for name in files:
            if any(name.endswith(ext) for ext in exts):
                rel = os.path.relpath(os.path.join(dirpath, name), '.')
                mapping[name].append(rel)
    return mapping


def parse_docs(path_pattern):
    doc_names = set()
    regex = re.compile(r"([\w/.-]+\.(?:h|hpp|c|cpp|cu))")
    for md_path in glob.glob(path_pattern):
        with open(md_path, 'r', encoding='utf-8') as f:
            text = f.read()
            for match in regex.findall(text):
                base = os.path.basename(match)
                doc_names.add(base)
    return doc_names


def main():
    include_files = gather_files('include', HEADER_EXTS)
    src_files = gather_files('src', SOURCE_EXTS)

    repo_basenames = set(include_files.keys()) | set(src_files.keys())

    doc_names = parse_docs('docs/diagrams/*.md')

    missing_in_repo = sorted(doc_names - repo_basenames)
    undocumented = sorted(repo_basenames - doc_names)

    if missing_in_repo:
        print('Names referenced in docs but missing in repo:')
        for name in missing_in_repo:
            print(f'- {name}')
    else:
        print('All names referenced in docs exist in repo.')

    if undocumented:
        print('\nNames present in repo but not referenced in docs:')
        for name in undocumented:
            print(f'- {name}')
    else:
        print('\nAll repo files are referenced in docs.')

    if missing_in_repo or undocumented:
        sys.exit(1)


if __name__ == '__main__':
    import glob
    main()
