#include "dag_graph.h"

// Standard library includes
#include <algorithm>
#include <vector>

namespace sep {
namespace dag {

    uint64_t DagGraph::addNode(const glm::vec3& pattern, float coherence,
                               const std::vector<uint64_t>& parents)
    {
        uint64_t id = next_id_++;
        nodes_[id] = DagNode{id, pattern, coherence, parents};
        return id;
    }

    uint64_t DagGraph::addNodeWithId(uint64_t id, const glm::vec3& pattern, float coherence,
                                     const std::vector<uint64_t>& parents)
    {
        // Update next_id_ if the provided id is higher
        if (id >= next_id_)
        {
            next_id_ = id + 1;
        }

        nodes_[id] = DagNode{id, pattern, coherence, parents};
        return id;
    }

void DagGraph::updateNodeParents(uint64_t id, const std::vector<uint64_t>& parents)
{
    auto it = nodes_.find(id);
    if (it != nodes_.end())
    {
        it->second.parents = parents;
    }
}

void DagGraph::updateCoherence(uint64_t id, float coherence)
{
    auto it = nodes_.find(id);
    if (it != nodes_.end())
    {
        it->second.coherence = coherence;
    }
}

std::vector<uint64_t> DagGraph::getParents(uint64_t id) const
{
    auto it = nodes_.find(id);
    if (it != nodes_.end())
    {
        return it->second.parents;
    }
    return {};
}

void DagGraph::removeNode(uint64_t id)
{
    auto it = nodes_.find(id);
    if (it != nodes_.end())
    {
        nodes_.erase(it);
        for (auto& pair : nodes_) {
            auto& node = pair.second;
            node.parents.erase(std::remove(node.parents.begin(), node.parents.end(), id), node.parents.end());
        }
    }
}

bool DagGraph::hasNode(uint64_t id) const
{
    return nodes_.find(id) != nodes_.end();
}

}  // namespace dag
}  // namespace sep
