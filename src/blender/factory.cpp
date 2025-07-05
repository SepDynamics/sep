#include <cstring>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <string>
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