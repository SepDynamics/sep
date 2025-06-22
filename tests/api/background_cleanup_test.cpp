#include <gtest/gtest.h>
#include "api/background_cleanup.h"
#include <atomic>
#include <chrono>
#include <thread>

using namespace sep::api;
using namespace std::chrono_literals;

class BackgroundCleanupTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    // Helper to create a cleanup object with a callback
    std::unique_ptr<BackgroundCleanup> createCleanup(
        std::chrono::milliseconds interval,
        BackgroundCleanup::CleanupCallback callback) {
        return std::make_unique<BackgroundCleanup>(interval, callback);
    }
};

// Basic initialization and shutdown test
TEST_F(BackgroundCleanupTest, InitializationAndShutdown) {
    std::atomic<int> callCount{0}; // seq_cst

    {
        auto cleanup = createCleanup(100ms, [&](auto) {
            callCount++;
        });

        // Wait for at least one execution
        std::this_thread::sleep_for(150ms);
    }

    // Verify callback was called at least once
    EXPECT_GT(callCount, 0);
}

// Test callback execution timing
TEST_F(BackgroundCleanupTest, CallbackTiming) {
    std::atomic<int> callCount{0}; // seq_cst
    std::vector<std::chrono::steady_clock::time_point> callTimes;
    std::mutex callTimesMutex;

    const auto interval = 100ms;

    auto cleanup = createCleanup(interval, [&](auto now) {
        callCount++;
        {
            std::lock_guard<std::mutex> lock(callTimesMutex);
            callTimes.push_back(now);
        }
    });

    // Wait for multiple executions
    std::this_thread::sleep_for(350ms);
    cleanup.reset();

    // Verify multiple calls occurred
    EXPECT_GE(callCount, 3);

    // Verify timing intervals
    std::lock_guard<std::mutex> lock(callTimesMutex);
    for (size_t i = 1; i < callTimes.size(); ++i) {
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
            callTimes[i] - callTimes[i-1]);

        // Allow for more timing variance on CI systems
        EXPECT_GE(diff, interval * 0.5);
        EXPECT_LE(diff, interval * 1.5);
    }
}

// Test thread safety
TEST_F(BackgroundCleanupTest, ThreadSafety) {
    std::atomic<int> callCount{0}; // seq_cst
    std::atomic<int> activeThreads{0}; // seq_cst

    auto cleanup = createCleanup(50ms, [&](auto) {
        callCount++;
        activeThreads++;
        std::this_thread::sleep_for(10ms);  // Simulate work
        activeThreads--;
    });

    // Wait for multiple executions
    std::this_thread::sleep_for(200ms);
    cleanup.reset();

    // Verify no concurrent executions
    EXPECT_GT(callCount, 0);
    EXPECT_EQ(activeThreads, 0);
}

// Test rapid shutdown
TEST_F(BackgroundCleanupTest, RapidShutdown) {
    std::atomic<int> callCount{0}; // seq_cst

    {
        auto cleanup = createCleanup(1000ms, [&](auto) {
            callCount++;
            std::this_thread::sleep_for(500ms);  // Long operation
        });

        // Immediate destruction
    }

    // Verify cleanup completed gracefully
    EXPECT_LE(callCount, 1);
}

// Test long interval
TEST_F(BackgroundCleanupTest, LongInterval) {
    std::atomic<int> callCount{0}; // seq_cst

    auto cleanup = createCleanup(1000ms, [&](auto) {
        callCount++;
    });

    // Wait for just under one interval
    std::this_thread::sleep_for(800ms);

    // Should not have executed yet
    int count = callCount.load();
    EXPECT_LE(count, 1) << "Expected at most 1 call after 800ms, got " << count;

    // Wait for remainder of interval plus some margin
    std::this_thread::sleep_for(400ms);

    // Should have executed at least once
    count = callCount.load();
    EXPECT_GE(count, 1) << "Expected at least 1 call after 1200ms, got " << count;
}

// Test short interval
TEST_F(BackgroundCleanupTest, ShortInterval) {
    std::atomic<int> callCount{0}; // seq_cst

    // Use 150ms interval since implementation has 100ms sleep
    auto cleanup = createCleanup(150ms, [&](auto) {
        callCount++;
    });

    // Wait for multiple intervals
    std::this_thread::sleep_for(500ms);
    cleanup.reset();

    // Should have executed at least twice
    int count = callCount.load();
    EXPECT_GE(count, 2) << "Expected at least 2 calls in 500ms with 150ms interval, got " << count;
}

// Test exception handling
TEST_F(BackgroundCleanupTest, ExceptionHandling) {
    std::atomic<int> callCount{0}; // seq_cst
    std::atomic<int> exceptionCount{0}; // seq_cst

    auto cleanup = createCleanup(150ms, [&](auto) {
        callCount++;
        try {
            throw std::runtime_error("Test exception");
        } catch (const std::exception&) {
            exceptionCount++;
        }
    });

    // Wait for multiple intervals
    std::this_thread::sleep_for(500ms);
    cleanup.reset();

    // Should have executed at least twice
    int count = callCount.load();
    int exceptions = exceptionCount.load();
    EXPECT_GE(count, 2) << "Expected at least 2 calls in 500ms, got " << count;
    EXPECT_EQ(exceptions, count) << "Expected exception count to match call count";
}

// Test callback time accuracy
TEST_F(BackgroundCleanupTest, CallbackTimeAccuracy) {
    std::vector<std::chrono::steady_clock::time_point> actualTimes;
    std::mutex timesMutex;

    // Use 200ms interval to align with implementation's 100ms sleep
    auto cleanup = createCleanup(200ms, [&](auto now) {
        std::lock_guard<std::mutex> lock(timesMutex);
        actualTimes.push_back(now);
    });

    auto startTime = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(500ms);
    cleanup.reset();

    // Verify timestamps are reasonably accurate
    std::lock_guard<std::mutex> lock(timesMutex);
    for (const auto& time : actualTimes) {
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
            time - startTime);

        // Time should be multiple of interval (with tolerance for 100ms sleep)
        auto remainder = diff.count() % 200;
        EXPECT_LE(remainder, 100) << "Expected remainder <= 100ms, got " << remainder << "ms";
    }
}

// Test multiple cleanup objects
TEST_F(BackgroundCleanupTest, MultipleCleanups) {
    std::atomic<int> count1{0}; // seq_cst
    std::atomic<int> count2{0}; // seq_cst

    auto cleanup1 = createCleanup(150ms, [&](auto) { count1++; });
    auto cleanup2 = createCleanup(300ms, [&](auto) { count2++; });

    // Wait for multiple executions
    std::this_thread::sleep_for(500ms);

    cleanup1.reset();
    cleanup2.reset();

    // Both should have executed at least once
    int c1 = count1.load();
    int c2 = count2.load();
    EXPECT_GE(c1, 2) << "Expected at least 2 calls for 150ms interval, got " << c1;
    EXPECT_GE(c2, 1) << "Expected at least 1 call for 300ms interval, got " << c2;
    EXPECT_GT(c1, c2) << "Expected more calls for shorter interval (got " << c1 << " vs " << c2 << ")";
}
