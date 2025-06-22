#include "blender/trace_viewer/trace_viewer.h"

#include <iostream>

namespace sep {
namespace blender {

TraceViewer::TraceViewer(sep::pattern::PatternProcessor* processor)
    : processor_(processor) {}

void TraceViewer::drawEvolutionPaths() {
  if (!processor_) return;
  const auto& patterns = processor_->getPatterns();
  for (const auto& p : patterns) {
    std::cout << "Pattern " << p.id << " coherence " << p.coherence
              << " stability " << p.stability << std::endl;
  }
}

}  // namespace blender
}  // namespace sep
