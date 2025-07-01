#include "blender/factory.h"
#include "blender/bridge.h"
#include "blender/types.h"

namespace sep {
namespace blender {

std::unique_ptr<BlenderBridge> createBlenderBridge() {
    return BlenderBridge::create();
}

} // namespace blender
} // namespace sep