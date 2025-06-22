#include <benchmark/benchmark.h>

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "blender/compat/blender_types.h"
#include "blender/mesh_handler.h"
#include "quantum/types.h"

using namespace sep::pattern;

// Test fixture setup
struct MeshBenchmark {
  void SetupMesh(int size) {
    // Create test mesh
    mesh = new Mesh();
    mesh->totvert = size * size;
    mesh->totedge = (size - 1) * size * 2;
    mesh->totpoly = (size - 1) * (size - 1);

    mesh->mvert = new MVert[mesh->totvert]();
    mesh->medge = new MEdge[mesh->totedge]();
    mesh->mloop = new MLoop[mesh->totpoly * 4]();
    mesh->mpoly = new MPoly[mesh->totpoly]();

    // Initialize grid vertices
    float seed = 1.0f;
    auto deterministic_noise = [&seed]() {
      seed = std::fmod(seed * 1.3f, 1.0f);
      return (seed - 0.5f) * 0.2f;
    };

    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        int idx = i * size + j;
        mesh->mvert[idx].co[0] = static_cast<float>(i) + deterministic_noise();
        mesh->mvert[idx].co[1] = static_cast<float>(j) + deterministic_noise();
        mesh->mvert[idx].co[2] = deterministic_noise();
      }
    }

    SetupGridTopology(size);

    // Create test object
    object = new Object();
    object->type = OB_MESH;
    object->data = mesh;

    // Initialize handler
    handler = new MeshHandler();
    handler->init(object, mesh);
  }

  void SetupGridTopology(int size) {
    int edge_idx = 0;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size - 1; ++j) {
        mesh->medge[edge_idx].v1 = i * size + j;
        mesh->medge[edge_idx].v2 = i * size + j + 1;
        edge_idx++;
      }
    }
    for (int i = 0; i < size - 1; ++i) {
      for (int j = 0; j < size; ++j) {
        mesh->medge[edge_idx].v1 = i * size + j;
        mesh->medge[edge_idx].v2 = (i + 1) * size + j;
        edge_idx++;
      }
    }

    int face_idx = 0;
    int loop_idx = 0;
    for (int i = 0; i < size - 1; ++i) {
      for (int j = 0; j < size - 1; ++j) {
        mesh->mpoly[face_idx].loopstart = loop_idx;
        mesh->mpoly[face_idx].totloop = 4;

        mesh->mloop[loop_idx].v = i * size + j;
        mesh->mloop[loop_idx + 1].v = i * size + j + 1;
        mesh->mloop[loop_idx + 2].v = (i + 1) * size + j + 1;
        mesh->mloop[loop_idx + 3].v = (i + 1) * size + j;

        face_idx++;
        loop_idx += 4;
      }
    }
  }

  void SetupPatterns(int count) {
    float seed = 0.123f;
    auto next = [&seed]() {
      seed = std::fmod(seed * 1.7f + 0.13f, 1.0f);
      return seed;
    };

    patterns.resize(count);
    for (auto& pattern : patterns) {
      pattern.coherence = next();
      pattern.entropy = next();
      pattern.stability = next();
      pattern.mutation_rate = next();
      pattern.generation = 1;
      pattern.position = {next() * 20.0f - 10.0f, next() * 20.0f - 10.0f, next() * 20.0f - 10.0f,
                          1.0f};
      pattern.velocity = {next(), next(), next(), 0.0f};
      // block_size field removed - not part of PatternData structure
    }
  }

  void Cleanup() {
    delete[] mesh->mvert;
    delete[] mesh->medge;
    delete[] mesh->mloop;
    delete[] mesh->mpoly;
    delete mesh;
    delete object;
    delete handler;
    patterns.clear();
  }

  Object* object = nullptr;
  Mesh* mesh = nullptr;
  MeshHandler* handler = nullptr;
  std::vector<sep::pattern::PatternData> patterns;
};

static void BM_PatternUpdate(::benchmark::State& state) {
  MeshBenchmark bench;
  bench.SetupMesh(state.range(0));
  bench.SetupPatterns(state.range(1));

  for (auto _ : state) {
    bench.handler->update(bench.patterns[state.iterations() % bench.patterns.size()]);
  }

  bench.Cleanup();
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * sizeof(sep::pattern::PatternData));
}
BENCHMARK(BM_PatternUpdate)
    ->Args({100, 10})   // 100x100 grid, 10 patterns
    ->Args({100, 100})  // 100x100 grid, 100 patterns
    ->Args({1000, 10})  // 1000x1000 grid, 10 patterns
    ->Unit(::benchmark::kMillisecond);

static void BM_Deformation(::benchmark::State& state) {
  MeshBenchmark bench;
  bench.SetupMesh(state.range(0));

  MeshHandler::DeformParams params;
  params.strength = 0.5f;
  params.smoothness = 0.3f;
  params.preserve_volume = true;
  params.use_falloff = true;

  for (auto _ : state) {
    bench.handler->applyDeformation(params);
  }

  // Save vertex count before cleanup
  int vertex_count = bench.mesh->totvert;
  bench.Cleanup();
  state.SetItemsProcessed(state.iterations() * vertex_count);
  state.SetBytesProcessed(state.iterations() * vertex_count * sizeof(MVert));
}
BENCHMARK(BM_Deformation)
    ->Args({100, 1})   // 100x100 grid
    ->Args({1000, 1})  // 1000x1000 grid
    ->Unit(::benchmark::kMillisecond);

static void BM_CustomDataLayer(::benchmark::State& state) {
  MeshBenchmark bench;
  bench.SetupMesh(state.range(0));

  for (auto _ : state) {
    state.PauseTiming();
    std::string name = "bench_layer_" + std::to_string(state.iterations());
    state.ResumeTiming();

    bench.handler->addCustomDataLayer(name.c_str(), CD_PROP_FLOAT3);
    bench.handler->removeCustomDataLayer(name.c_str());
  }

  bench.Cleanup();
  state.SetItemsProcessed(state.iterations() * 2);  // add + remove
}
BENCHMARK(BM_CustomDataLayer)
    ->Args({100, 1})   // 100x100 grid
    ->Args({1000, 1})  // 1000x1000 grid
    ->Unit(::benchmark::kMicrosecond);

static void BM_MetricsCalculation(::benchmark::State& state) {
  MeshBenchmark bench;
  bench.SetupMesh(state.range(0));

  for (auto _ : state) {
    ::benchmark::DoNotOptimize(bench.handler->getMetrics());
  }

  bench.Cleanup();
  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_MetricsCalculation)
    ->Args({100, 1})   // 100x100 grid
    ->Args({1000, 1})  // 1000x1000 grid
    ->Unit(::benchmark::kMicrosecond);

static void BM_PatternEvolution(::benchmark::State& state) {
  MeshBenchmark bench;
  bench.SetupMesh(state.range(0));
  bench.SetupPatterns(state.range(1));

  for (auto _ : state) {
    state.PauseTiming();
    auto pattern = bench.patterns[state.iterations() % bench.patterns.size()];
    pattern.generation++;
    pattern.coherence *= (1.0f - pattern.entropy);
    pattern.stability *= (1.0f - pattern.mutation_rate);
    state.ResumeTiming();

    bench.handler->update(pattern);
    MeshHandler::DeformParams params;
    params.strength = 0.1f;
    params.smoothness = 0.3f;
    params.preserve_volume = true;
    params.use_falloff = true;
    bench.handler->applyDeformation(params);
  }

  bench.Cleanup();
  state.SetItemsProcessed(state.iterations() * 2);  // update + deform
}
BENCHMARK(BM_PatternEvolution)
    ->Args({100, 100})   // 100x100 grid, 100 patterns
    ->Args({1000, 100})  // 1000x1000 grid, 100 patterns
    ->Unit(::benchmark::kMillisecond);

BENCHMARK_MAIN();