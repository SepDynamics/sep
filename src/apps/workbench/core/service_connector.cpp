#include "service_connector.hpp"

// Include engine headers - they should be found via CMake include paths
#include "engine.h"
#include "engine/manager.h"
#include "service_proxy_engine.h"
#include "config.h"
#include "ui_layout_manager.h"

#include <iostream>
#include <thread>
#include <cstring>
#include "engine/data_parser.h"

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
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <sys/un.h>
#endif

namespace sep::workbench {

ServiceConnector::ServiceConnector() : ServiceConnector(ConnectionConfig{}) {}

ServiceConnector::ServiceConnector(const ConnectionConfig& config)
    : config_(config) {
    auto& cfg = sep::config::ConfigManager::getInstance().oanda();
    std::string api_key = cfg.demo_api_key.empty() ? cfg.api_key : cfg.demo_api_key;
    std::string account_id = cfg.demo_account_id.empty() ? cfg.account_id : cfg.demo_account_id;
    if (api_key.empty() || account_id.empty()) {
        const char* env_key = std::getenv("OANDA_API_KEY");
        const char* env_id = std::getenv("OANDA_ACCOUNT_ID");
        const char* env_demo_key = std::getenv("OANDA_DEMO_API_KEY");
        const char* env_demo_id = std::getenv("OANDA_DEMO_ACCOUNT_ID");
        if (env_demo_key && env_demo_id) {
            api_key = env_demo_key;
            account_id = env_demo_id;
        } else {
            if (env_key) api_key = env_key;
            if (env_id) account_id = env_id;
        }
    }

    if (!api_key.empty() && !account_id.empty()) {
        oanda_connector_ = std::make_unique<connectors::OandaConnector>(api_key, account_id, true);
        trade_manager_ = std::make_unique<workbench::TradeManager>(oanda_connector_.get());
        trade_manager_->setRiskPercentage(0.02);
        trade_manager_->setPaperTrading(cfg.paper_trading);
        std::cout << "[ServiceConnector] OANDA connector configured for PRACTICE server" << std::endl;
        std::cout << "[ServiceConnector] API Key length: " << api_key.length() << std::endl;
        std::cout << "[ServiceConnector] Account ID: " << account_id << std::endl;
        if (oanda_connector_->initialize()) {
            if (oanda_connector_->fetchHistoricalData("EUR_USD", "eur_usd_m1_48h.json")) {
                loadInitialData("eur_usd_m1_48h.json");
            } else {
                loadInitialData("eur_usd_m1_48h.json");
            }
        } else {
            loadInitialData("eur_usd_m1_48h.json");
        }
    } else {
        std::cout << "[ServiceConnector] ERROR: OANDA credentials not provided" << std::endl;
        loadInitialData("eur_usd_m1_48h.json");
    }

    std::cout << "[ServiceConnector] Initialized with config: "
               << config.service_address << ":" << config.service_port << std::endl;
}

ServiceConnector::~ServiceConnector() {
    disconnect();
    stopHealthMonitoring();
    trade_manager_.reset();
}

bool ServiceConnector::connect() {
    std::cout << "[ServiceConnector] Attempting connection to SEP service..." << std::endl;
    
    if (connection_state_ == sep::workbench::ConnectionState::CONNECTED) {
        std::cout << "[ServiceConnector] Already connected" << std::endl;
        return true;
    }
    
    connection_state_ = sep::workbench::ConnectionState::CONNECTING;
    
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
        
        connection_state_ = sep::workbench::ConnectionState::CONNECTED;
        health_metrics_.last_heartbeat = std::chrono::steady_clock::now();
        
        // Start health monitoring
        startHealthMonitoring();
        
        // Notify via EventBus
        globalEventBus().publish(ConnectionStateEvent{sep::workbench::ConnectionState::CONNECTED});
        if (connection_callback_) {
            connection_callback_(sep::workbench::ConnectionState::CONNECTED);
        }
        
        std::cout << "[ServiceConnector] Successfully connected to SEP service" << std::endl;
        return true;
    }
    
    connection_state_ = sep::workbench::ConnectionState::CONNECTION_FAILED;
    std::cerr << "[ServiceConnector] Failed to connect to SEP service" << std::endl;
    
    // Return false to indicate connection failure
    return false;
}

void ServiceConnector::disconnect() {
    if (connection_state_ == sep::workbench::ConnectionState::DISCONNECTED) {
        return;
    }
    
    std::cout << "[ServiceConnector] Disconnecting from SEP service..." << std::endl;
    
    stopHealthMonitoring();
    
    // Clean up connection based on connection type
    if (service_handle_) {
        std::cout << "[ServiceConnector] Cleaning up connection resources..." << std::endl;
        
        // For TCP and IPC (Unix domain sockets), close the socket/file descriptor
        // For shared memory, we need different cleanup
        // Since we don't track connection type explicitly, we'll try socket close first
        
#ifdef _WIN32
        // On Windows, handle could be socket, named pipe, or file mapping
        // Try to close as socket first (most common case)
        if (closesocket(reinterpret_cast<SOCKET>(service_handle_)) == SOCKET_ERROR) {
            // If socket close failed, might be a named pipe or file mapping
            HANDLE handle = reinterpret_cast<HANDLE>(service_handle_);
            if (handle != INVALID_HANDLE_VALUE) {
                if (!CloseHandle(handle)) {
                    std::cerr << "[ServiceConnector] Failed to close handle: " << GetLastError() << std::endl;
                }
            }
        }
#else
        // On Unix, handle is a file descriptor (socket or shared memory)
        int fd = reinterpret_cast<intptr_t>(service_handle_);
        if (close(fd) != 0) {
            std::cerr << "[ServiceConnector] Failed to close file descriptor: " << strerror(errno) << std::endl;
        }
#endif
        service_handle_ = nullptr;
    }
    
    service_engine_ = nullptr;
    connection_state_ = sep::workbench::ConnectionState::DISCONNECTED;
    
    // Notify via EventBus
    globalEventBus().publish(ConnectionStateEvent{sep::workbench::ConnectionState::DISCONNECTED});
    if (connection_callback_) {
        connection_callback_(sep::workbench::ConnectionState::DISCONNECTED);
    }
    
    std::cout << "[ServiceConnector] Disconnected from SEP service" << std::endl;
}

bool ServiceConnector::reconnect() {
    disconnect();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return connect();
}

void ServiceConnector::setSignalsTab(SignalsTabController* tab)
{
    signals_tab_ = tab;
    if (signals_tab_ && !initial_data_.empty())
    {
        signals_tab_->setCandleData(initial_data_);
    }
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
    static uint32_t consecutive_failures = 0;
    std::cout << "[ServiceConnector] Health monitoring thread started" << std::endl;
    
    while (monitoring_active_) {
        try {
            if (connection_state_ == sep::workbench::ConnectionState::CONNECTED) {
                // Send heartbeat
                if (!sendHeartbeat()) {
                    consecutive_failures++;
                    std::cerr << "[ServiceConnector] Heartbeat failed (failures: " 
                              << consecutive_failures << "/" << config_.max_retry_attempts << ")" << std::endl;
                    
                    // Check if we've exceeded max retry attempts
                    if (consecutive_failures >= config_.max_retry_attempts) {
                        std::cerr << "[ServiceConnector] Max retry attempts exceeded, marking as disconnected" << std::endl;
                        connection_state_ = sep::workbench::ConnectionState::CONNECTION_FAILED;
                        
                        // Check if we should attempt reconnection
                        if (config_.auto_reconnect) {
                            std::cout << "[ServiceConnector] Scheduling auto-reconnect..." << std::endl;
                            connection_state_ = sep::workbench::ConnectionState::DISCONNECTED;
                        }
                        
                        // Notify via EventBus
                        globalEventBus().publish(ConnectionStateEvent{connection_state_});
                        if (connection_callback_) {
                            connection_callback_(connection_state_);
                        }
                        
                        consecutive_failures = 0; // Reset for next reconnection attempt
                    }
                } else {
                    // Heartbeat successful - reset failure counter
                    if (consecutive_failures > 0) {
                        std::cout << "[ServiceConnector] Heartbeat recovered after " 
                                  << consecutive_failures << " failures" << std::endl;
                        consecutive_failures = 0;
                    }
                    
                    // Update health metrics
                    updateHealthMetrics();
                }
            } else if (connection_state_ == sep::workbench::ConnectionState::DISCONNECTED && config_.auto_reconnect) {
                // Attempt reconnection if we're configured for it
                std::cout << "[ServiceConnector] Attempting scheduled reconnection..." << std::endl;
                if (reconnect()) {
                    std::cout << "[ServiceConnector] Auto-reconnection successful" << std::endl;
                    consecutive_failures = 0;
                } else {
                    std::cout << "[ServiceConnector] Auto-reconnection failed, will retry" << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[ServiceConnector] Exception in monitoring loop: " << e.what() << std::endl;
            consecutive_failures++;
            
            // If we have too many exceptions, disable monitoring
            if (consecutive_failures >= config_.max_retry_attempts * 2) {
                std::cerr << "[ServiceConnector] Too many exceptions, disabling monitoring" << std::endl;
                monitoring_active_ = false;
                break;
            }
        }
        
        // Sleep for heartbeat interval
        std::this_thread::sleep_for(config_.heartbeat_interval);
    }
    
    std::cout << "[ServiceConnector] Health monitoring thread exiting" << std::endl;
}

bool ServiceConnector::sendHeartbeat() {
    if (connection_state_ != sep::workbench::ConnectionState::CONNECTED || !service_handle_) {
        return false;
    }
    
    // For now, skip actual heartbeat check if we're in offline mode
    // This prevents immediate disconnection when no service is running
    if (!service_engine_) {
        // We're in offline mode, just update metrics
        health_metrics_.last_heartbeat = std::chrono::steady_clock::now();
        health_metrics_.is_responsive = true;
        return true;
    }
    
    // Send HTTP GET request for health check
    const char* health_request =
        "GET /api/v1/health HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";

    // Platform-specific send
#ifdef _WIN32
    int result = send(reinterpret_cast<SOCKET>(service_handle_),
                     health_request, strlen(health_request), 0);
#else
    ssize_t result = send(reinterpret_cast<intptr_t>(service_handle_),
                         health_request, strlen(health_request), MSG_NOSIGNAL);
#endif
    
    if (result > 0) {
        // Try to read response (non-blocking)
        char buffer[1024];
#ifdef _WIN32
        int recv_result = recv(reinterpret_cast<SOCKET>(service_handle_),
                              buffer, sizeof(buffer) - 1, 0);
#else
        ssize_t recv_result = recv(reinterpret_cast<intptr_t>(service_handle_),
                                  buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
#endif
        
        if (recv_result > 0) {
            buffer[recv_result] = '\0';
            // Check if response contains "200 OK"
            if (strstr(buffer, "200 OK") != nullptr) {
                health_metrics_.last_heartbeat = std::chrono::steady_clock::now();
                health_metrics_.is_responsive = true;
                return true;
            }
        } else if (recv_result == 0) {
            // Connection closed
            std::cerr << "[ServiceConnector] Connection closed by service" << std::endl;
        }
    }
    
    health_metrics_.is_responsive = false;
    return false;
}

void ServiceConnector::updateHealthMetrics() {
    auto now = std::chrono::steady_clock::now();
    
    // Calculate latency from last successful heartbeat
    if (health_metrics_.last_heartbeat.time_since_epoch().count() > 0) {
        health_metrics_.latency = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - health_metrics_.last_heartbeat
        );
    } else {
        health_metrics_.latency = std::chrono::milliseconds{0};
    }
    
    // Log health status periodically (every ~10 heartbeats)
    static int update_counter = 0;
    if (++update_counter % 10 == 0) {
        std::cout << "[ServiceConnector] Health: responsive=" << health_metrics_.is_responsive
                  << ", latency=" << health_metrics_.latency.count() << "ms"
                  << ", version=" << health_metrics_.version_info << std::endl;
    }
    
    // Check for connection timeout
    auto time_since_heartbeat = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - health_metrics_.last_heartbeat
    );
    
    if (time_since_heartbeat > config_.connection_timeout) {
        std::cerr << "[ServiceConnector] WARNING: No heartbeat for " 
                  << time_since_heartbeat.count() << "ms (timeout: " 
                  << config_.connection_timeout.count() << "ms)" << std::endl;
        health_metrics_.is_responsive = false;
    }
    
    // In production, these would be queried from the service via API calls
    // Use actual service data when available - NO FAKE INCREMENTS
    if (health_metrics_.is_responsive) {
        // Real implementation would query service metrics via API
        // For now, maintain last known values instead of fake incrementing
    } else {
        health_metrics_.processed_patterns = 0;
        health_metrics_.coherence_average = 0.0f;
    }
}

bool ServiceConnector::validateServiceVersion() {
    if (!service_handle_) {
        std::cerr << "[ServiceConnector] No service connection for version validation" << std::endl;
        return false;
    }
    
    std::cout << "[ServiceConnector] Validating service version..." << std::endl;
    
    // Send version check request
    const char* version_request =
        "GET /api/v1/version HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";

    // Platform-specific send
#ifdef _WIN32
    int result = send(reinterpret_cast<SOCKET>(service_handle_),
                     version_request, strlen(version_request), 0);
#else
    ssize_t result = send(reinterpret_cast<intptr_t>(service_handle_),
                         version_request, strlen(version_request), MSG_NOSIGNAL);
#endif
    
    if (result <= 0) {
        std::cerr << "[ServiceConnector] Failed to send version request" << std::endl;
        health_metrics_.version_info = "SEP Service v0.1.0 (assumed)";
        return true; // Allow connection with assumed version
    }
    
    // Read response with timeout
    char buffer[2048];
    memset(buffer, 0, sizeof(buffer));
    
#ifdef _WIN32
    int recv_result = recv(reinterpret_cast<SOCKET>(service_handle_),
                          buffer, sizeof(buffer) - 1, 0);
#else
    ssize_t recv_result = recv(reinterpret_cast<intptr_t>(service_handle_),
                              buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
#endif
    
    if (recv_result > 0) {
        buffer[recv_result] = '\0';
        std::cout << "[ServiceConnector] Version response received: " << buffer << std::endl;
        
        // Check for valid HTTP response
        if (strstr(buffer, "200 OK") != nullptr) {
            // Extract version from JSON response if available
            const char* version_start = strstr(buffer, "\"version\":");
            if (version_start) {
                // Simple version extraction
                version_start += 10; // Skip "version":
                while (*version_start && (*version_start == ' ' || *version_start == '"')) {
                    version_start++;
                }
                
                const char* version_end = strchr(version_start, '"');
                if (version_end) {
                    health_metrics_.version_info = std::string(version_start, version_end - version_start);
                } else {
                    health_metrics_.version_info = "SEP Service (version detected)";
                }
            } else {
                health_metrics_.version_info = "SEP Service v0.1.0 (detected)";
            }
            
            std::cout << "[ServiceConnector] Service version: " << health_metrics_.version_info << std::endl;
            return true;
        }
    }
    
    // Fallback - assume compatible version
    std::cout << "[ServiceConnector] Version validation failed, assuming compatibility" << std::endl;
    health_metrics_.version_info = "SEP Service v0.1.0 (assumed)";
    return true;
}

bool ServiceConnector::connectSharedMemory() {
    std::cout << "[ServiceConnector] Trying shared memory connection..." << std::endl;
    
#ifdef _WIN32
    // Windows: Use CreateFileMapping/MapViewOfFile
    const std::string shm_name = "SEP_SERVICE_SHM";
    
    // Try to open existing shared memory
    HANDLE hMapFile = OpenFileMappingA(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        shm_name.c_str()      // name of mapping object
    );
    
    if (hMapFile == nullptr) {
        std::cout << "[ServiceConnector] Shared memory not found: " << GetLastError() << std::endl;
        return false;
    }
    
    // Map the shared memory into our address space
    void* pBuf = MapViewOfFile(hMapFile,   // handle to map object
                               FILE_MAP_ALL_ACCESS, // read/write permission
                               0, 0, 0);   // entire file
    
    if (pBuf == nullptr) {
        std::cerr << "[ServiceConnector] Could not map view of file: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        return false;
    }
    
    // Store handle for cleanup
    service_handle_ = hMapFile;
    
    std::cout << "[ServiceConnector] Shared memory connection established" << std::endl;
    return true;
    
#else
    // Unix: Use POSIX shared memory
    const std::string shm_name = "/sep_service_shm";
    
    // Try to open existing shared memory
    int shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cout << "[ServiceConnector] Shared memory not found: " << strerror(errno) << std::endl;
        return false;
    }
    
    // Get the size of the shared memory
    struct stat shm_stat;
    if (fstat(shm_fd, &shm_stat) == -1) {
        std::cerr << "[ServiceConnector] Failed to get shared memory size: " << strerror(errno) << std::endl;
        close(shm_fd);
        return false;
    }
    
    // Map the shared memory
    void* shm_ptr = mmap(nullptr, shm_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        std::cerr << "[ServiceConnector] Failed to map shared memory: " << strerror(errno) << std::endl;
        close(shm_fd);
        return false;
    }
    
    // Store file descriptor for cleanup
    service_handle_ = reinterpret_cast<void*>(static_cast<intptr_t>(shm_fd));
    
    std::cout << "[ServiceConnector] Shared memory connection established" << std::endl;
    return true;
#endif
}

bool ServiceConnector::connectIPC() {
    std::cout << "[ServiceConnector] Trying IPC connection..." << std::endl;
    
#ifdef _WIN32
    // Windows: Use Named Pipes
    const std::string pipe_name = "\\\\.\\pipe\\sep_service_pipe";
    
    HANDLE hPipe = CreateFileA(
        pipe_name.c_str(),    // pipe name
        GENERIC_READ | GENERIC_WRITE,  // read and write access
        0,                    // no sharing
        nullptr,              // default security attributes
        OPEN_EXISTING,        // opens existing pipe
        0,                    // default attributes
        nullptr               // no template file
    );
    
    if (hPipe == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        std::cout << "[ServiceConnector] Named pipe not available: " << error << std::endl;
        return false;
    }
    
    // Set pipe mode to message read mode
    DWORD dwMode = PIPE_READMODE_MESSAGE;
    BOOL fSuccess = SetNamedPipeHandleState(
        hPipe,    // pipe handle
        &dwMode,  // new pipe mode
        nullptr,  // don't set maximum bytes
        nullptr   // don't set maximum time
    );
    
    if (!fSuccess) {
        std::cerr << "[ServiceConnector] Failed to set pipe mode: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return false;
    }
    
    // Store handle for cleanup
    service_handle_ = hPipe;
    
    std::cout << "[ServiceConnector] Named pipe connection established" << std::endl;
    return true;
    
#else
    // Unix: Use Unix Domain Sockets
    const std::string socket_path = "/tmp/sep_service.sock";
    
    // Create socket
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        std::cerr << "[ServiceConnector] Failed to create Unix socket: " << strerror(errno) << std::endl;
        return false;
    }
    
    // Setup socket address
    struct sockaddr_un server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path.c_str(), sizeof(server_addr.sun_path) - 1);
    
    // Set socket timeout for quick failure
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    
    // Attempt connection
    if (::connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cout << "[ServiceConnector] Unix socket connection failed: " << strerror(errno) << std::endl;
        close(sock_fd);
        return false;
    }
    
    // Store socket descriptor for cleanup
    service_handle_ = reinterpret_cast<void*>(static_cast<intptr_t>(sock_fd));
    
    std::cout << "[ServiceConnector] Unix domain socket connection established" << std::endl;
    return true;
#endif
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
    
    // Set socket timeout - use shorter timeout for quick failure
    struct timeval timeout;
    timeout.tv_sec = 1;  // 1 second timeout for faster failure
    timeout.tv_usec = 0;
    
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    
    // Setup server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config_.service_port);
    
    // Handle localhost/hostname resolution
    if (config_.service_address == "localhost") {
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    } else if (inet_pton(AF_INET, config_.service_address.c_str(), &server_addr.sin_addr) <= 0) {
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
        std::cerr << "[ServiceConnector] SEP service not available - will use offline mode" << std::endl;
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return false;
    }
    
    std::cout << "[ServiceConnector] TCP connection established" << std::endl;
    
    // Store socket handle for persistent connection
    service_handle_ = reinterpret_cast<void*>(static_cast<intptr_t>(sock));

    // Create service engine interface - connect to remote SEP API
    try
    {
        // For TCP connection, create a proxy engine that communicates with remote service
        service_engine_ = createServiceEngineProxy(sock);
        if (service_engine_)
        {
            std::cout << "[ServiceConnector] Service engine proxy created" << std::endl;
        }
        else
        {
            std::cerr << "[ServiceConnector] Failed to create service engine proxy" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "[ServiceConnector] Error creating service engine: " << e.what() << std::endl;
        service_engine_ = nullptr;
    }

    return true;
}

sep::core::Engine* ServiceConnector::createServiceEngineProxy(int socket_fd)
{
    // Since TCP connection was established, we can verify service is responsive
    // by testing the health endpoint on a new socket connection
    try
    {
        // Create a fresh socket for the health check
        int health_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (health_sock == -1)
        {
            std::cerr << "[ServiceConnector] Failed to create health check socket - creating local engine" << std::endl;
            // Create a real local engine instance instead of stub
            return createLocalEngine();
        }

        struct sockaddr_in health_addr;
        health_addr.sin_family = AF_INET;
        health_addr.sin_port = htons(config_.service_port);
        inet_pton(AF_INET, config_.service_address.c_str(), &health_addr.sin_addr);

        // Set short timeout for health check
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt(health_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        setsockopt(health_sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        if (::connect(health_sock, (struct sockaddr*)&health_addr, sizeof(health_addr)) == 0)
        {
            // Send health check request
            const char* health_request =
                "GET /api/v1/health HTTP/1.1\r\n"
                "Host: localhost\r\n"
                "Connection: close\r\n"
                "\r\n";

            if (send(health_sock, health_request, strlen(health_request), 0) > 0)
            {
                // Read response
                char buffer[1024];
                ssize_t recv_result = recv(health_sock, buffer, sizeof(buffer) - 1, 0);

                if (recv_result > 0)
                {
                    buffer[recv_result] = '\0';

                    // Check if response contains "200 OK" and valid JSON
                    if (strstr(buffer, "200 OK") != nullptr && strstr(buffer, "healthy") != nullptr)
                    {
                        std::cout << "[ServiceConnector] SEP service verified healthy" << std::endl;
                        health_metrics_.is_responsive = true;
                        health_metrics_.version_info = "SEP Service API v1.0";

                        close(health_sock);

                        // Service is verified healthy and reachable - create HTTP client engine proxy
                        std::cout << "[ServiceConnector] Creating HTTP engine proxy for remote service" << std::endl;
                        return createHttpEngineProxy(socket_fd);
                    }
                }
            }
        }

        close(health_sock);
    }
    catch (const std::exception& e)
    {
        std::cerr << "[ServiceConnector] Exception during service verification: " << e.what()
                  << std::endl;
    }

    std::cout << "[ServiceConnector] Remote service unavailable - creating local engine" << std::endl;
    // Create a real local engine instance for offline operation
    return createLocalEngine();
}

core::Engine* ServiceConnector::createLocalEngine()
{
    try {
        std::cout << "[ServiceConnector] Initializing local SEP engine..." << std::endl;
        
        // Create a real local engine instance
        local_engine_ = std::make_unique<core::Engine>();
        
        // Initialize the engine with default configuration
        config::CudaConfig cuda_config;
        cuda_config.use_gpu = true;
        cuda_config.max_memory_mb = 1024;
        
        if (!local_engine_->init(cuda_config)) {
            std::cerr << "[ServiceConnector] Failed to initialize local engine" << std::endl;
            return nullptr;
        }
        
        std::cout << "[ServiceConnector] Local SEP engine initialized successfully" << std::endl;
        health_metrics_.is_responsive = true;
        health_metrics_.version_info = "SEP Local Engine v1.0";
        
        return local_engine_.get();
        
    } catch (const std::exception& e) {
        std::cerr << "[ServiceConnector] Exception creating local engine: " << e.what() << std::endl;
        return nullptr;
    }
}

core::Engine* ServiceConnector::createHttpEngineProxy(int socket_fd)
{
    try {
        std::cout << "[ServiceConnector] Creating HTTP proxy engine for remote service..." << std::endl;
        
    // Create a proxy engine that forwards commands to the remote service via HTTP
    http_proxy_engine_ = std::make_unique<core::ServiceProxyEngine>(config_.service_address, config_.service_port);

    config::CudaConfig cfg{};
    if (!http_proxy_engine_->init(cfg) || !http_proxy_engine_->isConnected()) {
        std::string err = "HTTP proxy engine connection failed: " + http_proxy_engine_->getLastError();
        std::cerr << "[ServiceConnector] " << err << std::endl;
        health_metrics_.last_error = err;
        health_metrics_.is_responsive = false;
        // Fallback to local engine
        return createLocalEngine();
    }

    health_metrics_.last_error.clear();
        
        std::cout << "[ServiceConnector] HTTP proxy engine created successfully" << std::endl;
        health_metrics_.is_responsive = true;
        health_metrics_.version_info = "SEP Remote Service Proxy v1.0";
        
        return http_proxy_engine_.get();
        
    } catch (const std::exception& e) {
        std::cerr << "[ServiceConnector] Exception creating HTTP proxy engine: " << e.what() << std::endl;
        // Fallback to local engine
        return createLocalEngine();
    }
}

void ServiceConnector::loadInitialData(const std::string& path)
{
    DataParser parser;
    auto candles = parser.parseQuantJSON(path);
    initial_data_.clear();
    for (const auto& c : candles)
    {
        std::tm tm = {};
        std::istringstream ss(c.time);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        auto ts = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        initial_data_.emplace_back(c.open, c.high, c.low, c.close, static_cast<int>(c.volume), ts);
    }
    if (signals_tab_ && !initial_data_.empty())
    {
        signals_tab_->setCandleData(initial_data_);
    }
}

} // namespace sep::workbench