#pragma once
#include <string>

namespace sep {
namespace blender {
class CyclesRenderer {
public:
    CyclesRenderer() = default;
    bool render(const std::string& filepath);
};
} // namespace blender
} // namespace sep
