#include "service_connector.hpp"
#include "core/engine.h"
#include <iostream>
#include <thread>
#include <cstring>

// Platform-specific includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

namespace sep::workbench {

ServiceConnector::ServiceConnector() : ServiceConnector(ConnectionConfig{}) {}

ServiceConnector::ServiceConnector(const ConnectionConfig& config) 
    : config_(config) {
    std::cout << "[ServiceConnector] Initialized with config: " 
              << config.service_address << ":" << config.service_port << std::endl;
}

ServiceConnector::~ServiceConnector() {
    disconnect();
    stopHealthMonitoring();
}

bool ServiceConnector::connect() {
    std::cout << "[ServiceConnector] Attempting connection to SEP service..." << std::endl;
    
    if (connection_state_ == ConnectionState::CONNECTED) {
        std::cout << "[ServiceConnector] Already connected" << std::endl;
        return true;
    }
    
    connection_state_ = ConnectionState::CONNECTING;
    
    // Try different connection methods in order of preference
    bool connected = false;
    
    // 1. Try shared memory (fastest)
    connected = connectSharedMemory();
    
    // 2. Try IPC (Unix domain sockets / named pipes)
    if (!connected) {
        connected = connectIPC();
    }
    
    // 3. Try TCP (most portable)
    if (!connected) {
        connected = connectTCP();
    }
    
    if (connected) {
        // Validate service version
        if (!validateServiceVersion()) {
            std::cerr << "[ServiceConnector] Service version mismatch" << std::endl;
            disconnect();
            return false;
        }
        
        connection_state_ = ConnectionState::CONNECTED;
        health_metrics_.last_heartbeat = std::chrono::steady_clock::now();
        
        // Start health monitoring
        startHealthMonitoring();
        
        // Notify callback
        if (connection_callback_) {
            connection_callback_(ConnectionState::CONNECTED);
        }
        
        std::cout << "[ServiceConnector] Successfully connected to SEP service" << std::endl;
        return true;
    }
    
    connection_state_ = ConnectionState::ERROR;
    std::cerr << "[ServiceConnector] Failed to connect to SEP service" << std::endl;
    
    // For now, create a mock engine for offline mode
    // In production, this would return false
    std::cout << "[ServiceConnector] Creating mock engine for offline mode" << std::endl;
    return false;
}

void ServiceConnector::disconnect() {
    if (connection_state_ == ConnectionState::DISCONNECTED) {
        return;
    }
    
    std::cout << "[ServiceConnector] Disconnecting from SEP service..." << std::endl;
    
    stopHealthMonitoring();
    
    // Clean up connection
    if (service_handle_) {
        // Platform-specific cleanup
#ifdef _WIN32
        closesocket(reinterpret_cast<SOCKET>(service_handle_));
#else
        close(reinterpret_cast<intptr_t>(service_handle_));
#endif
        service_handle_ = nullptr;
    }
    
    service_engine_ = nullptr;
    connection_state_ = ConnectionState::DISCONNECTED;
    
    // Notify callback
    if (connection_callback_) {
        connection_callback_(ConnectionState::DISCONNECTED);
    }
    
    std::cout << "[ServiceConnector] Disconnected from SEP service" << std::endl;
}

bool ServiceConnector::reconnect() {
    disconnect();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return connect();
}

ServiceHealth ServiceConnector::getServiceHealth() const {
    return health_metrics_;
}

void ServiceConnector::startHealthMonitoring() {
    if (monitoring_active_) {
        return;
    }
    
    monitoring_active_ = true;
    monitor_thread_ = std::make_unique<std::thread>(&ServiceConnector::monitoringLoop, this);
    
    std::cout << "[ServiceConnector] Health monitoring started" << std::endl;
}

void ServiceConnector::stopHealthMonitoring() {
    if (!monitoring_active_) {
        return;
    }
    
    monitoring_active_ = false;
    
    if (monitor_thread_ && monitor_thread_->joinable()) {
        monitor_thread_->join();
    }
    
    monitor_thread_.reset();
    
    std::cout << "[ServiceConnector] Health monitoring stopped" << std::endl;
}

void ServiceConnector::monitoringLoop() {
    while (monitoring_active_) {
        if (connection_state_ == ConnectionState::CONNECTED) {
            // Send heartbeat
            if (!sendHeartbeat()) {
                std::cerr << "[ServiceConnector] Heartbeat failed" << std::endl;
                
                // Check if we should attempt reconnection
                if (config_.auto_reconnect) {
                    std::cout << "[ServiceConnector] Attempting auto-reconnect..." << std::endl;
                    reconnect();
                }
            } else {
                // Update health metrics
                updateHealthMetrics();
            }
        }
        
        // Sleep for heartbeat interval
        std::this_thread::sleep_for(config_.heartbeat_interval);
    }
}

bool ServiceConnector::sendHeartbeat() {
    if (connection_state_ != ConnectionState::CONNECTED || !service_handle_) {
        return false;
    }
    
    // Simple heartbeat message
    const char* heartbeat_msg = "HEARTBEAT\n";
    
    // Platform-specific send
#ifdef _WIN32
    int result = send(reinterpret_cast<SOCKET>(service_handle_), 
                     heartbeat_msg, strlen(heartbeat_msg), 0);
#else
    ssize_t result = send(reinterpret_cast<intptr_t>(service_handle_), 
                         heartbeat_msg, strlen(heartbeat_msg), MSG_NOSIGNAL);
#endif
    
    if (result > 0) {
        health_metrics_.last_heartbeat = std::chrono::steady_clock::now();
        health_metrics_.is_responsive = true;
        return true;
    }
    
    health_metrics_.is_responsive = false;
    return false;
}

void ServiceConnector::updateHealthMetrics() {
    // Calculate latency
    auto now = std::chrono::steady_clock::now();
    health_metrics_.latency = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - health_metrics_.last_heartbeat
    );
    
    // In production, these would be queried from the service
    health_metrics_.processed_patterns = 0;
    health_metrics_.coherence_average = 0.0f;
}

bool ServiceConnector::validateServiceVersion() {
    // In production, this would query the service for version info
    health_metrics_.version_info = "SEP Service v0.1.0-mock";
    return true;
}

bool ServiceConnector::connectSharedMemory() {
    std::cout << "[ServiceConnector] Trying shared memory connection..." << std::endl;
    
    // TODO: Implement shared memory connection
    // This would use platform-specific shared memory APIs
    
    return false;
}

bool ServiceConnector::connectIPC() {
    std::cout << "[ServiceConnector] Trying IPC connection..." << std::endl;
    
    // TODO: Implement IPC connection
    // Unix: Domain sockets
    // Windows: Named pipes
    
    return false;
}

bool ServiceConnector::connectTCP() {
    std::cout << "[ServiceConnector] Trying TCP connection to " 
              << config_.service_address << ":" << config_.service_port << "..." << std::endl;
    
#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[ServiceConnector] WSAStartup failed" << std::endl;
        return false;
    }
#endif
    
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[ServiceConnector] Failed to create socket" << std::endl;
        return false;
    }
    
    // Set socket timeout
    struct timeval timeout;
    timeout.tv_sec = config_.connection_timeout.count() / 1000;
    timeout.tv_usec = (config_.connection_timeout.count() % 1000) * 1000;
    
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    
    // Setup server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config_.service_port);
    
    if (inet_pton(AF_INET, config_.service_address.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "[ServiceConnector] Invalid address: " << config_.service_address << std::endl;
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return false;
    }
    
    // Attempt connection
    if (::connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[ServiceConnector] Connection failed: " << strerror(errno) << std::endl;
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return false;
    }
    
    std::cout << "[ServiceConnector] TCP connection established" << std::endl;
    
    // Store socket handle
    service_handle_ = reinterpret_cast<void*>(static_cast<intptr_t>(sock));
    
    // In production, this would establish protocol and get engine reference
    // For now, return false to indicate no real service
    
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    service_handle_ = nullptr;
    
    return false;
}

} // namespace sep::workbench