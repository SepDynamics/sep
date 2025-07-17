# TODO.md: Project Roadmap for Data Processor Integration

This document outlines a sequential gameplan to address current build issues and progress toward implementing a data processor. The focus is on resolving compilation errors first, then building and testing the data processor independently via socket and file/directory inputs. CUDA remains integrated in the demo manager as-is; no removal or questioning of its necessity. Proceed one step at a time, verifying success before moving to the next.

## Step 1: Resolve CUDA Redefinition and Namespace Conflicts
- Analyze the build log and defect report for all instances of redefinitions (e.g., `cudaStreamDestroy`, `cudaMemcpy`, `cudaMemcpyKind`).
- Refactor header includes in affected files (e.g., `cuda_types.h`, `cuda_functions.h`, `cuda_runtime.h`, `cuda_unified.h`) to use include guards, forward declarations, or conditional compilation to prevent multiple definitions.
- Fix namespace issues by ensuring all CUDA-related symbols are consistently qualified under `sep::cuda`.
- Rebuild the project and confirm all CUDA-related compilation errors are eliminated.

## Step 2: Address Remaining Compilation Errors from Defect Report
- Prioritize CRITICAL and HIGH severity defects (e.g., undefined types like `cudaStream_t`, missing declarations).
- Fix MEDIUM and LOW defects in batches: start with reserved identifiers and undefined memory manipulation in ImGui and GLAD files.
- Update any unused parameters or dead code as flagged.
- Rebuild and run the analyzer again to verify no new errors; ensure the project compiles cleanly.

## Step 3: Stabilize the Build Environment
- Verify all dependencies (e.g., CUDA toolkit, GLM, ImGui, Crow) are correctly installed and paths are set in CMake.
- Test a clean build with `ninja` or equivalent, ensuring no failures in linking or object generation.
- Run basic runtime tests on existing demos to confirm stability post-fixes.

## Step 4: Design and Implement the Data Processor Module
- Create a new class/module (e.g., `DataProcessor`) in a dedicated source file (e.g., `src/core/data_processor.cpp` and `.h`).
- Define core functionality: input parsing, processing logic (e.g., data transformation, validation), and output generation.
- Ensure it is decoupled from CUDA/demo manager initially for independent testing.

## Step 5: Implement Socket-Based Testing for Data Processor
- Add socket reading capabilities to `DataProcessor` using a library like Crow or Boost.Asio for input reception.
- Create a test harness (e.g., a standalone main or unit test) that simulates socket data input and processes it through the module.
- Verify functionality by sending sample data via a client tool (e.g., netcat) and checking processed outputs.

## Step 6: Implement File/Directory Reading for Data Processor
- Extend `DataProcessor` to handle file and directory inputs using standard C++ filesystem APIs.
- Add methods for reading from files/directories, processing contents, and handling errors.
- Update the test harness to include file-based inputs; run tests with sample files/directories to validate.

## Step 7: Integrate Data Processor with Demo Manager
- Hook `DataProcessor` into the demo manager workflow, ensuring CUDA components remain unchanged.
- Add configuration options in demos to toggle between socket/file inputs for data processing.
- Test integration by running demos with processed data from both socket and file sources.

## Step 8: Final Validation and Cleanup
- Run full project tests, including analyzer, to catch any regressions.
- Document the data processor usage in code comments and this TODO.md.
- Commit changes and prepare for next iterations if needed.