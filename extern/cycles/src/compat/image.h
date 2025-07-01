#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../util/image.h"

namespace cycles {

class VDBImage {
public:
  VDBImage() = default;
  virtual ~VDBImage() = default;

  // VDB-specific image properties
  virtual bool isVDB() const { return true; }
  virtual float voxelSize() const = 0;
  virtual bool hasGrid(const std::string& name) const = 0;
  
  // Basic image interface
  virtual int width() const = 0;
  virtual int height() const = 0;
  virtual int depth() const = 0;
  virtual int channels() const = 0;
  virtual const uint8_t* data() const = 0;
  virtual size_t size() const = 0;

  // Grid access
  virtual bool loadGrid(const std::string& name) = 0;
  virtual void* getGrid(const std::string& name) = 0;
};

using VDBImagePtr = std::shared_ptr<VDBImage>;

} // namespace cycles