import re
from collections import defaultdict
import pathlib

report_path = pathlib.Path('docs/report.md')
summary_path = pathlib.Path('docs/ARCH/condensed_report.md')
breakdown_path = pathlib.Path('docs/ARCH/issue_breakdown.md')

pattern = re.compile(r'^\[(?P<severity>[^\]]+)\]\s+(?P<file>[^:]+):(?P<line>\d+):(?P<col>\d+):\s+(?P<message>.*)\s\[(?P<rule>[^\]]+)\]')

severity_counts = defaultdict(int)
file_counts = defaultdict(int)
folder_counts = defaultdict(int)
unique_entries = defaultdict(int)

total = 0
with report_path.open('r', errors='ignore') as f:
    for line in f:
        m = pattern.match(line)
        if m:
            total += 1
            sev = m.group('severity')
            file = m.group('file')
            msg = m.group('message')
            rule = m.group('rule')
            severity_counts[sev] += 1
            file_counts[file] += 1
            parts = file.replace('/sep/', '').split('/')
            folder = '/'.join(parts[:2]) if len(parts) > 1 else parts[0]
            folder_counts[folder] += 1
            unique_entries[(file, msg, rule)] += 1

unique_issue_count = len(unique_entries)

# Compose markdown
lines = []
lines.append('# Condensed Static Analysis Report\n')
lines.append(f'Original reports processed: {total}\n')
lines.append(f'Unique issues identified: {unique_issue_count}\n')
lines.append('\n## Issues by Severity\n')
lines.append('| Severity | Count |\n|---|---|')
for sev, count in sorted(severity_counts.items(), key=lambda x: x[0]):
    lines.append(f'| {sev} | {count} |')

lines.append('\n## Top Files by Issue Count\n')
lines.append('| File | Count |\n|---|---|')
for file, count in sorted(file_counts.items(), key=lambda x: x[1], reverse=True)[:20]:
    short = file.replace('/sep/', '')
    lines.append(f'| {short} | {count} |')

summary_path.write_text('\n'.join(lines))

# Issue breakdown by top-level folders for diagnostics
b_lines = []
b_lines.append('# Issue Breakdown by Folder\n')
b_lines.append('| Folder | Count |\n|---|---|')
for folder, count in sorted(folder_counts.items(), key=lambda x: x[1], reverse=True):
    b_lines.append(f'| {folder} | {count} |')

breakdown_path.write_text('\n'.join(b_lines))

print(f'Wrote summary to {summary_path}')
print(f'Wrote folder breakdown to {breakdown_path}')
