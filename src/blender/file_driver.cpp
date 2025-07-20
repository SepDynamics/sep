#include "file_driver.h"
#include <iostream>

namespace sep {
namespace blender {
namespace ccl {

FileDriver::FileDriver(const char* filepath, const char* pass)
    : filepath_(filepath), pass_(pass)
{
    // Log that the driver was created
    std::cout << "FileDriver created for file: " << filepath_ << ", pass: " << pass_ << std::endl;
}

void FileDriver::write_render_tile(const Tile& tile)
{
    // This is a stub implementation that just logs the call
    // In a real implementation, this would write the tile data to a file
    
    std::cout << "FileDriver::write_render_tile called for file: " << filepath_ 
              << ", pass: " << pass_
              << ", tile size: " << tile.size.x << "x" << tile.size.y
              << ", at offset: " << tile.offset.x << "," << tile.offset.y
              << std::endl;
              
    // For now, we're just acknowledging the call without actually writing anything
    // This is enough to satisfy the abstract class requirement
}

} // namespace ccl
} // namespace blender
} // namespace sep