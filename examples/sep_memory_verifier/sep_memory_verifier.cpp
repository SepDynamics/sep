#include <iostream>
#include "memory/redis_manager.h"
#include <benchmark/benchmark.h>

static void BM_RedisConnection(benchmark::State& state) {
  for (auto _ : state) {
    auto redis_manager = sep::persistence::createRedisManager("localhost", 6379);
    benchmark::DoNotOptimize(redis_manager);
  }
}
BENCHMARK(BM_RedisConnection);

int main(int argc, char** argv) {
    auto redis_manager = sep::persistence::createRedisManager("localhost", 6379);
    if (!redis_manager->isConnected()) {
        std::cerr << "Failed to connect to Redis" << std::endl;
        return 1;
    }

    std::cout << "Successfully connected to Redis" << std::endl;

    std::cout << "SEP Memory Verifier" << std::endl;
    
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    return 0;
}