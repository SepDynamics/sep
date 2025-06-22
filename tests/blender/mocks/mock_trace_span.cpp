/**
 * @file mock_trace_span.cpp
 * @brief Mock implementation of TraceSpan for testing
 */

// Disable reserved identifier warnings for mangled names
#pragma clang diagnostic push
#pragma clang diagnostic ignored 

#include "metrics/tracing.h"
#include <iostream>
#include <string>
#include <chrono>

// Forward declaration of the class to use in extern "C" block
namespace sep::metrics {
    class TraceSpan;
}

// Provide the exact mangled names that the linker is looking for
extern "C" {
    // Constructor with std::string
    void* _ZN3sep7metrics9TraceSpanC1ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE(
        void* this_ptr,
        std::string* name_ptr) {
        
        std::cout << "Mock TraceSpan constructor called with name: " << *name_ptr << '\n';
        
        // Initialize the object's memory
        new (this_ptr) sep::metrics::TraceSpan(*name_ptr);
        
        return this_ptr;
    }

    // Destructor
    void _ZN3sep7metrics9TraceSpanD1Ev(void* this_ptr) {
        std::cout << "Mock TraceSpan destructor called" << '\n';
        
        // Call the destructor
        reinterpret_cast<sep::metrics::TraceSpan*>(this_ptr)->~TraceSpan();
    }
}

namespace sep::metrics {

// Regular implementation for the compiler
TraceSpan::TraceSpan(const std::string& name)
    : name_(name), start_(std::chrono::high_resolution_clock::now()) {
  std::cout << "Regular TraceSpan constructor called with name: " << name << '\n';
}

sep::metrics::TraceSpan::~TraceSpan() {
  std::cout << "Regular TraceSpan destructor called" << '\n';
}

void TraceSpan::setAttribute(const std::string& key, std::int64_t value) {
  std::cout << "TraceSpan setAttribute called with key: " << key << " and value: " << value << '\n';
  (void)key;
  (void)value;
}

}  // namespace sep::metrics

// Restore diagnostics
#pragma clang diagnostic pop