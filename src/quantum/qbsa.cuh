#pragma once

#pragma once
#include <cstdint>

namespace sep::quantum {

struct QBSAParams {
    const uint32_t* probe_indices;
    const uint32_t* expectations;
    uint32_t* corrections;
    uint32_t num_probes;
};

bool launch_qbsa_kernel(const QBSAParams& params);

} // namespace sep::quantum