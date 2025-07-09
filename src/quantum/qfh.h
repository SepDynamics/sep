#ifndef SEP_QUANTUM_QFH_H
#define SEP_QUANTUM_QFH_H

#include <cstdint>
#include <cstddef>  // For ptrdiff_t, max_align_t
#include "compat/shim.h"
#include <vector>
#include <optional>
#include <algorithm> // For std::sort, std::unique

namespace sep::quantum {

// Enumeration of transition states.
enum class QFHState : uint8_t {
    NULL_STATE = 0,
    FLIP = 1,
    RUPTURE = 2
};

// Event representation capturing index and bit context.
struct QFHEvent {
    std::size_t index;   // index of previous bit in the pair
    QFHState state;      // transition type
    uint8_t bit_prev;    // previous bit value
    uint8_t bit_curr;    // current bit value

    bool operator==(const QFHEvent& other) const;
};

// Aggregated event including run length.
struct QFHAggregateEvent {
    std::size_t index;
    QFHState state;
    int count;
};

// Transform an entire bit vector into transition events.
std::vector<QFHEvent> transform_rich(const std::vector<uint8_t>& bits);

// Aggregate events by state
std::vector<QFHAggregateEvent> aggregate(const std::vector<QFHEvent>& events);

// Streaming processor for online transformation.
class QFHProcessor {
private:
    std::optional<uint8_t> prev_bit;
public:
    QFHProcessor() = default;

    std::optional<QFHState> process(uint8_t current_bit);
    void reset();
};

// QFH analysis result
struct QFHResult {
    std::vector<QFHEvent> events;
    std::vector<QFHAggregateEvent> aggregated_events;
    int null_state_count{0};
    int flip_count{0};
    int rupture_count{0};
    float rupture_ratio{0.0f};
    float flip_ratio{0.0f};
    float collapse_threshold{0.0f};
    bool collapse_detected{false};
};

// QFH analysis options
struct QFHOptions {
    float collapse_threshold{0.3f};  // Ratio of ruptures that indicates collapse
    float flip_threshold{0.7f};      // Ratio of flips that indicates instability
};

// QFH-based QBSA processor
class QFHBasedProcessor {
public:
    explicit QFHBasedProcessor(const QFHOptions& options = {});

    // Analyze bit pattern
    QFHResult analyze(const std::vector<uint8_t>& bits);

    // Detect collapse based on rupture ratio
    bool detectCollapse(const QFHResult& result) const;

    // Convert uint32_t vector to bit vector
    static std::vector<uint8_t> convertToBits(const std::vector<uint32_t>& values);

private:
    QFHOptions options_;
};

} // namespace sep::quantum

#endif // SEP_QUANTUM_QFH_H
