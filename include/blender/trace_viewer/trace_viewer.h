#pragma once



namespace sep {
namespace blender {

class TraceViewer {
 public:
  explicit TraceViewer(sep::pattern::PatternProcessor* processor);
  void drawEvolutionPaths();

 private:
  sep::pattern::PatternProcessor* processor_;
};

}  // namespace blender
}  // namespace sep
