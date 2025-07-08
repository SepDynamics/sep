#pragma once

// Legacy wrapper that aliases the canonical PersistentPatternData definition.
// Keeping this header ensures older includes continue to compile while
// avoiding duplicate struct definitions across namespaces.

#include "persistent_pattern_data.hpp"

namespace sep {
namespace memory { namespace persistence {
using ::sep::persistence::PersistentPatternData;
} } // namespace memory::persistence

namespace persistence {
using PersistentPatternData = ::sep::persistence::PersistentPatternData;
} // namespace persistence
} // namespace sep
