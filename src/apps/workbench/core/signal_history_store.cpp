#include "signal_history_store.h"

SignalHistoryStore::SignalHistoryStore() {}

void SignalHistoryStore::add_signal(const SEPSignal& signal) {
    std::lock_guard<std::mutex> lock(mutex_);
    signals_.push_back(signal);
}

std::vector<SEPSignal> SignalHistoryStore::get_signals() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return signals_;
}
