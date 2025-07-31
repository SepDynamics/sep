#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

#include "quantum/bitspace/forward_window_result.h"

namespace sep::apps::cuda {

using ForwardWindowResult = sep::quantum::bitspace::ForwardWindowResult;

ForwardWindowResult simulateForwardWindowMetrics(const std::vector<uint8_t>& bits, size_t index_start);

}
