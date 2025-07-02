#pragma once

#include <cstdint>
#include <glm/vec3.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "compat/shim.h"

namespace sep {
namespace dag {

struct DagNode
{
    uint64_t              id;
    glm::vec3             pattern;
    float                 coherence;
    std::vector<uint64_t> parents;
};

class DagGraph
{
public:
    uint64_t addNode(const glm::vec3& pattern, float coherence, const std::vector<uint64_t>& parents);
    uint64_t addNodeWithId(uint64_t id, const glm::vec3& pattern, float coherence, const std::vector<uint64_t>& parents);
    void updateCoherence(uint64_t id, float coherence);
    void updateNodeParents(uint64_t id, const std::vector<uint64_t>& parents);
    std::vector<uint64_t> getParents(uint64_t id) const;
    void                  removeNode(uint64_t id);
    bool                  hasNode(uint64_t id) const;

    // Compatibility overload for integral identifiers that are not uint64_t
    template<typename T>
    typename std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, uint64_t>, uint64_t>
    addNode(T /*pattern_id*/, float coherence, const std::vector<T>& parents)
    {
        std::vector<uint64_t> converted_parents;
        converted_parents.reserve(parents.size());
        for (const auto& p : parents)
        {
            converted_parents.push_back(static_cast<uint64_t>(p));
        }
        glm::vec3 default_pattern(0.0f);
        return addNode(default_pattern, coherence, converted_parents);
    }

private:
    uint64_t                              next_id_{1};
    std::unordered_map<uint64_t, DagNode> nodes_;
};

}  // namespace dag
}  // namespace sep
