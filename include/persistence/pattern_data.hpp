#pragma once

namespace sep {
namespace persistence {

struct PersistentPatternData {
    float coherence{0.0f};
    float stability{0.0f};
    int generation_count{0};
};

} // namespace persistence
} // namespace sep
