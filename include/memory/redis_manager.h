#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace sep::persistence {
struct PatternData;

class IRedisManager {
public:
    virtual ~IRedisManager() = default;
    virtual void storePattern(std::uint64_t id, const PatternData& data, const std::string& tier) = 0;
    virtual std::optional<PatternData> loadPattern(std::uint64_t id, const std::string& tier) = 0;
    virtual std::vector<std::uint64_t> getPatternIds(const std::string& tier) = 0;
    virtual void removePattern(std::uint64_t id, const std::string& tier) = 0;
    virtual void bulkStore(const std::vector<std::pair<std::uint64_t, PatternData>>& patterns, const std::string& tier) = 0;
    virtual std::vector<PatternData> bulkLoad(const std::vector<std::uint64_t>& ids, const std::string& tier) = 0;
    virtual bool isConnected() const = 0;
};

std::shared_ptr<IRedisManager> createRedisManager(const std::string& host = "localhost", int port = 6379);
}
