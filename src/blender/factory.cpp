#include <cstring>
#include <ctime>
#include <cstdlib>
#include <string>
#include <memory>
#include <string.h>
#include <time.h>
#include <unistd.h>

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