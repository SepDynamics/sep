#ifndef SEP_CONTEXT_PRIORITY_H
#define SEP_CONTEXT_PRIORITY_H

#include <ctime>
#include <memory>
#include <unordered_map>

#include "engine/internal/math_common.h"
#include "engine/internal/standard_includes.h"

namespace sep::context {

// Priority tiers for contexts
enum class PriorityTier {
    LOW,
    NORMAL,
    HIGH,
    CRITICAL
};

// Priority multipliers for different context types
struct PriorityMultipliers {
    float low_multiplier{1.0f};
    float normal_multiplier{2.0f};
    float high_multiplier{3.5f};
    float critical_multiplier{5.0f};
};

// Context priority information
struct PriorityInfo {
    PriorityTier tier{PriorityTier::NORMAL};
    time_t last_access_time;
    std::size_t access_count{0};
    float base_score{0.0f};
    float time_decay{1.0f};
    float final_score{0.0f};
    sep::string parent_context_id;
};

// Priority system configuration
struct PriorityConfig {
    float time_decay_factor{0.05f};    // Decay factor for exp(-hours * factor)
    float hourly_decay_rate{0.95f};    // Decay rate per hour
    float access_multiplier{1.1f};     // Multiplier per access
    float relationship_weight{0.01f};  // Weight for relationship count
    float parent_influence{0.2f};      // Parent priority influence
    int recalc_interval_minutes{60};   // Priority recalculation interval (60 minutes)
    int expiration_hours{30 * 24};     // Context expiration (30 days)
};

class PriorityManager {
public:
    explicit PriorityManager(const PriorityConfig& config = PriorityConfig{});
    ~PriorityManager() = default;

    // Calculate time decay based on hours since last access
    float calculateTimeDecay(time_t last_access) const;
    
    // Get priority multiplier for a tier
    float getPriorityMultiplier(PriorityTier tier) const;
    
    // Calculate final score based on base relevance, time decay, and priority multiplier
    float calculateFinalScore(float base_relevance, float time_decay, PriorityTier tier) const;
    
    // Update priority info for a context
    void updatePriority(const sep::string& context_id, PriorityInfo& info);

    // Record context access
    void recordAccess(const sep::string& context_id, PriorityInfo& info);

    // Determine if a context should be expired
    bool shouldExpire(const PriorityInfo& info) const;
    
    // Determine if priority should be recalculated
    bool shouldRecalculatePriority(const PriorityInfo& info) const;
    
    // Adjust priority based on relationship count
    void adjustPriorityForRelationships(PriorityInfo& info, std::size_t relationship_count);
    
    // Adjust priority based on parent context
    void adjustPriorityForParent(PriorityInfo& info, const PriorityInfo& parent_info);

private:
    PriorityConfig config_;
    PriorityMultipliers multipliers_;
};

} // namespace sep::context

#endif // SEP_CONTEXT_PRIORITY_H