import re
import sys
from pathlib import Path

pattern = re.compile(r'\b(struct|class)\s+(\w+)\s*\{')

names = {}
root_dirs = [Path('src'), Path('examples'), Path('tests'), Path('_sep/testbed')]
for d in root_dirs:
    if not d.exists():
        continue
    for path in d.rglob('*.[ch]pp'):
        text = path.read_text(errors='ignore')
        for match in pattern.finditer(text):
            name = match.group(2)
            names.setdefault(name, set()).add(str(path))

duplicates = {k:v for k,v in names.items() if len(v) > 1}
if duplicates:
    for name, files in duplicates.items():
        print(f'Duplicate definition: {name}')
        for f in files:
            print(f'  - {f}')
    sys.exit(1)

print('No duplicate struct/class definitions found.')
