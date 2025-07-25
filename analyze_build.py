#!/usr/bin/env python3
import subprocess
import sys

# Run build script
print("Running build.sh...")
result = subprocess.run(['./build.sh'], capture_output=True, text=True)

# Combine stdout and stderr
full_output = result.stdout + result.stderr

# Extract error lines
errors = []
for line in full_output.splitlines():
    if 'error' in line.lower():
        errors.append(line.strip())

# Remove duplicates while preserving order
unique_errors = list(dict.fromkeys(errors))

print(f"\n\n{'='*80}")
print(f"BUILD ANALYSIS RESULTS")
print(f"{'='*80}\n")

if unique_errors:
    print(f"Found {len(unique_errors)} unique errors:\n")
    for i, error in enumerate(unique_errors, 1):
        print(f"{i}. {error}")
        print()
else:
    print("No errors found in build output.")

print(f"\n{'='*80}")
print(f"FULL BUILD OUTPUT")
print(f"{'='*80}\n")
print(full_output)
