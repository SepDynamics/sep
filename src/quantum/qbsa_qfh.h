#ifndef SEP_QUANTUM_QBSA_QFH_H
#define SEP_QUANTUM_QBSA_QFH_H

#include <memory>

#include "quantum/bitspace/qbsa.h"

namespace sep::quantum {

// Factory function to create a QFH-based QBSA processor
std::unique_ptr<QBSAProcessor> createQFHBasedQBSAProcessor(
    const QBSAOptions& options);

} // namespace sep::quantum

#endif // SEP_QUANTUM_QBSA_QFH_H
