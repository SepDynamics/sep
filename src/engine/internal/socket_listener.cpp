#include "socket_listener.h"

#include <chrono>
#include <iostream>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

namespace sep {
namespace core {

class SocketListener::Impl {
public:
    Impl(int port, std::function<void(int)> callback)
        : port_(port), callback_(callback), running_(false) {}

    void start() {
        running_ = true;
        
        // Real socket implementation using POSIX sockets
        std::thread([this]() {
            int server_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (server_fd < 0) {
                std::cerr << "Socket creation failed" << std::endl;
                return;
            }
            
            int opt = 1;
            setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            
            struct sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(port_);
            
            if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
                std::cerr << "Socket bind failed on port " << port_ << std::endl;
                close(server_fd);
                return;
            }
            
            if (listen(server_fd, 3) < 0) {
                std::cerr << "Socket listen failed" << std::endl;
                close(server_fd);
                return;
            }
            
            std::cout << "Socket listener started on port " << port_ << std::endl;
            
            while (running_) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                
                int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
                if (client_fd >= 0) {
                    if (callback_) {
                        callback_(client_fd);
                    }
                    close(client_fd);
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
            close(server_fd);
        }).detach();
        while (running_) {
            std::cout << "Listening for connections on port " << port_ << std::endl;
            // Sleep for a while to avoid busy-waiting.
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void stop() {
        running_ = false;
    }

private:
    int port_;
    std::function<void(int)> callback_;
    bool running_;
};

SocketListener::SocketListener(int port, std::function<void(int)> callback)
    : impl_(std::make_unique<Impl>(port, callback)) {}

SocketListener::~SocketListener() = default;

void SocketListener::start() {
    impl_->start();
}

void SocketListener::stop() {
    impl_->stop();
}

} // namespace core
} // namespace sep