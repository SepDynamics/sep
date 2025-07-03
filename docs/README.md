# SEP Engine Documentation

This folder contains notes and references for navigating and maintaining the SEP Engine. Everything here is written for my own use, so it focuses on where things live and how the pieces fit together.

- **`STRUCTURE.md`** — Quick guide to the top level directory layout.
- **`ARCHITECTURE.md`** — Mermaid diagrams and descriptions of each engine module.
- **`GAMEPLAN.md`** — Historical build issues and how they were resolved.
- **`THESIS.md`** — Background theory behind the project.
- **`strategy/SCIENTIFIC_VISUALIZATION_MARKET.md`** — Research market framing for the SEP Engine.
- **`vscodium.md`** — Notes on the development environment setup.
- **`CONFIG_OPTIONS.md`** — Description of configurable runtime parameters.
- **`diagrams/include-embeddings.md`** — Header map for the embeddings module.
- **`diagrams/src-embeddings.md`** — Implementation flow for embeddings.
- **`diagrams/include-cycles_src.md`** — Notes on the Cycles source symlink.

Most documentation assumes the code has already been built with CUDA support and that `sep_engine` runs. See below for a refresher on building and running.

## Build Recap

```bash
mkdir cmake-make
cd cmake-make
cmake ..
make -j$(nproc)
```

The resulting executable lives in `cmake-make/sep_engine`. Additional static libraries for each module are produced in the same directory.

## Running the Engine

Execute the engine from the build directory (`cmake-make`):

```bash
./sep_engine
```

Pass `--disable-audio` to skip initializing audio capture. This is useful on
systems without working audio devices or when `SEP_ENABLE_AUDIO` was set to
`OFF` during configuration:

```bash
./sep_engine --disable-audio
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

## Building Cycles

SEP can optionally render through Blender's Cycles engine. Building it requires
a number of external packages such as OpenVDB, OpenImageIO, OpenEXR/Imath,
OpenSubdiv and OpenImageDenoise. The helper script
`scripts/setup_cycles_env.sh` exports all required environment variables, builds
OpenVDB and then configures Cycles with CMake. Run this script after installing
the dependencies and whenever you start a new shell that doesn't have those
variables set.

The script defines variables including `OPENVDB_INCLUDE_DIR`,
`OPENVDB_LIBRARY`, `OPENIMAGEIO_INCLUDE_DIR`, `OPENIMAGEIO_LIBRARY`,
`OPENEXR_INCLUDE_DIR` and others. When no OpenShadingLanguage headers are found
on the system it automatically clones and builds version `v1.13.12.0` under
`/sep/extern/osl`. After executing it you can build Cycles from `/sep/cycles-build`:


## Diagram Sync Worker

The `diagram_sync_worker.py` script verifies that each module directory under `include/` and `src/` has a matching document in `docs/diagrams`.
Run it from the repository root:

```bash
python _sep/testbed/diagram_sync_worker.py
```

Add `--regen` to automatically create placeholder files for missing diagrams.
