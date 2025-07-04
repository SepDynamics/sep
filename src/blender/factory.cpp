#include <cstring> // For std::memcpy, std::memset, std::memcmp, std::strlen etc.
#include <ctime>   // For C-style time functions
#include <string>  // For std::string
#include <memory>
#include "blender/factory.h"
#include "blender/bridge.h"
#include "blender/types.h"

namespace sep {
namespace blender {

std::unique_ptr<pattern::BlenderBridge> createBlenderBridge() {
    return pattern::BlenderBridge::create();
}

} // namespace blender
} // namespace sep