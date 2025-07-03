#pragma once

#include <functional>
#include "session/output_driver.h"
#include "util/string.h"

CCL_NAMESPACE_BEGIN

class OIIOOutputDriver : public OutputDriver {
 public:
  using LogFunction = std::function<void(const string &)>;

  OIIOOutputDriver(const string_view filepath, const string_view pass, LogFunction log);
  ~OIIOOutputDriver() override;

  void write_render_tile(const Tile &tile) override;

 protected:
  string filepath_;
  string pass_;
  LogFunction log_;
};

CCL_NAMESPACE_END
