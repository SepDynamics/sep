#pragma once

#include <string>

namespace sep::workbench {

enum class SignalType { BUY, SELL, HOLD };

struct SignalResult {
    SignalType type;
    float confidence;
    std::string reason;
};

} // namespace sep::workbench
