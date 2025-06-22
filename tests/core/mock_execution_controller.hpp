#ifndef SEP_CORE_MOCK_EXECUTION_CONTROLLER_HPP
#define SEP_CORE_MOCK_EXECUTION_CONTROLLER_HPP

#include "core/execution_controller.h"

namespace sep::core {

class MockExecutionController : public ExecutionController {
private:
    int low_threshold_{0};
    int medium_threshold_{50};
    int high_threshold_{80};
    bool optimization_attempted_{false};

public:
    void setPriorityThresholds(int low, int medium, int high) override {
        low_threshold_ = low;
        medium_threshold_ = medium;
        high_threshold_ = high;
        optimization_attempted_ = true;
    }

    bool wasOptimizationAttempted() const override {
        return optimization_attempted_;
    }

    // Helper methods for testing
    int getLowThreshold() const { return low_threshold_; }
    int getMediumThreshold() const { return medium_threshold_; }
    int getHighThreshold() const { return high_threshold_; }
};

} // namespace sep::core

#endif // SEP_CORE_MOCK_EXECUTION_CONTROLLER_HPP
