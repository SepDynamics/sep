#pragma once
#include <memory>

namespace sep {
namespace blender {

class BlenderBridge;

std::unique_ptr<BlenderBridge> createBlenderBridge();

} // namespace blender
} // namespace sep
