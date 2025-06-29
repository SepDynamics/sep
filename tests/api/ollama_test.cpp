#include <gtest/gtest.h>
#include "api/ollama_client.h"
#include <fstream>
// shim.h not required for this standalone test

using namespace sep::api;

TEST(OllamaClientTest, FileGet) {
    const char* path = "ollama_response.json";
    std::ofstream out(path);
    out << "{\"response\":\"ok\"}";
    out.close();

    sep::ollama::OllamaConfig cfg;
    cfg.host = std::string("file://") + std::string("./");
    OllamaClient client(cfg);
    std::string result;
    EXPECT_EQ(client.get(path, result), sep::SEPResult::SUCCESS);
    EXPECT_NE(result.find("ok"), std::string::npos);
    std::remove(path);
}
