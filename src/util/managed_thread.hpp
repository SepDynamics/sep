#ifndef SEP_UTIL_MANAGED_THREAD_HPP
#define SEP_UTIL_MANAGED_THREAD_HPP

#include <thread>
#include <utility>

namespace sep::util {

class ManagedThread {
public:
    ManagedThread() = default;

    template <typename Callable, typename... Args>
    explicit ManagedThread(Callable&& func, Args&&... args) {
        start(std::forward<Callable>(func), std::forward<Args>(args)...);
    }

    ~ManagedThread() {
        join();
    }

    ManagedThread(const ManagedThread&) = delete;
    ManagedThread& operator=(const ManagedThread&) = delete;

    ManagedThread(ManagedThread&& other) noexcept
        : thread_(std::move(other.thread_)) {}

    ManagedThread& operator=(ManagedThread&& other) noexcept {
        if (this != &other) {
            join();
            thread_ = std::move(other.thread_);
        }
        return *this;
    }

    template <typename Callable, typename... Args>
    void start(Callable&& func, Args&&... args) {
        if (thread_.joinable()) {
            thread_.join();
        }
        thread_ = std::thread(std::forward<Callable>(func), std::forward<Args>(args)...);
    }

    bool joinable() const noexcept { return thread_.joinable(); }

    void join() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    std::thread::id get_id() const noexcept { return thread_.get_id(); }

private:
    std::thread thread_;
};

} // namespace sep::util

#endif // SEP_UTIL_MANAGED_THREAD_HPP
