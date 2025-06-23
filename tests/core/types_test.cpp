#include "core/types.h"
#include "compat/types.h"
#include "sep/result_defs.h"
#include <gtest/gtest.h>

// Test PinState equality operator
TEST(TypesTest, PinStateEquality) {
  sep::PinState a{42ULL, 7u};
  sep::PinState b{42ULL, 7u};
  sep::PinState c{43ULL, 7u};
  sep::PinState d{42ULL, 8u};

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
  EXPECT_FALSE(a == d);
}

using namespace sep;

// Test KernelDim default and parameterized constructors
TEST(TypesTest, KernelDimConstructors) {
  cuda::KernelDim defaultDim;
  EXPECT_EQ(defaultDim.x, 1u);
  EXPECT_EQ(defaultDim.y, 1u);
  EXPECT_EQ(defaultDim.z, 1u);

  cuda::KernelDim customDim(3, 4, 5);
  EXPECT_EQ(customDim.x, 3u);
  EXPECT_EQ(customDim.y, 4u);
  EXPECT_EQ(customDim.z, 5u);
}

// Test ProcessOptions default values
TEST(TypesTest, ProcessOptionsDefaults) {
  ProcessOptions opts;
  EXPECT_TRUE(opts.use_gpu);
  EXPECT_EQ(opts.batch_size, 64u);
  EXPECT_TRUE(opts.enable_validation);
}

// Test Metrics default initialization
TEST(TypesTest, MetricsDefaults) {
  Metrics m;
  EXPECT_DOUBLE_EQ(m.processing_time, 0.0);
  EXPECT_DOUBLE_EQ(m.gpu_utilization, 0.0);
  EXPECT_EQ(m.memory_used, 0u);
  EXPECT_EQ(m.error_count, 0u);
}

// Test Error default initialization and fields
TEST(TypesTest, ErrorDefaults) {
  Error e;
  EXPECT_EQ(e.status, Status::Success);
  EXPECT_TRUE(e.message.empty());
  EXPECT_TRUE(e.location.empty());
  EXPECT_EQ(e.code, 0u);
}

// Test MemoryAllocation default
TEST(TypesTest, MemoryAllocationDefaults) {
  MemoryAllocation ma;
  EXPECT_EQ(ma.ptr, nullptr);
  EXPECT_EQ(ma.size, 0u);
  EXPECT_EQ(ma.type, ResourceType::Host);
  EXPECT_FALSE(ma.success);
  // Error nested inside
  EXPECT_EQ(ma.error.status, Status::Success);
  EXPECT_TRUE(ma.error.message.empty());
}

// Test PoolStats default
TEST(TypesTest, PoolStatsDefaults) {
  PoolStats ps;
  EXPECT_EQ(ps.total_size, 0u);
  EXPECT_EQ(ps.used_size, 0u);
  EXPECT_EQ(ps.block_count, 0u);
  EXPECT_EQ(ps.free_blocks, 0u);
}

// Test PoolConfig default
TEST(TypesTest, PoolConfigDefaults) {
  PoolConfig pc;
  EXPECT_GT(pc.initial_size, 0u);
  EXPECT_GT(pc.max_size, 0u);
  EXPECT_GT(pc.block_size, 0u);
  EXPECT_TRUE(pc.enable_growth);
}

// Test MemoryResult default
TEST(TypesTest, MemoryResultDefaults) {
  MemoryResult mr;
  EXPECT_FALSE(mr.success);
  EXPECT_TRUE(mr.error.empty());
  EXPECT_EQ(mr.bytes_processed, 0u);
}

// Test kernel_config constants
TEST(TypesTest, KernelConfigConstants) {
  EXPECT_EQ(sep::cuda::kernel_config::QBSA_BLOCK_SIZE, 256);
  EXPECT_EQ(sep::cuda::kernel_config::QSH_BLOCK_SIZE, 128);
  EXPECT_EQ(sep::cuda::kernel_config::QBSA_SHARED_MEM_SIZE, 0);
  EXPECT_EQ(sep::cuda::kernel_config::QSH_SHARED_MEM_SIZE, 0);
}

// Test enum underlying values and bitmask behavior
TEST(TypesTest, EnumUnderlyingValues) {
  EXPECT_EQ(static_cast<int>(ResourceType::Host), 0);
  EXPECT_EQ(static_cast<int>(ResourceType::Device), 1);

  EXPECT_EQ(static_cast<int>(Status::Success), 0);
  EXPECT_EQ(static_cast<int>(Status::Error), 1);

  EXPECT_EQ(static_cast<int>(StreamFlags::Default), 0);
  EXPECT_EQ(static_cast<int>(StreamFlags::NonBlocking), 1);
  EXPECT_EQ(static_cast<int>(StreamFlags::Synchronizing), 2);

  // EventFlags bitmask
  unsigned int combined = static_cast<unsigned int>(EventFlags::BlockingSync) |
                          static_cast<unsigned int>(EventFlags::DisableTiming);
  EXPECT_EQ(combined, 0x01u | 0x02u);
}

// Test Result<T> behavior
TEST(TypesTest, ResultTemplate) {
  Result<int> r;
  EXPECT_FALSE(r.success);
  EXPECT_TRUE(r.error.empty());
  // value is default constructed
  EXPECT_EQ(r.value, 0);

  r.success = true;
  r.error = "err";
  r.value = 123;
  EXPECT_TRUE(r.success);
  EXPECT_EQ(r.error, "err");
  EXPECT_EQ(r.value, 123);
}
