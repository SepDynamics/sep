#pragma once

#include <cmath>
#include <array>

namespace cycles {

class Transform {
public:
  Transform() = default;
  virtual ~Transform() = default;

  // Basic transform operations
  virtual void translate(float x, float y, float z) = 0;
  virtual void rotate(float angle, float x, float y, float z) = 0;
  virtual void scale(float x, float y, float z) = 0;

  // Matrix operations
  virtual void setMatrix(const std::array<float, 16>& matrix) = 0;
  virtual std::array<float, 16> getMatrix() const = 0;
};

} // namespace cycles