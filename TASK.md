Critical: C++ Standard Mismatch Leading to Template/Concept Errors
Problem: The vast majority of errors (e.g., cannot take address of consteval function, no matching function for call to '__begin', constraints not satisfied for alias template 'sentinel_t', _Utf_view issues) indicate a C++ standard incompatibility, likely between your Clang 15.0.7 (which may default to C++20) and libraries like OpenImageIO (or its dependencies like fmtlib) that might be built/expected with C++17.
Location: Affects multiple CXX object files (ies.cpp.o, md5.cpp.o, osd.cpp.o, profiling.cpp.o, string.cpp.o, system.cpp.o, task.cpp.o, time.cpp.o, dice.cpp.o, interpolation.cpp.o, split.cpp.o, log.cpp.o, engine.cpp). The root cause is likely in your main CMakeLists.txt.
Action: Explicitly set the C++ standard for your project to C++17. Even though CMAKE_CXX_STANDARD_REQUIRED ON is set, CMAKE_CXX_STANDARD might not be explicitly set to 17, causing Clang to implicitly use a newer standard like C++20 which introduces features incompatible with how some libraries were built.
File to Edit: /sep/CMakeLists.txt (or your top-level CMake file).
Change: Add the line set(CMAKE_CXX_STANDARD 17) directly below set(CMAKE_CXX_STANDARD_REQUIRED ON).
Critical: Missing SEPResult Enum Member
Problem: The error error: no member named 'INTERNAL_ERROR' in 'sep::SEPResult' is reported.
Location: src/core/engine.cpp:326:30
Action: The SEPResult enum (defined in include/core/common.h) does not contain a member named INTERNAL_ERROR. Replace it with an existing appropriate error code. Based on the context (catching a generic std::exception), PROCESSING_ERROR or UNKNOWN_ERROR are suitable.
File to Edit: src/core/engine.cpp
Change: On line 326, change sep::SEPResult::INTERNAL_ERROR to sep::SEPResult::PROCESSING_ERROR.
Warning (Clean-up): Macro Redefinitions (WITH_NANOVDB, WITH_OPENIMAGEDENOISE)
Problem: You have warnings like 'WITH_NANOVDB' macro redefined and 'WITH_OPENIMAGEDENOISE' macro redefined. The build log indicates these are initially defined to 1 (likely by CMake command-line definitions) but then redefined to 0 (likely by included headers when the features are not found).
Location: Multiple CXX object compilations, originating from command-line definitions.
Action: Review the CMake logic that defines these macros. Instead of unconditionally defining them to 1, they should only be defined to 1 if the corresponding libraries are explicitly found AND enabled. Otherwise, they should be defined to 0 once if they need to be defined at all. This typically means using add_compile_definitions(WITH_NANOVDB=1) or add_compile_definitions(WITH_NANOVDB=0) conditionally.
File to Edit: Likely /sep/CMakeLists.txt or a related CMake module that handles third-party library detection and feature flags.
Change: Adjust the add_compile_definitions or target_compile_definitions calls related to WITH_NANOVDB and WITH_OPENIMAGEDENOISE to ensure they are defined correctly (and only once) based on whether the features were actually found.
Warning (Clean-up): Unused CMake Variables
Problem: CMake warns that PIPEWIRE_INCLUDE_DIR and PIPEWIRE_LIBRARY variables were manually specified but not used by the project.
Location: CMake configuration output.
Action: Since PipeWire seems to be correctly found via pkg-config elsewhere in your CMake setup, these explicitly set variables might be redundant. If they are not used elsewhere in your CMake logic, they can be removed.
File to Edit: Likely /sep/CMakeLists.txt or a PipeWire-related CMake module.
Change: Remove or comment out the lines where PIPEWIRE_INCLUDE_DIR and PIPEWIRE_LIBRARY are manually set, if pkg-config is indeed handling the discovery correctly.