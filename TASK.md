The build output from sep_080450.txt shows the exact same fatal error: too many errors emitted, stopping now [-ferror-limit=] and a cascade of template-related errors within the extern/cycles/src/util/ and extern/cycles/src/subd/ directories, specifically related to C++ <format> and <ranges> headers. This reconfirms the core issue identified previously: C++ standard library and compiler incompatibility for C++20 features within the Cycles external library.
The "warnings" about WITH_NANOVDB and WITH_OPENIMAGEDENOISE being redefined indicate that the CMake configuration is still not perfectly clean, but these are not the cause of the fatal errors. The errors are deep within the C++ standard library's implementation details.
Since setting CMAKE_CXX_STANDARD 17 was suggested but not explicitly confirmed as attempted, here's a refined breakdown and next steps, focusing on directly addressing the C++ standard library issue.
Targeted Advice to Address Issues:
Confirm CMAKE_CXX_STANDARD in CMakeLists.txt:
Problem: The compiler is hitting internal <format> and <ranges> errors, suggesting either an incomplete C++20 standard library implementation for your specific compiler version (Clang 15.0.7 with GCC 15's libstdc++) or conflicting flags that disable necessary C++20 features.
Location: The errors are in <c++/15/format> and <c++/15/bits/iterator_concepts.h>.
Verification: Double-check that your main CMakeLists.txt (or the one directly responsible for building the extern/cycles part if it's separate) explicitly sets a C++ standard.
Actionable Step: Open your primary CMakeLists.txt and ensure it contains:
Generated cmake
# Explicitly set C++17 standard to avoid C++20 feature issues if not fully supported
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Ensures strict C++17 compliance
Use code with caution.
Cmake
Reasoning: If Cycles internally relies on C++20, but your libstdc++ or Clang 15.0.7 has an incomplete/buggy implementation for specific C++20 features (consteval, ranges, format), forcing C++17 might circumvent those problematic features by making the compiler use C++17 equivalents or disallow the C++20 syntax altogether, if Cycles is designed to be backwards compatible. This is the most common fix for such "too many errors emitted" coming from standard library headers.
Verify extern/cycles's CMakeLists.txt (if it exists and is separate):
Problem: Sometimes, external libraries have their own CMakeLists.txt files that override the parent project's C++ standard settings.
Location: /sep/extern/cycles/CMakeLists.txt (or similar).
Verification: Check if extern/cycles/CMakeLists.txt (if present) is setting CMAKE_CXX_STANDARD itself.
Actionable Step: If it exists and sets its own standard, ensure it's also 17 or remove the line so it inherits from the top-level. Or, if it relies on C++20 features that are truly incompatible, consider a different version of Cycles or a different compiler/libstdc++ combination. Given the output, it seems Cycles's current source expects something that your current GCC 15 libstdc++ (which is quite new) doesn't perfectly provide for C++20.
Address Macro Redefinition Warnings (Cleanup for good measure):
Problem: While not critical errors, WITH_NANOVDB and WITH_OPENIMAGEDENOISE are still being redefined. This indicates a messy build environment or CMake configuration.
Location: The build output explicitly states command line is redefining them. This usually means your top-level CMakeLists.txt or a script is passing them via add_compile_definitions() or similar before Cycles's own CMake logic correctly determines its dependencies.
Actionable Step:
Search your entire CMakeLists.txt tree for WITH_NANOVDB and WITH_OPENIMAGEDENOISE.
Identify where they are initially set to 1 (which might be an add_compile_definitions in the root CMakeLists.txt that should ideally not be there, or it should be conditionally set after dependency checks).
Ensure that they are only defined once, preferably by the Cycles project itself after it checks for the presence of NanoVDB and OpenImageDenoise libraries using find_package(). If those libraries are not found, Cycles's own CMake should define WITH_NANOVDB and WITH_OPENIMAGEDENOISE to 0. Your build log explicitly says "NanoVDB not found, continuing without NanoVDB support" and "OpenImageDenoise not found, continuing without OpenImageDenoise support", so the final definition should consistently be 0.