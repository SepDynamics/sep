#!/usr/bin/env python3
import pathlib
import sys

def check_pch_order():
    repo_root = pathlib.Path(__file__).resolve().parents[1]
    blender_dir = repo_root / "src" / "blender"
    ok = True
    for cpp in sorted(blender_dir.glob("*.cpp")):
        with open(cpp, 'r', encoding='utf-8') as f:
            first_line = f.readline().strip()
            if first_line != '#include "blender_pch.h"':
                print(f"{cpp}: first line is {first_line!r}, expected '#include \"blender_pch.h\"'")
                ok = False
    return ok

if __name__ == "__main__":
    if check_pch_order():
        print("All blender .cpp files include blender_pch.h first")
        sys.exit(0)
    else:
        sys.exit(1)
