#include "mock_bridge.h"
#include <cstring>
#include <cstdio>

#include "sep/error_codes.h"

namespace sep::test {

// Global mutex used because C API exposes shared state
std::mutex g_mock_mutex;
std::string g_mock_result;
std::string g_mock_error;
int g_mock_required_size = 0;
bool g_should_fail = false;
bool g_should_resize = false;

} // namespace sep::test

extern "C" {

int sep_process_context(const char *context_json, const char *layer,
                        char *result_buffer, size_t buffer_size) {
  sep::test::MockGuard guard;
  (void)context_json;
  (void)layer;
  if (sep::test::g_should_fail) {
    return static_cast<int>(sep::ErrorCode::GeneralError);
  }

  if (sep::test::g_should_resize) {
    return static_cast<int>(sep::ErrorCode::BufferTooSmall);
  }

  if (buffer_size < sep::test::g_mock_result.size() + 1) {
    return static_cast<int>(sep::ErrorCode::BufferTooSmall);
  }

  (void)std::snprintf(result_buffer, buffer_size, "%s", sep::test::g_mock_result.c_str());
  return 0;
}

size_t sep_get_required_buffer_size() {
  sep::test::MockGuard guard;
  return sep::test::g_mock_required_size;
}

int sep_bridge_get_last_error(char *error_buffer, size_t buffer_size) {
  sep::test::MockGuard guard;
  (void)std::snprintf(error_buffer, buffer_size, "%s", sep::test::g_mock_error.c_str());
  return static_cast<int>(
      std::min(buffer_size - 1, sep::test::g_mock_error.length()));
}

} // extern "C"
