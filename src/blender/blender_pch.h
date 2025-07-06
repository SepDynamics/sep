#pragma once

// 1. Core C/C++ Standard Libraries
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <stdexcept>
#include <array>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

// 2. Heavy Third-Party Libraries
#include <OpenImageIO/imageio.h>
#include <OpenColorIO/OpenColorIO.h>
#include <spdlog/spdlog.h>
#include <lz4.h>
#include <zstd.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// 3. Cycles Headers
#include "util/system.h"
#include "util/types.h"
#include "util/math.h"
#include "scene/scene.h"

// 4. Your Own Project's Core Headers
#include "core/common.h"
#include "quantum/types.h"
#include "memory/types.h"
#include "compat/shim.h"
