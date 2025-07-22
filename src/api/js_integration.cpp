#include "api/js_integration.h"
#include "api/bridge.h"
#include "api/types.h"  // For sep::api::ErrorCode

#include <string>

namespace sep::api {

std::string JSIntegration::processContextCheck(const std::string& context_json,
                                               const std::string& layer) {
    size_t buffer_size = initial_buffer_size;
    std::string result_buffer(buffer_size, '\0');
    int ret_code;

    do {
        ret_code = sep_process_context(
            context_json.c_str(),
            layer.c_str(),
            result_buffer.data(), // result_buffer.data() is char*
            buffer_size           // buffer_size is size_t, matching API
        );

        if (ret_code == static_cast<int>(ErrorCode::BufferTooSmall)) { // Buffer too small
            size_t required = sep_get_required_buffer_size();
            if (required > 0) {
                buffer_size = required;
            } else {
                buffer_size *= 2; // Fallback doubling
            }
            result_buffer.resize(buffer_size);
        }
    } while (ret_code == static_cast<int>(ErrorCode::BufferTooSmall));

    if (ret_code != 0) {
        char error_buffer[1024] = {0};
        sep_bridge_get_last_error(error_buffer, sizeof(error_buffer));
        return std::string("{\"error\":\"") + error_buffer + "\"}";
    }

    // Trim to actual content length
    size_t actual_length = 0;
    while (actual_length < buffer_size && result_buffer[actual_length] != '\0') {
        ++actual_length;
    }

    return result_buffer.substr(0, actual_length);
}
} // namespace sep::api
