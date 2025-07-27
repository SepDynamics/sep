#pragma once

#include "apps/workbench/core/alpha_tracker.h"
#include "imgui.h"

namespace sep::workbench {

// Forward declarations
class WorkbenchEngine;

class AlphaDemoTabController {
public:
    AlphaDemoTabController(WorkbenchEngine* engine);
    void render();

private:
    WorkbenchEngine* workbench_engine_;
    AlphaTracker* alpha_tracker_;
};

}
