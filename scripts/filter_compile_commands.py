#!/usr/bin/env python3
"""
Filter compile_commands.json to exclude external dependencies

This script creates a filtered version of compile_commands.json that only
includes internal source files, dramatically reducing static analysis time
and noise from external dependencies.
"""

import json
import argparse
from pathlib import Path

def should_skip_file(file_path: str) -> bool:
    """Check if a file should be skipped based on path patterns."""
    
    # External dependency patterns that generate 96% of the noise
    skip_patterns = [
        'build/_deps/',
        'extern/',
        'third_party/',
        '_deps/',
        'imgui-src/',
        'implot-src/',
        'yaml-cpp-src/',
        'spdlog-src/',
        'tbb-src/',
        'glfw-src/',
        'CMakeFiles/',
        'cmake-build-',
    ]
    
    # Check if any skip pattern matches
    for pattern in skip_patterns:
        if pattern in file_path:
            return True
    
    return False

def filter_compile_commands(input_file: str, output_file: str) -> tuple[int, int]:
    """Filter compile_commands.json and return (total, kept) counts."""
    
    with open(input_file, 'r') as f:
        commands = json.load(f)
    
    total_count = len(commands)
    filtered_commands = []
    
    for cmd in commands:
        file_path = cmd.get('file', '')
        if not should_skip_file(file_path):
            filtered_commands.append(cmd)
    
    with open(output_file, 'w') as f:
        json.dump(filtered_commands, f, indent=2)
    
    kept_count = len(filtered_commands)
    return total_count, kept_count

def main():
    parser = argparse.ArgumentParser(description='Filter compile_commands.json to exclude external dependencies')
    parser.add_argument('--input', '-i', default='compile_commands.json',
                       help='Input compile_commands.json file (default: compile_commands.json)')
    parser.add_argument('--output', '-o', default='compile_commands_filtered.json',
                       help='Output filtered file (default: compile_commands_filtered.json)')
    parser.add_argument('--preview', '-p', action='store_true',
                       help='Preview which files would be kept/skipped without writing output')
    
    args = parser.parse_args()
    
    if not Path(args.input).exists():
        print(f"Error: {args.input} not found. Please run ./build.sh first.")
        return 1
    
    if args.preview:
        # Preview mode - show what would be filtered
        with open(args.input, 'r') as f:
            commands = json.load(f)
        
        kept_files = []
        skipped_files = []
        
        for cmd in commands:
            file_path = cmd.get('file', '')
            if should_skip_file(file_path):
                skipped_files.append(file_path)
            else:
                kept_files.append(file_path)
        
        print(f"Preview of filtering {args.input}:")
        print(f"  Total files: {len(commands)}")
        print(f"  Files to keep: {len(kept_files)}")
        print(f"  Files to skip: {len(skipped_files)}")
        print(f"  Reduction: {len(skipped_files)/len(commands)*100:.1f}%")
        
        print(f"\nFiles to keep (sample):")
        for f in kept_files[:10]:
            print(f"  ✓ {f}")
        if len(kept_files) > 10:
            print(f"  ... and {len(kept_files)-10} more")
        
        print(f"\nFiles to skip (sample):")
        for f in skipped_files[:10]:
            print(f"  ✗ {f}")
        if len(skipped_files) > 10:
            print(f"  ... and {len(skipped_files)-10} more")
        
    else:
        # Actually filter and write output
        total, kept = filter_compile_commands(args.input, args.output)
        reduction = (total - kept) / total * 100 if total > 0 else 0
        
        print(f"Filtered {args.input} -> {args.output}")
        print(f"  Total files: {total}")
        print(f"  Kept files: {kept}")
        print(f"  Skipped files: {total - kept}")
        print(f"  Reduction: {reduction:.1f}%")
        
        if reduction > 90:
            print(f"\n🎉 Excellent! Reduced analysis scope by {reduction:.1f}%")
            print("This should dramatically speed up CodeChecker analysis.")

if __name__ == '__main__':
    exit(main() or 0)
