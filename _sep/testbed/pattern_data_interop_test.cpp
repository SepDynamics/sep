#include "quantum/data.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <cstdio>

namespace fs = std::filesystem;

static std::string write_temp(const std::string &data) {
    char path[] = "/tmp/patternXXXXXX";
    int fd = mkstemp(path);
    if (fd == -1) {
        return "";
    }
    std::ofstream out(path);
    out << data;
    out.close();
    close(fd);
    return std::string(path);
}

static std::string run_cmd(const std::string &cmd) {
    std::string output;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return output;
    char buf[256];
    while (fgets(buf, sizeof(buf), pipe)) {
        output += buf;
    }
    pclose(pipe);
    return output;
}

static nlohmann::json to_json(const sep::pattern::PatternData &p) {
    nlohmann::json j;
    j["id"] = std::string(p.id.c_str());
    j["generation"] = p.generation;
    j["position"] = {p.position.x, p.position.y, p.position.z, p.position.w};
    j["coherence"] = p.coherence;
    j["stability"] = p.stability;
    j["entropy"] = p.entropy;
    j["mutation_rate"] = p.mutation_rate;
    if (!p.relationships.empty()) {
        j["relationships"] = nlohmann::json::array();
        for (const auto &rel : p.relationships) {
            nlohmann::json r;
            r["target"] = std::string(rel.targetId.c_str());
            r["strength"] = rel.strength;
            r["type"] = static_cast<int>(rel.type);
            j["relationships"].push_back(r);
        }
    }
    return j;
}

static sep::pattern::PatternData from_json(const nlohmann::json &j) {
    sep::pattern::PatternData p;
    if (j.contains("id")) p.id = j["id"].get<std::string>().c_str();
    p.generation = j.value("generation", 0);
    if (j.contains("position") && j["position"].is_array() && j["position"].size() >= 4) {
        p.position = {j["position"][0].get<float>(), j["position"][1].get<float>(),
                      j["position"][2].get<float>(), j["position"][3].get<float>()};
    }
    p.coherence = j.value("coherence", 0.0f);
    p.stability = j.value("stability", 0.0f);
    p.entropy = j.value("entropy", 0.0f);
    p.mutation_rate = j.value("mutation_rate", 0.0f);
    if (j.contains("relationships")) {
        for (const auto &rel : j["relationships"]) {
            sep::quantum::PatternRelationship r;
            if (rel.contains("target")) r.targetId = rel["target"].get<std::string>().c_str();
            r.strength = rel.value("strength", 0.0f);
            r.type = static_cast<sep::quantum::RelationshipType>(rel.value("type", 0));
            p.relationships.push_back(r);
        }
    }
    return p;
}

TEST(PatternDataInterop, CppToNodeRoundtrip) {
    sep::pattern::PatternData p{};
    p.id = "cpp-origin";
    p.generation = 1;
    p.position = {1.0f, 2.0f, 3.0f, 4.0f};
    p.coherence = 0.2f;
    p.stability = 0.3f;
    p.entropy = 0.4f;
    p.mutation_rate = 0.1f;
    sep::quantum::PatternRelationship rel{"node", 0.8f, sep::quantum::RelationshipType::ENTANGLEMENT};
    p.relationships.push_back(rel);

    auto j1 = to_json(p).dump();
    auto tmp = write_temp(j1);
    ASSERT_FALSE(tmp.empty());
    fs::path script = fs::path(__FILE__).parent_path() / "pattern_data_io.js";
    std::string cmd = std::string("node ") + script.string() + " " + tmp;
    auto out = run_cmd(cmd);
    std::remove(tmp.c_str());
    auto parsed = from_json(nlohmann::json::parse(out));
    EXPECT_EQ(j1, to_json(parsed).dump());
}

TEST(PatternDataInterop, NodeToCppRoundtrip) {
    fs::path script = fs::path(__FILE__).parent_path() / "pattern_data_io.js";
    auto gen = run_cmd(std::string("node ") + script.string() + " --generate");
    auto node_json = nlohmann::json::parse(gen);
    auto parsed = from_json(node_json);
    auto cpp_json = to_json(parsed).dump();
    auto tmp = write_temp(cpp_json);
    ASSERT_FALSE(tmp.empty());
    auto out = run_cmd(std::string("node ") + script.string() + " " + tmp);
    std::remove(tmp.c_str());
    auto out_json = nlohmann::json::parse(out);
    EXPECT_EQ(nlohmann::json::parse(cpp_json), out_json);
}

