#pragma once

#include <glm/glm.hpp>

namespace sep {
namespace compat {

struct PatternData {
    glm::vec4 attributes;
    float coherence;
    float stability;
};

}
}