#pragma once
#include "engine/internal/standard_includes.h"

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
    std::string name;
    Level level{Level::INFO};
    struct ConsoleConfig
    {
        bool enabled{true};
    } console;
  struct FileConfig {
      std::string path;
      std::size_t max_size{1048576};
      std::size_t max_files{3};
  } file;
  std::string pattern;
};

} // namespace sep::logging
