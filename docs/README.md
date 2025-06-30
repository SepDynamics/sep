# SEP Engine Documentation

This folder contains notes and references for navigating and maintaining the SEP Engine. Everything here is written for my own use, so it focuses on where things live and how the pieces fit together.

- **`STRUCTURE.md`** — Quick guide to the top level directory layout.
- **`ARCHITECTURE.md`** — Mermaid diagrams and descriptions of each engine module.
- **`GAMEPLAN.md`** — Historical build issues and how they were resolved.
- **`THESIS.md`** — Background theory behind the project.
- **`vscodium.md`** — Notes on the development environment setup.

Most documentation assumes the code has already been built with CUDA support and that `sep_engine` runs. See below for a refresher on building and running.

## Build Recap

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

The resulting executable lives in `build/sep_engine`. Additional static libraries for each module are produced in the same directory.

## Running the Engine

Execute the engine from the build directory:

```bash
./sep_engine
```

Configuration files are located in `config`. Command‑line flags and environment variables override these defaults.

### New Configuration Sections

`memory` and `quantum` sections expose promotion and coherence thresholds. Example:

```json
"memory": {
    "promote_stm_to_mtm": 0.7,
    "promote_mtm_to_ltm": 0.9,
    "demote_threshold": 0.3
},
"quantum": {
    "ltm_coherence_threshold": 0.9,
    "mtm_coherence_threshold": 0.6,
    "stability_threshold": 0.8
}
```

## Tests

The `/tests` directory contains a minimal suite. Enable it in CMake with:

```bash
cmake .. -DSEP_BUILD_TESTS=ON
make -j$(nproc)
ctest
```

Refer to `STRUCTURE.md` whenever you need a reminder of where things are.
