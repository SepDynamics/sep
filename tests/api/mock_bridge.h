#ifndef SEP_TEST_MOCK_BRIDGE_H
#define SEP_TEST_MOCK_BRIDGE_H

#include <mutex>
#include <string>

namespace sep::test {

// Thread-safe mock state
// Global mutex since these helpers are used across tests
extern std::mutex g_mock_mutex;
extern std::string g_mock_result;
extern std::string g_mock_error;
extern int g_mock_required_size;
extern bool g_should_fail;
extern bool g_should_resize;

// Helper to ensure thread-safe access to mock variables
struct MockGuard {
  std::lock_guard<std::mutex> lock;
  MockGuard() : lock(g_mock_mutex) {}
};

} // namespace sep::test

// Mock bridge functions declarations
extern "C" {

__attribute__((weak)) int sep_process_context(const char *context_json,
                                              const char *layer,
                                              char *result_buffer,
                                              size_t buffer_size);

__attribute__((weak)) size_t sep_get_required_buffer_size();

__attribute__((weak)) int sep_bridge_get_last_error(char *error_buffer,
                                                    size_t buffer_size);

} // extern "C"

#endif // SEP_TEST_MOCK_BRIDGE_H
