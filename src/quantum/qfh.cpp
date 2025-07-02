#include "quantum/qfh.h"
#include <numeric>

namespace sep::quantum {

bool QFHEvent::operator==(const QFHEvent& other) const {
    return index == other.index && state == other.state &&
           bit_prev == other.bit_prev && bit_curr == other.bit_curr;
}

std::vector<QFHEvent> transform_rich(const std::vector<uint8_t>& bits) {
    std::vector<QFHEvent> result;
    if (bits.size() < 2) {
        return result;
    }
    result.reserve(bits.size() - 1);
    for (size_t i = 1; i < bits.size(); ++i) {
        uint8_t prev = bits[i - 1];
        uint8_t curr = bits[i];
        if ((prev != 0 && prev != 1) || (curr != 0 && curr != 1)) {
            return {};
        }
        if (prev == 0 && curr == 0) {
            result.push_back({i - 1, QFHState::NULL_STATE, prev, curr});
        } else if ((prev == 0 && curr == 1) || (prev == 1 && curr == 0)) {
            result.push_back({i - 1, QFHState::FLIP, prev, curr});
        } else if (prev == 1 && curr == 1) {
            result.push_back({i - 1, QFHState::RUPTURE, prev, curr});
        }
    }
    return result;
}

std::vector<QFHAggregateEvent> aggregate(const std::vector<QFHEvent>& events) {
    if (events.empty()) return {};
    std::vector<QFHAggregateEvent> aggregated;
    aggregated.push_back({events[0].index, events[0].state, 1});
    for (size_t i = 1; i < events.size(); ++i) {
        if (events[i].state == aggregated.back().state) {
            aggregated.back().count++;
        } else {
            aggregated.push_back({events[i].index, events[i].state, 1});
        }
    }
    return aggregated;
}

std::optional<QFHState> QFHProcessor::process(uint8_t current_bit) {
    if (current_bit != 0 && current_bit != 1) {
        return std::nullopt; 
    }
    if (!prev_bit.has_value()) {
        prev_bit = current_bit;
        return std::nullopt;
    }
    uint8_t prev = prev_bit.value();
    std::optional<QFHState> event_state;
    if (prev == 0 && current_bit == 0) {
        event_state = QFHState::NULL_STATE;
    } else if ((prev == 0 && current_bit == 1) || (prev == 1 && current_bit == 0)) {
        event_state = QFHState::FLIP;
    } else if (prev == 1 && current_bit == 1) {
        event_state = QFHState::RUPTURE;
    }
    prev_bit = current_bit;
    return event_state;
}

void QFHProcessor::reset() {
    prev_bit.reset();
}

// QFHBasedProcessor implementation
QFHBasedProcessor::QFHBasedProcessor(const QFHOptions& options) : options_(options) {}

QFHResult QFHBasedProcessor::analyze(const std::vector<uint8_t>& bits) { 
    QFHResult result;
    result.collapse_threshold = options_.collapse_threshold; 
    
    // Transform bits to events
    result.events = transform_rich(bits);
    
    // Aggregate events
    result.aggregated_events = aggregate(result.events);
    
    // Count event types
    for (const auto& event : result.events) {
        switch (event.state) {
            case QFHState::NULL_STATE:
                result.null_state_count++;
                break;
            case QFHState::FLIP:
                result.flip_count++;
                break;
            case QFHState::RUPTURE:
                result.rupture_count++;
                break;
            default:
                break;
        }
    }
    
    // Calculate ratios
    if (!result.events.empty()) {
        result.rupture_ratio = static_cast<float>(result.rupture_count) / 
                               static_cast<float>(result.events.size());
        result.flip_ratio = static_cast<float>(result.flip_count) / 
                            static_cast<float>(result.events.size());
    }
    
    // Detect collapse
    result.collapse_detected = (result.rupture_ratio >= options_.collapse_threshold);
    
    return result;
}

bool QFHBasedProcessor::detectCollapse(const QFHResult& result) const {
    return result.collapse_detected || result.rupture_ratio >= options_.collapse_threshold;
}

std::vector<uint8_t> QFHBasedProcessor::convertToBits(const std::vector<uint32_t>& values) {
    std::vector<uint8_t> bits;
    bits.reserve(values.size() * 32);
    
    for (uint32_t value : values) {
        for (int i = 0; i < 32; ++i) {
            bits.push_back((value >> i) & 1);
        }
    }
    
    return bits;
}

} // namespace sep::quantum
