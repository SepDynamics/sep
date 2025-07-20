#pragma once
#include <memory>

namespace sep {
namespace pattern {
class BlenderBridge;
}

namespace blender {
std::unique_ptr<pattern::BlenderBridge> createBlenderBridge();
} // namespace blender
} // namespace sep
