# SEP Engine Standalone Executable

The SEP (Self-Emergent Processor) standalone executable is a comprehensive tool that combines all SEP Engine functionality into a single, powerful command-line application with full CUDA/GPU support.

## Features

- **Pattern Processing**: Analyze patterns in any data with quantum-inspired algorithms
- **GPU Acceleration**: Full CUDA support for high-performance processing
- **Financial Analysis**: Complete pipeline for alpha prediction and backtesting
- **Benchmarking**: Performance testing for CPU vs GPU processing
- **Interactive Demos**: Explore SEP capabilities with built-in demonstrations
- **Memory Tier Verification**: Test the multi-tier memory management system
- **JSON Output**: Machine-readable metrics for integration with other tools

## Building

The standalone executable is built automatically when running:

```bash
./build.sh
```

This will:
1. Build a Docker image with all dependencies (including CUDA)
2. Compile the SEP engine with GPU support
3. Run all tests and benchmarks
4. Package the standalone executable as `build/sep`

## Usage

```bash
./sep [options] <command> [command-args]
```

### Global Options

- `--gpu` - Enable GPU acceleration
- `--json` - Output in JSON format
- `--output <file>` - Write output to file
- `--iterations <n>` - Number of processing iterations
- `--verbose` - Enable verbose output
- `--no-clear` - Don't clear state between files

### Commands

#### 1. Process Command
Process pattern data from files or directories.

```bash
# Process a single file
./sep process data.json --gpu --json --output metrics.json

# Process all files in a directory
./sep process /path/to/data/ --gpu --iterations 3

# Process with state retention
./sep process data/ --no-clear --verbose
```

#### 2. Benchmark Command
Run performance benchmarks comparing CPU and GPU processing.

```bash
# Run default benchmarks
./sep benchmark

# Run with custom iterations
./sep benchmark --iterations 100

# GPU benchmarks only
./sep benchmark --gpu
```

#### 3. Demo Command
Run interactive demonstrations of SEP capabilities.

```bash
# Run all demos
./sep demo

# Run with GPU acceleration
./sep demo --gpu

# Run specific demo type
./sep demo --demo-type quantum
```

Demos include:
- Binary pattern analysis
- Text pattern analysis
- Numeric sequence analysis (Fibonacci)
- Quantum coherence evolution

#### 4. Verify Command
Verify the memory tier system functionality.

```bash
./sep verify
```

This tests:
- Memory allocation across tiers (STM, MTM, LTM)
- Tier utilization tracking
- Memory deallocation
- Tier transition logic

#### 5. Financial Command
Run the complete financial analysis pipeline.

```bash
# Process financial data only
./sep financial OANDA_data.json --gpu

# Process and run backtest
./sep financial OANDA_data.json --backtest

# With custom output
./sep financial data.json --gpu --output results/
```

## Output Formats

### JSON Output
When using `--json`, metrics are output as an array of pattern objects:

```json
[
  {
    "pattern_id": "abc123",
    "coherence": 0.8523,
    "stability": 0.9102,
    "entropy": 0.3421
  },
  ...
]
```

### Text Output
Default human-readable format:

```
=== Metrics for data.json ===
Pattern Count: 1024
Avg Coherence: 0.7832
Avg Stability: 0.8921
Avg Entropy:   0.4123
```

## Performance Considerations

### GPU Acceleration
- Automatically detects CUDA-capable GPUs
- Processes patterns in parallel on GPU
- Typically 10-100x faster than CPU for large datasets

### Memory Usage
- Processes files in 64KB chunks
- Efficient memory tier management
- Automatic garbage collection

### Benchmarking
Built-in benchmarks test various data sizes:
- 1KB to 1MB data blocks
- Measures throughput in MB/s
- Compares CPU vs GPU performance

## Integration Examples

### Python Integration
```python
import subprocess
import json

# Process data and get metrics
result = subprocess.run(
    ['./sep', 'process', 'data.json', '--gpu', '--json'],
    capture_output=True,
    text=True
)

metrics = json.loads(result.stdout)
```

### Bash Pipeline
```bash
# Process multiple files and analyze results
for file in data/*.json; do
    ./sep process "$file" --gpu --json >> all_metrics.json
done

# Run financial backtest
./sep financial combined_data.json --backtest
```

### Docker Usage
```bash
# Run in Docker with GPU support
docker run --gpus all -v $(pwd):/data sep-engine \
    ./sep process /data/input.json --gpu --output /data/output.json
```

## Troubleshooting

### CUDA Errors
- Ensure NVIDIA drivers are installed
- Check CUDA compatibility: `nvidia-smi`
- Use `--verbose` for detailed GPU info

### Memory Issues
- For large files, use `--iterations 1`
- Process directories file-by-file
- Monitor with `./sep verify`

### Performance Issues
- Use `--gpu` for large datasets
- Run benchmarks to verify setup
- Check system resources

## Advanced Usage

### Stateful Processing
Process data multiple times to observe pattern evolution:

```bash
./sep process data.json --iterations 5 --no-clear --verbose
```

### Custom Benchmarks
Create benchmark data:

```bash
dd if=/dev/urandom of=bench.dat bs=1M count=10
./sep process bench.dat --gpu --json
```

### Pipeline Integration
Complete financial analysis pipeline:

```bash
#!/bin/bash
# Process data
./sep process market_data/ --gpu --json --output metrics.json

# Run backtest
python3 run_alpha_experiment.py --input metrics.json --output alpha.json

# Analyze results
./sep demo --verbose
```

## Version Information

To check version and capabilities:

```bash
./sep --version  # (if implemented)
```

Current capabilities:
- SEP Engine v1.0
- CUDA 12.9 support
- Multi-tier memory system
- Quantum pattern processing
- Financial alpha prediction

## Support

For issues or questions:
1. Check the build log: `output/build_log.txt`
2. Run verification: `./sep verify`
3. Use verbose mode: `--verbose`
4. Review documentation in `docs/`

---

Built with the SEP Engine - Unifying quantum information theory with practical pattern analysis.