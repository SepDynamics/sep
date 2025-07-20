#pragma once

#include <glm/glm.hpp>

namespace sep {
namespace compat {

struct PatternData {
    glm::vec4 attributes;
    float coherence;
    float stability;

    bool empty() const { return false; }
    size_t size() const { return 0; }
    float& operator[](size_t) { static float dummy = 0; return dummy; }
    const float& operator[](size_t) const { static float dummy = 0; return dummy; }
    void resize(size_t) {}
};

}
}