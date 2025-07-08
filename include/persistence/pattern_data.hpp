#pragma once

// This header historically duplicated the PersistentPatternData definition
// from the memory subsystem.  That caused multiple type definitions when both
// headers were included.  To keep a single canonical struct we now simply
// re-export the existing definition from persistent_pattern_data.hpp.

#include "persistent_pattern_data.hpp"

namespace sep {
namespace persistence {

// Alias maintained for backward compatibility with code that included this
// header.  All users should migrate to persistent_pattern_data.hpp directly.
using PersistentPatternData = ::sep::persistence::PersistentPatternData;

} // namespace persistence
} // namespace sep
