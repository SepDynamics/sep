# AGENT.md - SEP Engine Project Guide

## Project Overview
**SEP Engine** is a predictive financial modeling system using CUDA-accelerated pattern analysis on forex data. The goal is to develop alpha prediction capabilities through optimized time-series analysis.

## Build System

### Primary Build Command
```bash
./build_and_test.sh
```
- **Purpose**: Complete Docker-based build and test cycle
- **Environment**: Containerized build eliminates system dependencies
- **Output**: Logs to `output/build_log.txt` with intelligent error parsing
- **CUDA**: Handles CUDA Toolkit v12.9 with system library compatibility issues

### Build Status
- ✅ **Build System Stabilized**: Docker-based hermetic builds resolve CUDA/glibc conflicts
- ✅ **CUDA Integration**: Using targeted `noexcept(x)` workaround for compiler compatibility
- ⚠️ **Known Issue**: CUDA Toolkit v12.9 has `noexcept` conflicts with modern glibc

## Key Project Components

### Core Executables
- `./build/examples/pattern_metric_example` - Main metrics analysis engine
  - Supports `--json` flag for structured output
  - Use `--no-clear` for stateful processing
  - Example: `./pattern_metric_example data/ --json --no-clear`

### Python Pipeline
- `run_alpha_experiment.py` - Complete alpha prediction experiment
- `financial_backtest.py` - Strategy backtesting with performance metrics
- `prepare_experiment_data.py` - Data preparation and chunking

### Data Structure
- `Testing/OANDA/` - Historical forex data for experiments
- Training data: `O-train-1.json`
- Test data: `O-test-2.json`

## Current Priorities (Phase 2)

### Completed ✅
1. **JSON Output**: `pattern_metric_example` already supports `--json` flag
2. **Build Stabilization**: Docker environment resolves CUDA compilation issues

### In Progress 🔄
1. **Pipeline Integration**: Updated `run_alpha_experiment.py` to use JSON output
2. **Test Pipeline**: Ready to run complete OANDA experiment

### Next Steps 📋
1. **Run Experiment**: Execute `python3 run_alpha_experiment.py`
2. **Profile Performance**: Use `nvprof` or `Nsight Systems` for CUDA optimization
3. **Enhance Visualization**: Add charts to backtesting results
4. **Document Results**: Create `docs/proofs/poc_6_predictive_backtest.md`

## CUDA Development

### Environment
- **CUDA Version**: 12.9
- **Compiler**: clang++-15 (host compiler)
- **Known Fix**: CMakeLists.txt includes `-Xcompiler -Dnoexcept\\(x\\)=` workaround

### Profiling Commands
```bash
# Build first
./build_and_test.sh

# Profile with Nsight Systems
nsys profile --output=pattern_metric_profile ./build/examples/pattern_metric_example Testing/OANDA/

# Profile with nvprof (legacy)
nvprof ./build/examples/pattern_metric_example Testing/OANDA/
```

## Development Workflow

### MCP Tools Integration
- **Filesystem**: Full access to `/sep` directory structure
- **Memory**: Knowledge graph for complex relationships
- **Sequential Thinking**: For complex problem analysis

### Code Standards
- **C++**: Follow existing patterns in `src/` and `examples/`
- **Python**: PEP 8, use existing pandas/numpy patterns
- **CUDA**: Separate `.cu` files, proper abstraction from C++ headers
- **JSON**: Use structured output for all metrics and results

### Testing Approach
1. Build with `./build_and_test.sh`
2. Check `output/build_log.txt` for issues
3. Run experiments with Python scripts
4. Profile CUDA performance for optimization

## Project Structure
```
/sep/
├── src/                    # Core C++ engine source
├── examples/              # Executable examples (pattern_metric_example)
├── Testing/OANDA/         # Historical forex data
├── docs/                  # Documentation and proofs
├── build/                 # Build artifacts (generated)
├── output/                # Logs and results
├── *.py                   # Python pipeline scripts
└── build_and_test.sh      # Primary build command
```

## Communication Protocol

### Build Issues
1. Run `./build_and_test.sh`
2. Check `output/build_log.txt` for detailed errors
3. Address CUDA/compilation issues in Docker environment

### Experiment Issues
1. Ensure build succeeds first
2. Run Python scripts with error capture
3. Use JSON output for all metrics processing

### Performance Issues
1. Profile with CUDA tools after successful build
2. Focus on GPU utilization and memory patterns
3. Document bottlenecks in `profiling_results.txt`
