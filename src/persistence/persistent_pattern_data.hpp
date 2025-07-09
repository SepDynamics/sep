#pragma once
#include <cstdint>

namespace sep {
namespace persistence {

struct PersistentPatternData {
    float coherence{0.0f};
    float stability{0.0f};
    std::uint32_t generation_count{0};
    float weight{1.0f};
};

} // namespace persistence
} // namespace sep
