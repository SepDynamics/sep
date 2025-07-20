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
                              const shim::string& tier) = 0;
    virtual std::optional<persistence::PersistentPatternData> loadPattern(
        std::uint64_t id, const shim::string& tier) = 0;
    virtual shim::vector<std::uint64_t> getPatternIds(const shim::string& tier) = 0;
    virtual void removePattern(std::uint64_t id, const shim::string& tier) = 0;
    virtual void bulkStore(
        const shim::vector<std::pair<std::uint64_t, persistence::PersistentPatternData>>& patterns,
        const shim::string& tier) = 0;
    virtual shim::vector<persistence::PersistentPatternData> bulkLoad(
        const shim::vector<std::uint64_t>& ids, const shim::string& tier) = 0;
    virtual bool isConnected() const = 0;
};

class RedisManager : public IRedisManager {
public:
    RedisManager(const shim::string& host, int port);
    ~RedisManager() override;

    void storePattern(std::uint64_t id, const persistence::PersistentPatternData& data,
                      const shim::string& tier) override;
    std::optional<persistence::PersistentPatternData> loadPattern(
        std::uint64_t id, const shim::string& tier) override;
    shim::vector<std::uint64_t> getPatternIds(const shim::string& tier) override;
    void removePattern(std::uint64_t id, const shim::string& tier) override;
    void bulkStore(
        const shim::vector<std::pair<std::uint64_t, persistence::PersistentPatternData>>& patterns,
        const shim::string& tier) override;
    shim::vector<persistence::PersistentPatternData> bulkLoad(
        const shim::vector<std::uint64_t>& ids, const shim::string& tier) override;
    bool isConnected() const override;

private:
    class Impl {
    public:
        Impl(const shim::string& host, int port);
        ~Impl();
        
        bool isConnected() const;
        void storePattern(std::uint64_t id, const persistence::PersistentPatternData& data,
                          const shim::string& tier);
        std::optional<persistence::PersistentPatternData> loadPattern(std::uint64_t id,
                                                                      const shim::string& tier);
        shim::vector<std::uint64_t> getPatternIds(const shim::string& tier);
        void removePattern(std::uint64_t id, const shim::string& tier);
        void bulkStore(const shim::vector<
                           std::pair<std::uint64_t, persistence::PersistentPatternData>>& patterns,
                       const shim::string& tier);
        shim::vector<persistence::PersistentPatternData> bulkLoad(
            const shim::vector<std::uint64_t>& ids, const shim::string& tier);

    private:
        shim::string getPatternKey(std::uint64_t id, const shim::string& tier) const;
        shim::string getTierPatternsKey(const shim::string& tier) const;
        shim::string normalizeTier(const shim::string& tier) const;

        ::redisContext* context_;
        bool connected_;
        std::mutex mutex_;
    };

    std::unique_ptr<Impl> impl_;
};

// Factory function to create RedisManager instances
std::shared_ptr<IRedisManager> createRedisManager(const shim::string& host = "localhost",
                                                  int port = 6379);

} // namespace sep::persistence
