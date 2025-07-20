#pragma once

#include <hiredis/hiredis.h>

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "memory/persistent_pattern_data.hpp"
#include "memory/types.h"

namespace sep::persistence {

// Interface for Redis pattern persistence
class IRedisManager {
public:
    virtual ~IRedisManager() = default;
    virtual void storePattern(std::uint64_t id, const persistence::PersistentPatternData& data,
                              const std::string& tier) = 0;
    virtual std::optional<persistence::PersistentPatternData> loadPattern(
        std::uint64_t id, const std::string& tier) = 0;
    virtual std::vector<std::uint64_t> getPatternIds(const std::string& tier) = 0;
    virtual void removePattern(std::uint64_t id, const std::string& tier) = 0;
    virtual void bulkStore(
        const std::vector<std::pair<std::uint64_t, persistence::PersistentPatternData>>& patterns,
        const std::string& tier) = 0;
    virtual std::vector<persistence::PersistentPatternData> bulkLoad(
        const std::vector<std::uint64_t>& ids, const std::string& tier) = 0;
    virtual bool isConnected() const = 0;
};

class RedisManager : public IRedisManager {
public:
    RedisManager(const std::string& host, int port);
    ~RedisManager() override;

    void storePattern(std::uint64_t id, const persistence::PersistentPatternData& data,
                      const std::string& tier) override;
    std::optional<persistence::PersistentPatternData> loadPattern(std::uint64_t id,
                                                                  const std::string& tier) override;
    std::vector<std::uint64_t> getPatternIds(const std::string& tier) override;
    void removePattern(std::uint64_t id, const std::string& tier) override;
    void bulkStore(
        const std::vector<std::pair<std::uint64_t, persistence::PersistentPatternData>>& patterns,
        const std::string& tier) override;
    std::vector<persistence::PersistentPatternData> bulkLoad(const std::vector<std::uint64_t>& ids,
                                                             const std::string& tier) override;
    bool isConnected() const override;

private:
    class Impl {
    public:
        Impl(const std::string& host, int port);
        ~Impl();
        
        bool isConnected() const;
        void storePattern(std::uint64_t id, const persistence::PersistentPatternData& data,
                          const std::string& tier);
        std::optional<persistence::PersistentPatternData> loadPattern(std::uint64_t id,
                                                                      const std::string& tier);
        std::vector<std::uint64_t> getPatternIds(const std::string& tier);
        void removePattern(std::uint64_t id, const std::string& tier);
        void bulkStore(const std::vector<
                           std::pair<std::uint64_t, persistence::PersistentPatternData>>& patterns,
                       const std::string& tier);
        std::vector<persistence::PersistentPatternData> bulkLoad(
            const std::vector<std::uint64_t>& ids, const std::string& tier);

    private:
        std::string getPatternKey(std::uint64_t id, const std::string& tier) const;
        std::string getTierPatternsKey(const std::string& tier) const;
        std::string normalizeTier(const std::string& tier) const;

        ::redisContext* context_;
        bool connected_;
        std::mutex mutex_;
    };

    std::unique_ptr<Impl> impl_;
};

// Factory function to create RedisManager instances
std::shared_ptr<IRedisManager> createRedisManager(const std::string& host = "localhost",
                                                  int port = 6379);

} // namespace sep::persistence
