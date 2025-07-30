#include "file_system_watcher.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <sys/stat.h>
#include <ctime>

namespace sep {
namespace core {

class FileSystemWatcher::Impl {
public:
    Impl(const std::string& path, std::function<void(const std::string&)> callback)
        : path_(path), callback_(callback), running_(false)
    {
    }

    void start() {
        running_ = true;
        
        // Real file system monitoring implementation using basic polling
        std::thread([this]() {
            std::time_t last_check = std::time(nullptr);
            
            while (running_) {
                try {
                    // Simple polling approach - check if file exists and is newer
                    struct stat st;
                    if (stat(path_.c_str(), &st) == 0) {
                        if (st.st_mtime > last_check) {
                            if (callback_) {
                                callback_(path_);
                            }
                            last_check = st.st_mtime;
                        }
                    }
                } catch (...) {
                    // Ignore errors in monitoring
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Poll every second
            }
        }).detach();
        while (running_) {
            std::cout << "Watching for changes in " << path_ << std::endl;
            // Sleep for a while to avoid busy-waiting.
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void stop() {
        running_ = false;
    }

private:
    std::string path_;
    std::function<void(const std::string&)> callback_;
    bool running_;
};

FileSystemWatcher::FileSystemWatcher(const std::string& path,
                                     std::function<void(const std::string&)> callback)
    : impl_(std::make_unique<Impl>(path, callback))
{
}

FileSystemWatcher::~FileSystemWatcher() = default;

void FileSystemWatcher::start() {
    impl_->start();
}

void FileSystemWatcher::stop() {
    impl_->stop();
}

} // namespace core
} // namespace sep