#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

namespace sep::api {

class BackgroundCleanup {
public:
    using CleanupCallback = std::function<void(std::chrono::steady_clock::time_point)>;

    explicit BackgroundCleanup(
        std::chrono::milliseconds interval,
        CleanupCallback callback
    ) : interval_(interval),
        callback_(std::move(callback)),
        running_(true) {
        cleanup_thread_ = std::thread([this] { run(); });
    }

    ~BackgroundCleanup() {
        running_ = false;
        if (cleanup_thread_.joinable()) {
            cleanup_thread_.join();
        }
    }

    // Non-copyable, non-movable
    BackgroundCleanup(const BackgroundCleanup&) = delete;
    BackgroundCleanup& operator=(const BackgroundCleanup&) = delete;
    BackgroundCleanup(BackgroundCleanup&&) = delete;
    BackgroundCleanup& operator=(BackgroundCleanup&&) = delete;

private:
    void run() {
        while (running_) {
            auto now = std::chrono::steady_clock::now();
            callback_(now);

            // Sleep for the interval, but be responsive to shutdown
            auto next_cleanup = now + interval_;
            while (running_ && std::chrono::steady_clock::now() < next_cleanup) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

    const std::chrono::milliseconds interval_;
    CleanupCallback callback_;
    std::atomic<bool> running_;
    std::thread cleanup_thread_;
};

} // namespace sep::api
