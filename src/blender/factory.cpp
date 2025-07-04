#include <cstring> // Added for C string functions
#include <ctime>   // Added for C time functions
#include <string> // Required for std::string

using namespace std; // For string type
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