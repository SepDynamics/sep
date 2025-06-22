#!/usr/bin/env python3
import re
import sys
from pathlib import Path

base_dir = Path(__file__).resolve().parents[2]
files = [
    base_dir / 'src/core/engine.cpp',
    base_dir / 'src/context/context_module.cpp',
    base_dir / 'src/memory/manager.cpp',
    base_dir / 'src/persistence/redis_manager.cpp'
]

pattern = re.compile(r'//.*?$|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S | re.M)

for f in files:
    text = f.read_text()
    stripped = re.sub(pattern, '', text)
    if stripped.count('{') != stripped.count('}'):
        print(f'Brace mismatch in {f}')
        sys.exit(1)
print('Brace check passed')
