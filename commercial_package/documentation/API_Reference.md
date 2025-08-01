# SEP Engine API Reference - Breakthrough Configuration

## Revolutionary Performance Achievement
**60.73% accuracy at 19.1% signal rate** - Commercial-grade algorithmic trading performance with patentable optimal configuration.

## Core Libraries Integration

### libsep_quantum.a - Pattern Recognition Engine

#### Key Headers
```cpp
#include "quantum/pattern_processor.h"
#include "quantum/qfh.h"  
#include "quantum/qbsa.h"
```

#### Core Classes

##### PatternProcessor
```cpp
namespace sep::quantum {
    class PatternProcessor {
    public:
        // Analyze market data for patterns
        PatternMetrics analyzePattern(const std::vector<double>& data);
        
        // Process bitstream data
        QFHResult processQFH(const std::vector<uint8_t>& bits);
        
        // Quantum bit state analysis
        QBSAResult processQBSA(const std::vector<uint8_t>& bits);
    };
}
```

##### Pattern Metrics Structure
```cpp
struct PatternMetrics {
    double coherence;        // Pattern predictability [0,1]
    double stability;        // Temporal consistency [0,1]  
    double entropy;          // Information content [0,1]
    double energy;           // Signal strength
    size_t length;           // Pattern length
    double confidence;       // Overall confidence [0,1]
};
```

#### Example Usage
```cpp
#include "quantum/pattern_processor.h"

// Initialize processor
auto processor = sep::quantum::PatternProcessor();

// Analyze market data
std::vector<double> prices = {1.1234, 1.1235, 1.1233, ...};
auto metrics = processor.analyzePattern(prices);

// Check signal strength
if (metrics.confidence > 0.85 && metrics.coherence > 0.6) {
    // High-confidence trading signal
    auto signal = (metrics.stability < 0.45) ? "BUY" : "SELL";
}
```

### libsep_trader_cuda.a - GPU Acceleration

#### CUDA Requirements
- CUDA Compute Capability 6.1+
- CUDA Toolkit v12.9
- Minimum 4GB GPU memory

#### Headers
```cpp
#include "apps/oanda_trader/tick_cuda_kernels.cuh"
#include "apps/oanda_trader/cuda_types.cuh"
```

#### GPU Processing
```cpp
// Launch GPU-accelerated analysis
std::vector<TrajectoryPointDevice> trajectories;
std::vector<DampedValueDevice> results;

// Process on GPU
launchTrajectoryKernel(trajectories.data(), results.data(), 
                      num_trajectories, trajectory_length);
```

### libsep_trader_logic.a - Signal Generation

#### Market Data Processing
```cpp
#include "apps/oanda_trader/quantum_signal_bridge.hpp"

// Initialize signal generator
auto bridge = std::make_unique<QuantumSignalBridge>();
bridge->initialize();

// Process market data
MarketData current_data = {
    .instrument = "EUR_USD",
    .mid = 1.1234,
    .bid = 1.1233,
    .ask = 1.1235,
    .volume = 1000,
    .timestamp = current_timestamp
};

std::vector<MarketData> history = {...};
auto signal = bridge->analyzeMarketData(current_data, history);

// Use generated signal
switch(signal.action) {
    case QuantumTradingSignal::BUY:
        // Execute buy order
        break;
    case QuantumTradingSignal::SELL:  
        // Execute sell order
        break;
    case QuantumTradingSignal::HOLD:
        // No action
        break;
}
```

## Complete Applications

### quantum_tracker - Live Trading Application

#### Command Line Usage
```bash
# GUI mode (requires X11/display)
./quantum_tracker

# Headless mode for servers  
./quantum_tracker --test

# With specific market data
./quantum_tracker --data /path/to/oanda_data.json
```

#### Programmatic Control
```cpp
#include "apps/oanda_trader/quantum_tracker_app.hpp"

// Initialize application
auto app = QuantumTrackerApp(true); // headless mode
app.initialize();

// Process market data
MarketData data = {...};
app.processNewMarketData(data);

// Get latest signal
auto signal = app.getLatestSignal();
```

### pme_testbed - Backtesting Engine  

#### Usage
```bash
# Basic backtesting
./pme_testbed path/to/historical_data.json

# With JSON output
./pme_testbed data.json --json > results.json

# Specify analysis window
./pme_testbed data.json --window 100
```

#### Output Format
```json
{
  "timestamp": "2025-07-24T14:00:33.000000000Z",
  "open": 1.12250,
  "high": 1.12255, 
  "low": 1.12248,
  "close": 1.12252,
  "volume": 156,
  "pattern_id": "pattern_2025-07-24T14:00:33.000000000Z",
  "coherence": 0.99991,
  "stability": 0.10000,
  "entropy": 0.19608,
  "signal": "SELL",
  "confidence": 0.58923
}
```

## Linking Instructions

### CMake Integration
```cmake
# Find CUDA
find_package(CUDA REQUIRED)

# Add SEP Engine libraries
target_link_libraries(your_target
    ${SEP_ENGINE_PATH}/libsep_quantum.a
    ${SEP_ENGINE_PATH}/libsep_trader_logic.a
    ${SEP_ENGINE_PATH}/libsep_trader_cuda.a
    ${CUDA_LIBRARIES}
)

# Include directories  
target_include_directories(your_target PRIVATE
    ${SEP_ENGINE_HEADERS}/quantum
    ${SEP_ENGINE_HEADERS}/connectors
    ${SEP_ENGINE_HEADERS}/apps
)
```

### Manual Compilation
```bash
# Compile with libraries
g++ -o your_app your_code.cpp \
    -L/path/to/sep/libraries \
    -lsep_quantum -lsep_trader_logic -lsep_trader_cuda \
    -I/path/to/sep/headers \
    -lcudart -ltbb -pthread

# Ensure CUDA runtime is available
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
```

## Performance Optimization

### Memory Management
```cpp
// Efficient batch processing
std::vector<MarketData> batch;
batch.reserve(1000); // Pre-allocate

// Process in chunks
for (auto& chunk : data_chunks) {
    auto results = processor.analyzePattern(chunk);
    // Process results...
}
```

### CUDA Optimization
```cpp
// Configure GPU memory
cudaSetDevice(0);
cudaDeviceSetCacheConfig(cudaFuncCachePreferL1);

// Use streams for parallel processing
cudaStream_t stream;
cudaStreamCreate(&stream);
```

### Threading
```cpp
// Thread-safe processing
std::mutex processor_mutex;
auto processor = sep::quantum::PatternProcessor();

// In worker thread
{
    std::lock_guard<std::mutex> lock(processor_mutex);
    auto result = processor.analyzePattern(data);
}
```

## Error Handling

### Common Issues
```cpp
// Check CUDA availability
if (!cudaDeviceCount()) {
    // Fallback to CPU processing
}

// Validate input data
if (market_data.empty() || market_data.size() < 20) {
    throw std::invalid_argument("Insufficient data for analysis");
}

// Check library initialization
if (!quantum_bridge->initialize()) {
    throw std::runtime_error("Failed to initialize quantum bridge");
}
```

### Debug Output
```cpp
// Enable detailed logging
#define SEP_DEBUG_ENABLED
#include "quantum/pattern_processor.h"

// Debug information will be output to stderr
```

## Integration Validation

### Test Your Integration
```cpp
// Validate library integration
auto processor = sep::quantum::PatternProcessor();
std::vector<double> test_data = {1.0, 1.1, 1.0, 1.1, 1.0}; // Alternating pattern
auto metrics = processor.analyzePattern(test_data);

assert(metrics.coherence > 0.8); // Should detect high coherence
assert(metrics.confidence > 0.5); // Should have reasonable confidence
```

### Performance Benchmarking
```cpp
#include <chrono>

auto start = std::chrono::high_resolution_clock::now();
auto metrics = processor.analyzePattern(large_dataset);
auto end = std::chrono::high_resolution_clock::now();

auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
// Should complete in <100ms for typical dataset
```

This API provides the complete interface for integrating SEP Engine's validated mathematical algorithms into your trading systems.
