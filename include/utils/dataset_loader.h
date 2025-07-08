#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace sep::utils {

std::vector<glm::vec3> loadDataset(const std::string &path);

std::vector<glm::vec3> parseJsonDataset(const nlohmann::json &j);

}
