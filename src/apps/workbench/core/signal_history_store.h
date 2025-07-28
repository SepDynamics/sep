#pragma once

#include "sep_signal_generator.h"
#include <vector>
#include <mutex>

class SignalHistoryStore {
public:
    SignalHistoryStore();

    void add_signal(const SEPSignal& signal);
    std::vector<SEPSignal> get_signals() const;

private:
    mutable std::mutex mutex_;
    std::vector<SEPSignal> signals_;
};
