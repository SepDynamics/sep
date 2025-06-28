#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "memory/types.h"

namespace sep::persistence {

// Interface for Redis pattern persistence
class IRedisManager {
public:
    virtual ~IRedisManager() = default;
    virtual void storePattern(std::uint64_t id, const persistence::PatternData& data, const std::string& tier) = 0;
    virtual std::optional<persistence::PatternData> loadPattern(std::uint64_t id, const std::string& tier) = 0;
    virtual std::vector<std::uint64_t> getPatternIds(const std::string& tier) = 0;
    virtual void removePattern(std::uint64_t id, const std::string& tier) = 0;
    virtual void bulkStore(const std::vector<std::pair<std::uint64_t, persistence::PatternData>>& patterns, const std::string& tier) = 0;
    virtual std::vector<persistence::PatternData> bulkLoad(const std::vector<std::uint64_t>& ids, const std::string& tier) = 0;
    virtual bool isConnected() const = 0;
};

// Forward declare concrete implementation
class RedisManager;

// Factory function to create RedisManager instances
std::shared_ptr<IRedisManager> createRedisManager(const std::string& host = "localhost", int port = 6379);

} // namespace sep::persistence
