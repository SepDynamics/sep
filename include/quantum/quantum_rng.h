#ifndef SEP_QUANTUM_QUANTUM_RNG_H
#define SEP_QUANTUM_QUANTUM_RNG_H

#include <cstdint>
#include <cmath>

namespace sep::quantum {

class QuantumRNG {
public:
    explicit QuantumRNG(uint32_t seed = 0x12345678u) : state_(seed) {}

    float uniform() {
        state_ ^= state_ << 13;
        state_ ^= state_ >> 17;
        state_ ^= state_ << 5;
        return static_cast<float>(state_ & 0xFFFFFFu) / static_cast<float>(0xFFFFFFu);
    }

    uint32_t uniform(uint32_t max) {
        return static_cast<uint32_t>(uniform() * static_cast<float>(max));
    }

    float gaussian(float mean, float stddev) {
        float u1 = uniform();
        float u2 = uniform();
        float z = std::sqrt(-2.0f * std::log(u1 + 1e-6f)) * std::cos(2.0f * M_PI * u2);
        return mean + z * stddev;
    }

private:
    uint32_t state_;
};

} // namespace sep::quantum

#endif // SEP_QUANTUM_QUANTUM_RNG_H
