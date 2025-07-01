#pragma once

// Standard includes first
#include <csignal>
#include <string>
#include <vector>

// Cycles namespace compatibility macros
#ifndef CCL_NAMESPACE_BEGIN
#define CCL_NAMESPACE_BEGIN \
  namespace ccl { \
  using namespace std;
#endif

#ifndef CCL_NAMESPACE_END
#define CCL_NAMESPACE_END }
#endif

// Include Cycles headers after namespace definitions
#include "util/defines.h"
#include "util/types.h"
#include "scene/scene.h"
#include "session/session.h"

// Pull specific symbols into ccl namespace
CCL_NAMESPACE_BEGIN
using ::std::signal;
using ::std::raise;
using ::std::sig_atomic_t;
CCL_NAMESPACE_END