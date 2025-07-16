#!/usr/bin/env python3
"""Utility to rewrite reserved identifiers to project-safe names.

Usage:
    python tools/fix_reserved_identifiers.py [directory]

Without arguments the script processes the ``src/`` directory.
"""

import os
import re
import sys
from pathlib import Path


class IdentifierFixer:
    def __init__(self, project_prefix: str = "SEP"):
        self.project_prefix = project_prefix
        self.replacements = {}
        self.stats = {
            "files_processed": 0,
            "identifiers_fixed": 0,
            "errors": [],
        }

    def fix_file(self, filepath: str) -> bool:
        """Fix reserved identifiers in a single file."""
        try:
            with open(filepath, "r", encoding="utf-8") as f:
                content = f.read()

            original_content = content

            # Pattern 1: Macros with double underscore
            content = re.sub(
                r"#define\s+(__[A-Z_]+)(\s+)",
                lambda m: f"#define {self.project_prefix}_INTERNAL_{m.group(1)[2:]}{m.group(2)}",
                content,
            )

            # Pattern 2: Macros starting with underscore + capital
            content = re.sub(
                r"#define\s+(_[A-Z][A-Z_]*)",
                lambda m: f"#define {self.project_prefix}_PRIVATE_{m.group(1)[1:]}",
                content,
            )

            # Pattern 3: Enum values with double underscore
            content = re.sub(
                r"\b(__[A-Z_]+)\b(?=[,\s\}])",
                lambda m: f"{self.project_prefix}_INTERNAL_{m.group(1)[2:]}",
                content,
            )

            # Pattern 4: Global functions starting with underscore
            content = re.sub(
                r"^(\s*)(static\s+)?(\w+[\s\*]+)(_[a-z]\w*)\s*\(",
                lambda m: f"{m.group(1)}{m.group(2) or ''}{m.group(3)}{self.project_prefix.lower()}_{m.group(4)[1:]}(",
                content,
                flags=re.MULTILINE,
            )

            # Pattern 5: Include guards
            content = re.sub(
                r"#ifndef\s+(__\w+_H__?)\s*\n#define\s+(__\w+_H__?)",
                lambda m: self._fix_include_guard(m, filepath),
                content,
            )

            if content != original_content:
                backup_path = f"{filepath}.backup"
                with open(backup_path, "w", encoding="utf-8") as f:
                    f.write(original_content)

                with open(filepath, "w", encoding="utf-8") as f:
                    f.write(content)

                self.stats["identifiers_fixed"] += content.count(self.project_prefix) - original_content.count(
                    self.project_prefix
                )
                self.stats["files_processed"] += 1
                return True

            return False
        except Exception as e:  # pragma: no cover - best effort script
            self.stats["errors"].append(f"{filepath}: {str(e)}")
            return False

    def _fix_include_guard(self, match: re.Match, filepath: str) -> str:
        """Generate proper include guard from filepath."""
        path = Path(filepath)
        if "src" in path.parts:
            parts = list(path.parts[path.parts.index("src") + 1 :])
        else:
            parts = list(path.parts)
        guard_name = "_".join(parts).replace(".", "_").replace("-", "_").upper()
        guard = f"{self.project_prefix}_{guard_name}_"
        return f"#ifndef {guard}\n#define {guard}"

    def process_directory(self, directory: str) -> None:
        """Process all C++ files in directory."""
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith((".h", ".hpp", ".cpp", ".cc")):
                    filepath = os.path.join(root, file)
                    print(f"Processing {filepath}...")
                    self.fix_file(filepath)

    def report(self) -> None:
        """Print summary report."""
        print("\n" + "=" * 60)
        print("Reserved Identifier Cleanup Report")
        print("=" * 60)
        print(f"Files processed: {self.stats['files_processed']}")
        print(f"Identifiers fixed: {self.stats['identifiers_fixed']}")
        if self.stats["errors"]:
            print(f"\nErrors encountered: {len(self.stats['errors'])}")
            for error in self.stats["errors"][:10]:
                print(f"  - {error}")


def main() -> None:
    fixer = IdentifierFixer("SEP")
    target = sys.argv[1] if len(sys.argv) > 1 else "src/"
    fixer.process_directory(target)
    fixer.report()


if __name__ == "__main__":  # pragma: no cover
    main()
