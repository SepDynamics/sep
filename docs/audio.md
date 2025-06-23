# Audio Pipeline and Cleanup Tasks

This document summarises the real-time audio path and the repository cleanup performed for the module.

## Pipeline Overview

1. **Capture** – `PipeWireCapture` opens a `pw_thread_loop` and pulls samples from the default source.
2. **Frame Queue** – `AudioPipeline` groups samples into fixed-size frames.
3. **Window & FFT** – each frame receives a Hann window before a simplified FFT.
4. **Feature Extraction** – fundamental frequency, spectral centroid and spectral flux are computed.
5. **Pattern Output** – results are normalised into `glm::vec3` vectors and optionally queued for other modules.

Implementation files live under `src/audio/` and the public headers in `include/audio/` now contain only declarations.  Any stub logic originally inside headers was moved to `.cpp` sources.

## Cleanup Tasks

Generated build artefacts such as `cmake_install.cmake` were removed across the repository.  See `docs/build_cleanup.md` for the commands used to search for these files.  The previously referenced `src/audio/cmake_install.cmake` is no longer present.
