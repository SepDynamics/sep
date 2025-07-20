#ifndef SEP_CONTEXT_RELATIONSHIP_H
#define SEP_CONTEXT_RELATIONSHIP_H
#include <string>
#include <unordered_map>
#include <vector>

#include "engine/types.h"

namespace sep::context {

    float cosineSimilarity(const shim::vector<float>& a, const shim::vector<float>& b);
    bool simplePatternMatch(const shim::string& pattern, const shim::string& text);

    // Relationship strength and metadata
    struct RelationshipInfo
    {
        shim::string target_id;
        uint8_t type{0};  // 0 = REFERENCE
        float strength{0.0f};
        bool bidirectional{false};
    };

// Relationship configuration
struct RelationshipConfig {
  float min_strength_threshold{0.3f};
  float min_relevance_threshold{0.4f};
  size_t max_relationships{1000};
  bool auto_prune{true};
};

class RelationshipManager {
 public:
  explicit RelationshipManager(const RelationshipConfig& config = RelationshipConfig{});
  ~RelationshipManager() = default;

  // Add a relationship between contexts
  bool addRelationship(const shim::string& source_id, const shim::string& target_id, uint8_t type,
                       float strength, bool bidirectional = false);

  // Remove a relationship
  bool removeRelationship(const shim::string& source_id, const shim::string& target_id);

  // Get relationships for a context
  shim::vector<RelationshipInfo> getRelationships(const shim::string& context_id) const;

  // Get parent-child relationships
  shim::vector<shim::string> getChildren(const shim::string& parent_id) const;
  shim::string getParent(const shim::string& child_id) const;

  // Calculate relationship strength using normalized dot product
  float calculateRelationshipStrength(const shim::vector<float>& embedding1,
                                      const shim::vector<float>& embedding2) const;

  // Prune weak relationships
  size_t pruneWeakRelationships(const shim::string& context_id);

  // Check if relationship count exceeds maximum
  bool exceedsMaxRelationships(const shim::string& context_id) const;

  // Get relationship count
  size_t getRelationshipCount(const shim::string& context_id) const;

  // Store cosine similarity result between two contexts
  float storeCosineSimilarity(const shim::string& source_id, const shim::string& target_id,
                              const shim::vector<float>& emb_a, const shim::vector<float>& emb_b);

  // Store pattern matching result between two contexts
  bool storePatternMatch(const shim::string& source_id, const shim::string& target_id,
                         const shim::string& pattern, const shim::string& text);

  private:
  RelationshipConfig config_;
  std::unordered_map<shim::string, shim::vector<RelationshipInfo>> relationships_;
  std::unordered_map<shim::string, std::unordered_map<shim::string, float>> similarity_map_;
  std::unordered_map<shim::string, std::unordered_map<shim::string, bool>> pattern_map_;
};

}  // namespace sep::context

#endif  // SEP_CONTEXT_RELATIONSHIP_H