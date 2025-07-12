#pragma once
#include <memory>
#include "blender/types.h"

namespace sep {
namespace pattern {
class BlenderBridge;
}

struct SEPBlenderBridge;

namespace blender {
std::unique_ptr<SEPBlenderBridge> createBlenderBridge();
} // namespace blender
} // namespace sep
