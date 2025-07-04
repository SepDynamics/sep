#include <string.h> // For memcpy, memset, memcmp, strlen, etc.
#include <time.h>   // For time-related functions
#include <cstring>
#include <ctime>
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