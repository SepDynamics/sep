#pragma once

#include "session/output_driver.h"
#include "util/string.h"

namespace sep {
namespace blender {
namespace ccl {

// FileDriver: A concrete implementation of OutputDriver for file output
class FileDriver : public ::ccl::OutputDriver {
public:
    // Constructor that matches the parameters used in cycles_renderer.cpp
    FileDriver(const char* filepath, const char* pass);
    
    // Destructor
    ~FileDriver() override = default;
    
    // Implementation of the pure virtual function from OutputDriver
    void write_render_tile(const Tile& tile) override;
    
private:
    ::ccl::string filepath_;
    ::ccl::string pass_;
};

} // namespace ccl
} // namespace blender
} // namespace sep