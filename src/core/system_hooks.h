#pragma once

#include <cstddef>
#include <string_view>

namespace sep::core {
class SystemHooks {
public:
    virtual ~SystemHooks() = default;
    virtual void onPatternProcessed(std::size_t) {}
    virtual void onPatternPromoted(std::size_t) {}
    virtual void onPatternDemoted(std::size_t) {}
    virtual void onSubsystemStarting(std::string_view /*name*/) {}
    virtual void onSubsystemReady(std::string_view /*name*/) {}
    virtual void onSubsystemError(std::string_view /*name*/,
                                 std::string_view /*msg*/) {}
};
} // namespace sep::core
