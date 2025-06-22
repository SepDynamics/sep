#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#ifndef SCENE_SOURCE_DIR
#define SCENE_SOURCE_DIR "scenes"
#endif

TEST(SceneLoadTest, AllScenesParse) {
    namespace fs = std::filesystem;
    fs::path scene_dir = fs::path(SCENE_SOURCE_DIR);
    size_t count = 0;
    for (const auto& entry : fs::directory_iterator(scene_dir)) {
        if (entry.path().extension() != ".json") continue;
        std::ifstream in(entry.path());
        nlohmann::json data;
        ASSERT_NO_THROW(in >> data);
        EXPECT_TRUE(data.contains("objects"));
        ++count;
    }
    EXPECT_GT(count, 0u);
}
