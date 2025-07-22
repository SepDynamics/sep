#include "api/client.h"
#include "engine/error_handler.h"
#include <stdexcept>
#include <chrono>

namespace sep::api {

CurlHttpClient::CurlHttpClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

CurlHttpClient::~CurlHttpClient() {
    curl_global_cleanup();
}

size_t CurlHttpClient::write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total = size * nmemb;
    std::string *str = static_cast<std::string *>(userp);
    str->append(static_cast<char *>(contents), total);
    return total;
}

APIResponse CurlHttpClient::send_request(const APIRequest &request) {
    auto start_time = std::chrono::steady_clock::now();
    
    APIResponse resp;
    resp.requestId = request.requestId;
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        resp.success = false;
        resp.error.code = ErrorCode::ApiError;
        resp.error.message = "curl_easy_init failed";
        return resp;
    }
    
    std::string buffer;
    curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str()); // Fix: CURLOPT_URL takes const char*
    
    // Set timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, request.timeout.count());
    
    // Set up headers
    struct curl_slist *headers = nullptr; // Fix: curl_slist is a struct
    for (const auto &h : request.headers) {
        std::string header = h.first + ": " + h.second;
        headers = curl_slist_append(headers, header.c_str());
    }
    
    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    
    // Handle different HTTP methods
    if (request.method == "POST") { // Fix: Compare request.method with string literal
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
    } else if (request.method != "GET") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request.method.c_str()); // Fix: CURLOPT_CUSTOMREQUEST takes const char*
        if (!request.body.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
        }
    }
    
    // Set up callbacks
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    
    // Execute request
    CURLcode code = curl_easy_perform(curl);
    
    // Get status code
    long status = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
    resp.statusCode = static_cast<int>(status);
    
    // Process response
    if (code == CURLE_OK) {
        resp.success = status >= 200 && status < 300;
        resp.body = buffer; // Fix: Assign buffer to resp.body
    } else {
        resp.success = false;
        resp.error.code = ErrorCode::ApiError;
        resp.error.message = curl_easy_strerror(code);
    }
    
    // Clean up
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    // Calculate response time
    auto end_time = std::chrono::steady_clock::now();
    resp.responseTime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    return resp;
}
} // namespace sep::api
