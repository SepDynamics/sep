# Build Directory Cleanup

This repository occasionally accumulates CMake build outputs when local builds are performed. These files are not required in version control and should be removed if committed by mistake.

## Common artifacts

- `cmake_install.cmake`
- `build.ninja`
- `CMakeCache.txt`
- `CMakeFiles/` directories

## Cleanup procedure

1. Search the repository for these files:
   ```bash
   find . -name 'cmake_install.cmake' -o -name 'build.ninja' -o -name 'CMakeCache.txt'
   ```
2. Delete any matches from the repository and commit the changes.
3. Ensure `.gitignore` contains entries for the above artifacts so they are not re‑added.

Currently the repository contains no such build outputs.
