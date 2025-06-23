#include "blender/gpu_context.h"

namespace sep {

// This translation unit intentionally remains empty. All methods of GPUContext
// and its helper types are defined inline in the header to allow the minimal
// stub implementation to be header-only. Keeping this file ensures that the
// build system can still reference a source for the component without
// generating duplicate symbol definitions when the header is included in
// multiple translation units.

}  // namespace sep
