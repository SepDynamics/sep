#include "utils/dataset_loader.h"

#include <fstream>
#include <stdexcept>

namespace sep::utils {

std::vector<glm::vec3> parseJsonDataset(const nlohmann::json &jsonData) {
    std::vector<glm::vec3> patterns;
    auto processItem = [&](const nlohmann::json &item) {
        float x = 0.0f, y = 0.0f, z = 0.0f;
        if (item.is_array() && item.size() >= 3) {
            x = item[0].get<float>();
            y = item[1].get<float>();
            z = item[2].get<float>();
        } else if (item.is_object()) {
            if (item.contains("x") && item.contains("y") && item.contains("z")) {
                x = item["x"].get<float>();
                y = item["y"].get<float>();
                z = item["z"].get<float>();
            } else if (item.contains("vector") && item["vector"].is_array() && item["vector"].size() >= 3) {
                x = item["vector"][0].get<float>();
                y = item["vector"][1].get<float>();
                z = item["vector"][2].get<float>();
            } else if (item.contains("embedding") && item["embedding"].is_array() && item["embedding"].size() >= 3) {
                x = item["embedding"][0].get<float>();
                y = item["embedding"][1].get<float>();
                z = item["embedding"][2].get<float>();
            }
        }
        patterns.emplace_back(x, y, z);
    };

    if (jsonData.is_array()) {
        for (const auto &item : jsonData) {
            processItem(item);
        }
    } else if (jsonData.is_object()) {
        if (jsonData.contains("patterns")) {
            auto more = parseJsonDataset(jsonData["patterns"]);
            patterns.insert(patterns.end(), more.begin(), more.end());
        } else if (jsonData.contains("data")) {
            auto more = parseJsonDataset(jsonData["data"]);
            patterns.insert(patterns.end(), more.begin(), more.end());
        } else if (jsonData.contains("embeddings")) {
            auto more = parseJsonDataset(jsonData["embeddings"]);
            patterns.insert(patterns.end(), more.begin(), more.end());
        }
    }
    return patterns;
}

static std::vector<glm::vec3> loadJsonFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open dataset");
    }
    nlohmann::json j;
    file >> j;
    return parseJsonDataset(j);
}

static std::vector<glm::vec3> loadBinaryFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open dataset");
    }
    file.seekg(0, std::ios::end);
    std::size_t sz = file.tellg();
    file.seekg(0, std::ios::beg);
    std::size_t count = sz / (sizeof(float) * 3);
    std::vector<glm::vec3> data(count);
    file.read(reinterpret_cast<char *>(data.data()), count * sizeof(glm::vec3));
    return data;
}

std::vector<glm::vec3> loadDataset(const std::string &path) {
    if (path.size() >= 4 && path.substr(path.size() - 4) == ".bin") {
        return loadBinaryFile(path);
    }
    return loadJsonFile(path);
}

} // namespace sep::utils
