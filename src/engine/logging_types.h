#pragma once
#include "engine/shim.h"

namespace sep::logging {

enum class Level {
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  CRITICAL
};

struct LoggerConfig {
    shim::string name;
    Level level{Level::INFO};
    struct ConsoleConfig
    {
        bool enabled{true};
    } console;
  struct FileConfig {
      shim::string path;
      std::size_t max_size{1048576};
      std::size_t max_files{3};
  } file;
  shim::string pattern;
};

} // namespace sep::logging
