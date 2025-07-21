#include "file_dialog.hpp"
#include <imgui.h>
#include <vector>
#include <algorithm>

namespace sep::workbench {

void FileDialog::open(const std::string& start_dir) {
    current_dir_ = start_dir.empty() ? std::filesystem::current_path() : std::filesystem::path(start_dir);
    open_ = true;
    selection_.clear();
}

bool FileDialog::render(std::string& selected_path) {
    if (!open_) return false;
    bool chosen = false;
    ImGui::OpenPopup("File Browser");
    if (ImGui::BeginPopupModal("File Browser", &open_, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextUnformatted(current_dir_.string().c_str());
        ImGui::Separator();

        if (current_dir_ != current_dir_.root_path()) {
            if (ImGui::Selectable("..", false)) {
                current_dir_ = current_dir_.parent_path();
            }
        }

        std::vector<std::filesystem::directory_entry> entries;
        for (const auto& entry : std::filesystem::directory_iterator(current_dir_)) {
            entries.push_back(entry);
        }
        std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b){
            if (a.is_directory() != b.is_directory()) return a.is_directory() && !b.is_directory();
            return a.path().filename() < b.path().filename();
        });
        for (const auto& entry : entries) {
            const auto filename = entry.path().filename().string();
            if (entry.is_directory()) {
                if (ImGui::Selectable((filename + "/").c_str(), false)) {
                    current_dir_ /= entry.path().filename();
                }
            } else {
                bool selected = (selection_ == entry.path().string());
                if (ImGui::Selectable(filename.c_str(), selected)) {
                    selection_ = entry.path().string();
                }
            }
        }
        ImGui::Separator();
        if (ImGui::Button("Select") && !selection_.empty()) {
            selected_path = selection_;
            chosen = true;
            open_ = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            open_ = false;
        }
        ImGui::EndPopup();
    }
    return chosen;
}

} // namespace sep::workbench
