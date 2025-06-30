# Repository Layout

This file describes the key directories and files in the project. It serves as a personal reference so you can quickly locate components when working on the engine.

## Top-Level Directories

| Path | Purpose |
| ---- | ------- |
| `src` | Source code for the SEP engine. Contains modules such as `core`, `api`, `memory`, `quantum`, `blender`, `audio`, and `compat`. |
| `include` | Public header files mirrored from `src` for external use. Follows the same module structure. |
| `tests` | Test suite for validating engine functionality. |
| `extern` | Third‑party dependencies. Currently only the Cycles rendering library placeholder lives here. |
| `assets` | Sample assets used for development and rendering tests. |
| `docs` | Documentation folder (this folder). Holds architecture notes, diagrams, and other reference material. |
| `cmake` | CMake modules and helper scripts used during the build process. |
| `config` | Default configuration files. |
| `scripts` | Utility scripts for development and CI tasks. |

## Build Outputs

The default build directory is `build`. After compilation you'll find the `sep_engine` executable alongside generated libraries.

## Where to Start

1. `src/main.cpp` – entry point that sets up the engine.
2. `include` headers – look here when integrating the engine with other projects.
3. `docs/ARCHITECTURE.md` – high level component breakdown.

Keep this file handy as a quick guide when navigating the repository.
