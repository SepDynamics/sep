# Audio Module Overview

This document describes the remaining files under `src/audio` and `include/audio` after removing generated artefacts. It also explains how the audio interfaces integrate with the rest of the engine.

## Clean Up Results

All `cmake_install.cmake` files were deleted from the repository. The `src/audio` directory now contains only the source files needed to build the optional audio library:

```
src/audio/
  CMakeLists.txt        - build rules for the `sep_audio` library
  README.md             - documentation for the module
  config.cpp            - global configuration and coherence helpers
  pipeline.cpp          - implementation of `AudioPipeline`
  pipewire_capture.cpp  - implementation of `PipeWireCapture`
```

The public headers remain under `include/audio`:

```
include/audio/
  capture.h             - `AudioCapture` interface and factory helper
  config.h              - `AudioPipelineConfig` and coherence utilities
  pipeline.h            - declarations for `AudioPipeline`
  pipewire_capture.h    - `PipeWireCapture` class
  pipewire_includes.h   - conditional PipeWire wrapper/stubs
  types.h               - common structs and enums
  README.md             - API overview
```

`pipewire_capture.cpp` and `pipeline.cpp` include their corresponding headers for all declarations. No extra inline implementations exist outside the headers except for the small stubs in `pipewire_includes.h` which allow compilation without the PipeWire library.

## Integration Points

`include/core/engine.h` forward declares `AudioCapture` and `Engine` owns a `std::unique_ptr` to an instance created via `AudioCapture::create()`. During `Engine::init()` the capture object is initialised and optionally started when the engine runs. Processed pattern vectors produced by `AudioPipeline` can then be consumed by other modules such as `src/pattern` or `src/memory`.

## File Status

No obsolete headers or stub sources were found. `capture.h` is still required as the abstract interface used by `PipeWireCapture` and referenced by the core engine. The only stub code resides in `pipewire_includes.h` to handle the absence of PipeWire headers.

