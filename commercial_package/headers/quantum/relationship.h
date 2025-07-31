#ifndef SEP_CONTEXT_RELATIONSHIP_H
#define SEP_CONTEXT_RELATIONSHIP_H
#include <string>
#include <unordered_map>
#include <vector>

#include "engine/internal/types.h"

namespace sep::context {

    float cosineSimilarity(const std::vector<float>& a, const std::vector<float>& b);
    bool simplePatternMatch(const std::string& pattern, const std::string& text);

    // Relationship strength and metadata
    struct RelationshipInfo
    {
        std::string target_id;
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
  bool addRelationship(const std::string& source_id, const std::string& target_id, uint8_t type,
                       float strength, bool bidirectional = false);

  // Remove a relationship
  bool removeRelationship(const std::string& source_id, const std::string& target_id);

  // Get relationships for a context
  std::vector<RelationshipInfo> getRelationships(const std::string& context_id) const;

  // Get parent-child relationships
  std::vector<std::string> getChildren(const std::string& parent_id) const;
  std::string getParent(const std::string& child_id) const;

  // Calculate relationship strength using normalized dot product
  float calculateRelationshipStrength(const std::vector<float>& embedding1,
                                      const std::vector<float>& embedding2) const;

  // Prune weak relationships
  size_t pruneWeakRelationships(const std::string& context_id);

  // Check if relationship count exceeds maximum
  bool exceedsMaxRelationships(const std::string& context_id) const;

  // Get relationship count
  size_t getRelationshipCount(const std::string& context_id) const;

  // Store cosine similarity result between two contexts
  float storeCosineSimilarity(const std::string& source_id, const std::string& target_id,
                              const std::vector<float>& emb_a, const std::vector<float>& emb_b);

  // Store pattern matching result between two contexts
  bool storePatternMatch(const std::string& source_id, const std::string& target_id,
                         const std::string& pattern, const std::string& text);

  private:
  RelationshipConfig config_;
  std::unordered_map<std::string, std::vector<RelationshipInfo>> relationships_;
  std::unordered_map<std::string, std::unordered_map<std::string, float>> similarity_map_;
  std::unordered_map<std::string, std::unordered_map<std::string, bool>> pattern_map_;
};

}  // namespace sep::context

#endif  // SEP_CONTEXT_RELATIONSHIP_H