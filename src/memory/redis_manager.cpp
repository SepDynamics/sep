#include "memory/redis_manager.h"
#include "memory/types.h"
#include "memory/manager.h" // For logging manager
#include <mutex>
// Define namespace alias to clarify that Manager is in the logging namespace
namespace logging = sep::logging;
#include <cstdint>
#if __has_include(<hiredis/hiredis.h>)
#    include <hiredis/hiredis.h>
#    define SEP_HAS_HIREDIS 1
#else
#    define SEP_HAS_HIREDIS 0
struct redisContext
{};
struct redisReply
{};
inline redisContext* redisConnect(const char*, int)
{
    return nullptr;
}
inline void  redisFree(redisContext*) {}
inline void  freeReplyObject(void*) {}
inline void* redisCommand(redisContext*, const char*, ...)
{
    return nullptr;
}
#endif
#include "memory/memory_tier_manager.hpp"
#include <memory>
#include <sstream>
#include <string>

namespace sep::persistence {


// RedisManager implementation
RedisManager::RedisManager(const std::string& host, int port) : impl_(std::make_unique<Impl>(host, port)) {}
RedisManager::~RedisManager() = default;
std::shared_ptr<IRedisManager> createRedisManager(const std::string& host, int port) { return std::make_shared<RedisManager>(host, port); }
void RedisManager::storePattern(std::uint64_t id, const sep::persistence::PersistentPatternData& data, const std::string& tier)
{
    impl_->storePattern(id, data, tier);
}

std::optional<persistence::PersistentPatternData> RedisManager::loadPattern(std::uint64_t id, const std::string& tier)
{
    return impl_->loadPattern(id, tier);
}

std::vector<std::uint64_t> RedisManager::getPatternIds(const std::string& tier)
{
    return impl_->getPatternIds(tier);
}

void RedisManager::removePattern(std::uint64_t id, const std::string& tier)
{
    impl_->removePattern(id, tier);
}

void RedisManager::bulkStore(const std::vector<std::pair<std::uint64_t, persistence::PersistentPatternData>>& patterns,
                             const std::string& tier)
{
    impl_->bulkStore(patterns, tier);
}

std::vector<persistence::PersistentPatternData> RedisManager::bulkLoad(const std::vector<std::uint64_t>& ids,
                                                                  const std::string& tier)
{
    return impl_->bulkLoad(ids, tier);
}

bool RedisManager::isConnected() const
{
    return impl_->isConnected();
}

}  // namespace sep::persistence
