#include <cstring> // For memcpy, memset, memcmp, strlen, etc.
#include <ctime>   // For time-related functions
#include <string>  // For std::string
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