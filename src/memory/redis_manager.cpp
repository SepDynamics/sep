#include "memory/types.h"
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
#include "memory/manager.h"
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

namespace sep::persistence {

class RedisManager::Impl
{
public:
    Impl(const std::string& host, int port) : context_(nullptr), connected_(false)
    {
         auto logger =
            sep::logging::Manager::getInstance().getLogger("redis");
#if SEP_HAS_HIREDIS
        // Initialize Redis connection
        context_ = redisConnect(host.c_str(), port);
        if (context_ == nullptr || context_->err)
        {
            if (logger)
            {
                if (context_)
                {
                    logger->error("Redis connection error: {}", context_->errstr);
                }
                else
                {
                    logger->error("Redis connection error: cannot allocate redis context");
                }
            }
        }
        else
        {
            connected_ = true;
            if (logger)
            {
                logger->info("Redis connection established");
            }
        }
#endif
    }

    ~Impl()
    {
#if SEP_HAS_HIREDIS
        if (context_)
        {
            redisFree(context_);
            context_ = nullptr;
        }
#endif
    }

    bool isConnected() const
    {
        return connected_;
    }

    void storePattern( std::size_t id,
                       const sep::pattern::PatternData& data,
                       const std::string& tier)
    {
#if SEP_HAS_HIREDIS
        if (!connected_ || !context_)
            return;

        auto logger = sep::logging::Manager::getInstance().getLogger("redis");
        std::lock_guard<std::mutex> lock(mutex_);

        std::stringstream key;
        key << "pattern:" << tier << ":" << id;

        // Clear existing pattern data
        redisReply* reply = static_cast<redisReply*>(redisCommand(context_, "DEL %s", key.str().c_str()));
        if (reply)
            freeReplyObject(reply);

        // Store pattern position
        // cast position components to double to match redisCommand formatting
        reply = static_cast<redisReply*>(redisCommand(context_,
                                                      "HSET %s position:x %f position:y %f position:z %f",
                                                      key.str().c_str(),
                                                      static_cast<double>(data.position.x),
                                                      static_cast<double>(data.position.y),
                                                      static_cast<double>(data.position.z)));
        if (reply)
            freeReplyObject(reply);

        // Store pattern metadata
        reply = static_cast<redisReply*>(redisCommand(
            context_,
            "HSET %s coherence %f stability %f generation_count %d access_frequency %f timestamp %lld",
            key.str().c_str(),
            static_cast<double>(data.coherence),
            static_cast<double>(data.stability),
            data.generation,
            static_cast<double>(data.coherence), // Using coherence instead of access_frequency
            static_cast<long long>(
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())));
        if (reply)
            freeReplyObject(reply);

        // Store enhanced relationship data
        for (size_t i = 0; i < data.relationships.size(); ++i)
        {
            const auto& rel = data.relationships[i];
            // Convert targetId to size_t ID by using a hash
            size_t rel_id = std::hash<std::string>{}(rel.targetId.c_str());
            
            reply = static_cast<redisReply*>(redisCommand(context_,
                                                          "HSET %s rel:%zu:id %zu rel:%zu:type %d rel:%zu:strength %f",
                                                          key.str().c_str(),
                                                          i,
                                                          rel_id,
                                                          i,
                                                          static_cast<int>(rel.type),
                                                          i,
                                                          static_cast<double>(rel.strength)));
            if (reply)
                freeReplyObject(reply);
        }

        // Store relationship count
        reply = static_cast<redisReply*>(
            redisCommand(context_, "HSET %s rel_count %zu", key.str().c_str(), data.relationships.size()));
        if (reply)
            freeReplyObject(reply);

        // Add to tier index
        reply = static_cast<redisReply*>(redisCommand(context_, "SADD %s:patterns %zu", tier.c_str(), id));
        if (reply)
            freeReplyObject(reply);

        if (logger)
        {
            logger->debug(
                "Stored pattern {} in tier {} with {} relationships", id, tier, data.relationships.size());
        }
#endif
    }

    std::optional<sep::pattern::PatternData> loadPattern( std::size_t id,
                                           const std::string& tier)
    {
#if SEP_HAS_HIREDIS
        if (!connected_ || !context_)
            return std::nullopt;

        auto logger = sep::logging::Manager::getInstance().getLogger("redis");
        std::lock_guard<std::mutex> lock(mutex_);

        std::stringstream key;
        key << "pattern:" << tier << ":" << id;

        // Check if pattern exists
        redisReply* reply = static_cast<redisReply*>(redisCommand(context_, "EXISTS %s", key.str().c_str()));
        if (!reply || reply->type != REDIS_REPLY_INTEGER || reply->integer == 0)
        {
            if (reply)
                freeReplyObject(reply);
            return std::nullopt;
        }
        freeReplyObject(reply);

        sep::pattern::PatternData data;

        // Load position
        reply = static_cast<redisReply*>(
            redisCommand(context_, "HMGET %s position:x position:y position:z", key.str().c_str()));
        if (reply && reply->type == REDIS_REPLY_ARRAY && reply->elements == 3)
        {
            data.position.x = reply->element[0]->str ? std::stof(reply->element[0]->str) : 0.0f;
            data.position.y = reply->element[1]->str ? std::stof(reply->element[1]->str) : 0.0f;
            data.position.z = reply->element[2]->str ? std::stof(reply->element[2]->str) : 0.0f;
        }
        if (reply)
            freeReplyObject(reply);

        // Load metadata
        reply = static_cast<redisReply*>(
            redisCommand(context_,
                         "HMGET %s coherence stability generation_count access_frequency timestamp dag_node_id",
                         key.str().c_str()));
        if (reply && reply->type == REDIS_REPLY_ARRAY && reply->elements == 6)
        {
            data.coherence = reply->element[0]->str ? std::stof(reply->element[0]->str) : 0.0f;
            data.stability = reply->element[1]->str ? std::stof(reply->element[1]->str) : 0.0f;
            data.generation = reply->element[2]->str ? std::stoi(reply->element[2]->str) : 0;
            // We're ignoring access_frequency since it's not in the core PatternData structure
            // Just use coherence value for both fields
            data.coherence = reply->element[0]->str ? std::stof(reply->element[0]->str) : 0.0f;
            
            // Parse timestamp but don't try to store it since it's not in the target structure
            // Just parse for logging purposes
            long long ts = reply->element[4]->str ? std::stoll(reply->element[4]->str) : 0;
            (void)ts; // timestamp currently unused
        }
        if (reply)
            freeReplyObject(reply);

        // Load relationship count
        reply = static_cast<redisReply*>(redisCommand(context_, "HGET %s rel_count", key.str().c_str()));
        size_t rel_count = 0;
        if (reply && reply->type == REDIS_REPLY_STRING)
        {
            rel_count = std::stoul(reply->str);
        }
        if (reply)
            freeReplyObject(reply);

        // Load relationships
        for (size_t i = 0; i < rel_count; ++i)
        {
            reply = static_cast<redisReply*>(redisCommand(
                context_, "HMGET %s rel:%zu:id rel:%zu:type rel:%zu:strength", key.str().c_str(), i, i, i));

            if (reply && reply->type == REDIS_REPLY_ARRAY && reply->elements == 3)
            {
                // Create a PatternRelationship from the Redis data
                sep::pattern::PatternRelationship patternRel;
                
                // Get the raw ID from Redis
                size_t rel_id = reply->element[0]->str ? std::stoull(reply->element[0]->str) : 0;
                
                // Convert the numeric ID to a string identifier
                std::string id_str = "pattern_" + std::to_string(rel_id);
                patternRel.targetId = id_str.c_str();
                
                // Set the relationship type (convert from int to enum)
                int type_int = reply->element[1]->str ? std::stoi(reply->element[1]->str) : 0;
                patternRel.type = static_cast<uint8_t>(type_int);
                
                // Set the strength
                patternRel.strength = reply->element[2]->str ? std::stof(reply->element[2]->str) : 0.0f;
                
                // Add to the relationships vector
                data.relationships.push_back(patternRel);
            }
            if (reply)
                freeReplyObject(reply);
        }

        if (logger)
        {
            logger->debug(
                "Loaded pattern {} from tier {} with {} relationships", id, tier, data.relationships.size());
        }

        return data;
#else
        return std::nullopt;
#endif
    }

    std::vector<std::size_t> getPatternIds( const std::string& tier)
    {
#if SEP_HAS_HIREDIS
        std::vector<std::size_t> ids;
        if (!connected_ || !context_)
            return ids;

        auto logger = sep::logging::Manager::getInstance().getLogger("redis");
        std::lock_guard<std::mutex> lock(mutex_);

        std::stringstream key;
        key << tier << ":patterns";

        redisReply* reply = static_cast<redisReply*>(redisCommand(context_, "SMEMBERS %s", key.str().c_str()));

        if (reply && reply->type == REDIS_REPLY_ARRAY)
        {
            ids.reserve(reply->elements);
            for (size_t i = 0; i < reply->elements; ++i)
            {
                if (reply->element[i]->str)
                {
                    ids.push_back(std::stoull(reply->element[i]->str));
                }
            }
        }
        if (reply)
            freeReplyObject(reply);

        if (logger)
        {
            logger->debug("Retrieved {} pattern IDs from tier {}", ids.size(), tier);
        }

        return ids;
#else
        return {};
#endif
    }

    void removePattern( std::size_t id,  const std::string& tier)
    {
#if SEP_HAS_HIREDIS
        if (!connected_ || !context_)
            return;

        auto logger = sep::logging::Manager::getInstance().getLogger("redis");
        std::lock_guard<std::mutex> lock(mutex_);

        std::stringstream key;
        key << "pattern:" << tier << ":" << id;

        // Remove from tier index
        redisReply* reply = static_cast<redisReply*>(redisCommand(context_, "SREM %s:patterns %zu", tier.c_str(), id));
        if (reply)
            freeReplyObject(reply);

        // Remove pattern data
        reply = static_cast<redisReply*>(redisCommand(context_, "DEL %s", key.str().c_str()));
        if (reply)
            freeReplyObject(reply);

        if (logger)
        {
            logger->debug("Removed pattern {} from tier {}", id, tier);
        }
#endif
    }

    void bulkStore(const std::vector<std::pair<std::size_t, sep::pattern::PatternData>>& patterns, const std::string& tier)
    {
        for (const auto& pair : patterns)
        {
            storePattern(pair.first, pair.second, tier);
        }
    }

    std::vector<sep::pattern::PatternData> bulkLoad(const std::vector<std::size_t>& ids, const std::string& tier)
    {
        std::vector<sep::pattern::PatternData> results;
        results.reserve(ids.size());

        for (std::size_t id : ids)
        {
            auto data_opt = loadPattern(id, tier);
            if (data_opt)
            {
                results.push_back(*data_opt);
            }
        }

        return results;
    }

private:
    redisContext* context_;
    bool          connected_;
    std::mutex    mutex_;
};

// RedisManager implementation
RedisManager::RedisManager(const std::string& host, int port) : impl_(std::make_unique<Impl>(host, port)) {}
RedisManager::~RedisManager() = default;

void RedisManager::storePattern(std::size_t id, const sep::pattern::PatternData& data, const std::string& tier)
{
    impl_->storePattern(id, data, tier);
}

std::optional<sep::pattern::PatternData> RedisManager::loadPattern(std::size_t id, const std::string& tier)
{
    return impl_->loadPattern(id, tier);
}

std::vector<std::size_t> RedisManager::getPatternIds(const std::string& tier)
{
    return impl_->getPatternIds(tier);
}

void RedisManager::removePattern(std::size_t id, const std::string& tier)
{
    impl_->removePattern(id, tier);
}

void RedisManager::bulkStore(const std::vector<std::pair<std::size_t, sep::pattern::PatternData>>& patterns, const std::string& tier)
{
    impl_->bulkStore(patterns, tier);
}

std::vector<sep::pattern::PatternData> RedisManager::bulkLoad(const std::vector<std::size_t>& ids, const std::string& tier)
{
    return impl_->bulkLoad(ids, tier);
}

bool RedisManager::isConnected() const
{
    return impl_->isConnected();
}

}  // namespace sep::persistence
