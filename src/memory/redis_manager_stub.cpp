#include "memory/redis_manager.h"

namespace sep::persistence {

class StubRedisManager : public IRedisManager {
public:
    StubRedisManager(const std::string&, int) {}
    void storePattern(std::uint64_t, const PersistentPatternData&, const std::string&) override {}
    std::optional<PersistentPatternData> loadPattern(std::uint64_t, const std::string&) override { return std::nullopt; }
    std::vector<std::uint64_t> getPatternIds(const std::string&) override { return {}; }
    void removePattern(std::uint64_t, const std::string&) override {}
    void bulkStore(const std::vector<std::pair<std::uint64_t, PersistentPatternData>>&, const std::string&) override {}
    std::vector<PersistentPatternData> bulkLoad(const std::vector<std::uint64_t>&, const std::string&) override { return {}; }
    bool isConnected() const override { return false; }
};

std::shared_ptr<IRedisManager> createRedisManager(const std::string& host, int port) {
    return std::make_shared<StubRedisManager>(host, port);
}

} // namespace sep::persistence
