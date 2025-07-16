#!/usr/bin/env python3
import os
import re
import networkx as nx
import matplotlib.pyplot as plt
from collections import defaultdict

class IncludeAnalyzer:
    def __init__(self, src_dir):
        self.src_dir = src_dir
        self.graph = nx.DiGraph()
        self.cycles = []

    def analyze(self):
        for root, dirs, files in os.walk(self.src_dir):
            for file in files:
                if file.endswith(('.h', '.hpp')):
                    filepath = os.path.join(root, file)
                    self._parse_includes(filepath)
        self.cycles = list(nx.simple_cycles(self.graph))
        return self

    def _parse_includes(self, filepath):
        relative_path = os.path.relpath(filepath, self.src_dir)
        self.graph.add_node(relative_path)
        with open(filepath, 'r') as f:
            content = f.read()
        include_pattern = r'#include\s*[<"]([^>"]+)[>"]'
        includes = re.findall(include_pattern, content)
        for include in includes:
            if not include.startswith('/'):
                include_path = os.path.normpath(os.path.join(os.path.dirname(relative_path), include))
                if os.path.exists(os.path.join(self.src_dir, include_path)):
                    self.graph.add_edge(relative_path, include_path)

    def report_cycles(self):
        print(f"Found {len(self.cycles)} circular dependencies:\n")
        for i, cycle in enumerate(self.cycles, 1):
            print(f"Cycle {i}:")
            for j in range(len(cycle)):
                print(f"  {cycle[j]} \u2192 {cycle[(j+1)%len(cycle)]}")
            print()

    def visualize(self, output_file='dependency_graph.svg'):
        plt.figure(figsize=(20,20))
        node_colors = []
        cyclic_nodes = set()
        for cycle in self.cycles:
            cyclic_nodes.update(cycle)
        for node in self.graph.nodes():
            node_colors.append('red' if node in cyclic_nodes else 'lightblue')
        pos = nx.spring_layout(self.graph, k=2, iterations=50)
        nx.draw(self.graph, pos, node_color=node_colors, with_labels=True,
                node_size=100, font_size=8, arrows=True, edge_color='gray')
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()

if __name__ == '__main__':
    analyzer = IncludeAnalyzer('src/')
    analyzer.analyze().report_cycles()
    analyzer.visualize()
