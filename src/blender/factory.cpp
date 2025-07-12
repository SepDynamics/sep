#include "blender_pch.h"
#include <unistd.h>

#include "blender/factory.h"
#include "blender/bridge.h"
#include "blender/types.h"

namespace sep {
namespace blender {

std::unique_ptr<SEPBlenderBridge> createBlenderBridge() {
    auto bridge = std::make_unique<SEPBlenderBridge>();
    if (!bridge) {
        return nullptr;
    }
    bridge->impl = pattern::BlenderBridge::create();
    return bridge;
}

} // namespace blender
} // namespace sep