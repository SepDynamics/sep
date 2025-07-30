#ifndef SEP_CORE_SOCKET_LISTENER_H
#define SEP_CORE_SOCKET_LISTENER_H

#include <functional>
#include <memory>
#include <string>

#include "engine/internal/standard_includes.h"

namespace sep {
namespace core {

class SocketListener {
public:
    SocketListener(int port, std::function<void(int)> callback);
    ~SocketListener();

    void start();
    void stop();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace core
} // namespace sep

#endif // SEP_CORE_SOCKET_LISTENER_H