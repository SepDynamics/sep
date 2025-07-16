#!/usr/bin/env python3
"""Analyze include dependencies in C/C++ headers.

This script detects circular include dependencies and can optionally
visualize the dependency graph.
"""

import argparse
import os
import re
from collections import defaultdict

try:
    import networkx as nx
    import matplotlib.pyplot as plt
except Exception:
    nx = None
    plt = None


class IncludeAnalyzer:
    def __init__(self, src_dir: str):
        self.src_dir = src_dir
        self.graph = nx.DiGraph() if nx else None
        self.cycles = []

    def analyze(self):
        if not nx:
            raise RuntimeError("networkx required for analysis")
        for root, _dirs, files in os.walk(self.src_dir):
            for file in files:
                if file.endswith((".h", ".hpp")):
                    self._parse_includes(os.path.join(root, file))
        self.cycles = list(nx.simple_cycles(self.graph))
        return self

    def _parse_includes(self, filepath: str) -> None:
        rel = os.path.relpath(filepath, self.src_dir)
        self.graph.add_node(rel)
        with open(filepath, "r", encoding="utf-8", errors="ignore") as f:
            content = f.read()
        pattern = r"#include\s*[<\"]([^>\"]+)[>\"]"
        for include in re.findall(pattern, content):
            if not include.startswith("/"):
                include_path = os.path.normpath(os.path.join(os.path.dirname(rel), include))
                if os.path.exists(os.path.join(self.src_dir, include_path)):
                    self.graph.add_edge(rel, include_path)

    def report_cycles(self) -> None:
        print(f"Found {len(self.cycles)} circular dependencies:\n")
        for idx, cycle in enumerate(self.cycles, 1):
            print(f"Cycle {idx}:")
            for i in range(len(cycle)):
                nxt = cycle[(i + 1) % len(cycle)]
                print(f"  {cycle[i]} \u2192 {nxt}")
            print()

    def visualize(self, output_file: str) -> None:
        if not plt:
            raise RuntimeError("matplotlib required for visualization")
        plt.figure(figsize=(20, 20))
        cyclic_nodes = {n for c in self.cycles for n in c}
        node_colors = ["red" if n in cyclic_nodes else "lightblue" for n in self.graph.nodes]
        pos = nx.spring_layout(self.graph, k=2, iterations=50)
        nx.draw(self.graph, pos, node_color=node_colors, with_labels=True,
                node_size=100, font_size=8, arrows=True, edge_color="gray")
        plt.savefig(output_file, dpi=300, bbox_inches="tight")
        plt.close()


def main() -> None:
    parser = argparse.ArgumentParser(description="Analyze C/C++ include dependencies")
    parser.add_argument("src", nargs="?", default="src/", help="source directory")
    parser.add_argument("--graph", dest="graph", metavar="PNG", nargs="?", const="dependency_graph.png",
                        help="output dependency graph as PNG")
    args = parser.parse_args()

    analyzer = IncludeAnalyzer(args.src)
    analyzer.analyze()
    analyzer.report_cycles()
    if args.graph:
        analyzer.visualize(args.graph)
        print(f"Graph written to {args.graph}")


if __name__ == "__main__":
    main()
