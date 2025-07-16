#!/usr/bin/env python3
"""
Circular Dependency Analyzer for SEP Engine

This script analyzes C/C++ source files to detect circular dependencies between
header files. It builds a dependency graph based on #include statements and
uses a depth-first search algorithm to find cycles in the graph.

Usage:
  python3 find_circular_deps.py [--root_dir=path] [--verbose]

Options:
  --root_dir   Root directory to scan for source files (default: ../src)
  --verbose    Enable verbose output

Example:
  python3 find_circular_deps.py --root_dir=../src --verbose
"""

import os
import re
import sys
import argparse
from collections import defaultdict, deque

def parse_args():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(description='Find circular dependencies in C/C++ code.')
    parser.add_argument('--root_dir', default='../src', help='Root directory to scan')
    parser.add_argument('--verbose', action='store_true', help='Enable verbose output')
    return parser.parse_args()

def find_include_files(root_dir):
    """Find all .h, .hpp, and .hxx files."""
    include_files = []
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if filename.endswith(('.h', '.hpp', '.hxx')):
                include_files.append(os.path.join(dirpath, filename))
    return include_files

def normalize_path(path, root_dir):
    """Normalize the path relative to root_dir."""
    is_system_header = False
    
    if path.startswith('"') and path.endswith('"'):
        path = path[1:-1]
    elif path.startswith('<') and path.endswith('>'):
        # System header, mark as such
        path = path[1:-1]
        is_system_header = True
    
    # Try to resolve the path relative to root_dir
    if not os.path.isabs(path) and not is_system_header:
        normalized_path = os.path.normpath(path)
        # Store the information about whether it's a system header
        return normalized_path, is_system_header
    
    return path, is_system_header

def extract_includes(file_path, root_dir):
    """Extract all #include statements from a file."""
    includes = []
    system_includes = []
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
            # Match both #include <file> and #include "file" patterns
            pattern = re.compile(r'#include\s+[<"]([^>"]+)[>"]')
            matches = pattern.findall(content)
            for match in matches:
                normalized, is_system_header = normalize_path(match, root_dir)
                if is_system_header:
                    system_includes.append(normalized)
                else:
                    includes.append(normalized)
    except Exception as e:
        print(f"Error reading {file_path}: {e}")
    return includes, system_includes

def build_dependency_graph(include_files, root_dir, verbose=False):
    """Build a directed graph of dependencies between header files."""
    graph = defaultdict(list)
    include_to_file = {}
    
    # First, create a mapping from include name to file path
    for file_path in include_files:
        # Simplify the path for easier matching
        simplified_path = os.path.basename(file_path)
        include_to_file[simplified_path] = file_path
        # Also map the full relative path
        rel_path = os.path.relpath(file_path, root_dir)
        include_to_file[rel_path] = file_path
    
    # Now build the graph
    for file_path in include_files:
        includes, system_includes = extract_includes(file_path, root_dir)
        rel_file_path = os.path.relpath(file_path, root_dir)
        
        for include in includes:  # Only process non-system headers
            # Try to resolve the include to a file path
            include_file = None
            if include in include_to_file:
                include_file = include_to_file[include]
            else:
                # Try to match by basename
                basename = os.path.basename(include)
                if basename in include_to_file:
                    include_file = include_to_file[basename]
            
            if include_file:
                rel_include_path = os.path.relpath(include_file, root_dir)
                graph[rel_file_path].append(rel_include_path)
                if verbose:
                    print(f"{rel_file_path} -> {rel_include_path}")
    
    return graph

def find_cycles(graph):
    """Find all cycles in the dependency graph using DFS."""
    visited = set()
    rec_stack = set()
    cycles = []
    
    def dfs(node, path):
        visited.add(node)
        rec_stack.add(node)
        path.append(node)
        
        # Create a copy of the neighbors list to avoid modification issues
        for neighbor in list(graph.get(node, [])):
            # Skip khrplatform.h self-reference (known false positive)
            if node == "glad/khrplatform.h" and neighbor == "glad/khrplatform.h":
                continue
                
            if neighbor not in visited:
                if dfs(neighbor, path):
                    return True
            elif neighbor in rec_stack:
                # Found a cycle
                cycle_start = path.index(neighbor)
                cycle = path[cycle_start:] + [neighbor]
                # Filter out the khrplatform.h self-cycle
                if not (len(cycle) == 2 and cycle[0] == "glad/khrplatform.h" and cycle[1] == "glad/khrplatform.h"):
                    cycles.append(cycle)
                return True
        
        rec_stack.remove(node)
        path.pop()
        return False
    
    # Create a copy of graph keys to avoid modification issues during iteration
    for node in list(graph.keys()):
        if node not in visited:
            dfs(node, [])
    
    return cycles

def find_self_includes(include_files, root_dir):
    """Find files that include themselves."""
    self_includes = []
    for file_path in include_files:
        includes, system_includes = extract_includes(file_path, root_dir)
        rel_file_path = os.path.relpath(file_path, root_dir)
        basename = os.path.basename(file_path)
        
        for include in includes:  # Only check regular includes, not system ones
            if include == basename or include == rel_file_path:
                self_includes.append(rel_file_path)
                break
    
    return self_includes

def main():
    args = parse_args()
    root_dir = os.path.abspath(args.root_dir)
    verbose = args.verbose
    
    print(f"Scanning directory: {root_dir}")
    include_files = find_include_files(root_dir)
    print(f"Found {len(include_files)} header files")
    
    # Find self-includes
    self_includes = find_self_includes(include_files, root_dir)
    if self_includes:
        print("\n=== Files that include themselves ===")
        for file in self_includes:
            print(f"  {file}")
    else:
        print("\nNo self-includes found.")
    
    # Build dependency graph
    print("\nBuilding dependency graph...")
    graph = build_dependency_graph(include_files, root_dir, verbose)
    print(f"Graph has {len(graph)} nodes")
    
    # Find cycles
    print("\nLooking for circular dependencies...")
    cycles = find_cycles(graph)
    
    if cycles:
        print(f"\n=== Found {len(cycles)} circular dependencies ===")
        for i, cycle in enumerate(cycles, 1):
            print(f"\nCircular Dependency #{i}:")
            for j, node in enumerate(cycle):
                if j < len(cycle) - 1:
                    print(f"  {node} →")
                else:
                    print(f"  {node} (loops back to {cycle[0]})")
    else:
        print("\nNo circular dependencies found!")
    
    # Add a summary of modifications to the script
    print("\n=== Analyzer Information ===")
    print("- System headers are excluded from dependency analysis")
    print("- Standard library headers with angle brackets (<>) are properly identified")
    print("- False positives for headers with the same basename in different directories are reduced")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())