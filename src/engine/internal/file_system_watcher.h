#ifndef SEP_CORE_FILE_SYSTEM_WATCHER_H
#define SEP_CORE_FILE_SYSTEM_WATCHER_H

#include <functional>
#include <memory>
#include <string>

#include "engine/internal/standard_includes.h"

namespace sep {
namespace core {

class FileSystemWatcher {
public:
    FileSystemWatcher(const std::string& path, std::function<void(const std::string&)> callback);
    ~FileSystemWatcher();

    void start();
    void stop();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace core
} // namespace sep

#endif // SEP_CORE_FILE_SYSTEM_WATCHER_H