#pragma once
#include <string>
#include <filesystem>

namespace sep::workbench {

class FileDialog {
public:
    void open(const std::string& start_dir = ".");
    bool render(std::string& selected_path);
    bool isOpen() const { return open_; }
private:
    bool open_{false};
    std::filesystem::path current_dir_{};
    std::string selection_{};
};

} // namespace sep::workbench
