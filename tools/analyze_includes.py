#!/usr/bin/env python3
"""Analyze C/C++ includes for circular dependencies and visualize the graph."""

import os
import re
from collections import defaultdict
import networkx as nx
import matplotlib.pyplot as plt

class IncludeAnalyzer:
    def __init__(self, src_dir: str):
        self.src_dir = src_dir
        self.graph = nx.DiGraph()
        self.cycles = []

    def analyze(self):
        for root, _dirs, files in os.walk(self.src_dir):
            for file in files:
                if file.endswith(('.h', '.hpp')):
                    self._parse_includes(os.path.join(root, file))
        self.cycles = list(nx.simple_cycles(self.graph))
        return self

    def _parse_includes(self, filepath: str):
        relative = os.path.relpath(filepath, self.src_dir)
        self.graph.add_node(relative)
        with open(filepath, 'r', errors='ignore') as f:
            content = f.read()
        pattern = r'#include\s*[<"]([^>"]+)[>"]'
        for include in re.findall(pattern, content):
            if not include.startswith('/'):
                path = os.path.normpath(os.path.join(os.path.dirname(relative), include))
                if os.path.exists(os.path.join(self.src_dir, path)):
                    self.graph.add_edge(relative, path)

    def report_cycles(self):
        print(f"Found {len(self.cycles)} circular dependencies:\n")
        for i, cycle in enumerate(self.cycles, 1):
            print(f"Cycle {i}:")
            for j in range(len(cycle)):
                print(f"  {cycle[j]} -> {cycle[(j + 1) % len(cycle)]}")
            print()

    def visualize(self, output_file: str = 'dependency_graph.png'):
        plt.figure(figsize=(20, 20))
        cyclic_nodes = set(n for c in self.cycles for n in c)
        colors = ['red' if n in cyclic_nodes else 'lightblue' for n in self.graph.nodes()]
        pos = nx.spring_layout(self.graph, k=2, iterations=50)
        nx.draw(self.graph, pos, node_color=colors, with_labels=True, node_size=100, font_size=8,
                arrows=True, edge_color='gray')
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()

if __name__ == '__main__':
    src = os.path.join(os.path.dirname(__file__), '..', 'src')
    IncludeAnalyzer(src).analyze().report_cycles()
