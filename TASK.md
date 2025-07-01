~: Open `CMakeLists.txt` at project root.
~: Remove `add_compile_definitions(WITH_NANOVDB=${WITH_NANOVDB})` (line 21).
~: Ensure `WITH_NANOVDB` and `WITH_OPENIMAGEDENOISE` are defined only once after dependency checks (lines 63–72).
~: Run `cmake` configuration to verify that macro redefinition warnings disappear.
~: Confirm root `CMakeLists.txt` already sets `CMAKE_CXX_STANDARD 17`.
~: Add an include (`cmake/SetCXXStandard.cmake`) that sets the standard and is included by all subdirectories.
~: Update every `CMakeLists.txt` (e.g., under `src/` and `tests/`) to call this helper instead of setting the standard individually.
~: Configure and build to ensure all targets inherit the C++17 setting.
~: Replace the broken symlink `extern/cycles` with the actual Cycles source or a git submodule.
~: Update build instructions to pull the submodule (e.g., `git submodule update --init extern/cycles`).
~: Ensure `scripts/setup_cycles_env*.sh` references the correct path.
~: Verify that CMake detects the Cycles source during configuration.
~: Search the entire project for `WITH_OPENIMAGEDENOISE` definitions.
~: Keep the conditional block in `CMakeLists.txt` that sets this macro based on `find_package(OpenImageDenoise)`.
~: Remove any other compile-time definitions of `WITH_OPENIMAGEDENOISE`.
~: Reconfigure the build and confirm the macro redefinition warning in `build.log` is resolved.
~: Modify `scripts/setup_cycles_env.sh` and `setup_cycles_env_fixed.sh` to validate that `${CMAKE_CXX_STANDARD}` equals 17 before invoking CMake.
~: If not, print an error and abort the script.
~: Document this requirement in `docs/CONFIG_OPTIONS.md`.
~: Test by running the script with an incorrect standard and ensuring it fails with a clear message.
~: Update `docs/GAMEPLAN.md` with a short section on initializing external dependencies.
~: Include commands for initializing the Cycles submodule and verifying OpenVDB/OpenImageDenoise detection.
~: Cross-reference `install_dependencies.py` for Python requirements.
~: Provide a troubleshooting tip for missing libraries.