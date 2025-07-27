# Visual Component Pipeline

The main entry point for the application is in `src/apps/workbench/core/workbench_main.cpp`, which creates a `WorkbenchEngine` object and calls its `run` method.

The `WorkbenchEngine` class is the core of the application. Its `run` method contains the main application loop. Inside the loop, the `renderFrame` method is called to render the UI.

The `renderFrame` method in `src/apps/workbench/core/workbench_core.cpp` orchestrates the rendering of the entire UI. It calls two key methods:

1.  `layout_manager_->render()`: This is responsible for the overall layout of the UI. The implementation is in `src/apps/workbench/core/ui_layout_manager.cpp`.
2.  `renderTabs()`: This method, also in `workbench_core.cpp`, is responsible for rendering the main tab bar and the content of each tab.

The `renderTabs` method creates a tab bar with the following tabs:

*   **Signals**: Controlled by `SignalsTabController` (`src/apps/workbench/tabs/signals_tab_controller.cpp`). This tab is responsible for displaying the main price chart, technical indicators, and SEP signals.
*   **Engine**: Controlled by `EngineTabController` (`src/apps/workbench/tabs/engine_tab_controller.cpp`). This tab likely displays information about the SEP engine's status and performance.
*   **Backend**: Controlled by `BackendTabController` (`src/apps/workbench/tabs/backend_tab_controller.cpp`). This tab provides tools for data source selection, backtesting, and paper trading. It contains the "Signal Validation" window that was causing the `PopID` error.
*   **Backtester**: Controlled by `BacktesterTabController` (`src/apps/workbench/backtester/ui/backtester_tab_controller.cpp`). This tab is dedicated to backtesting strategies.

In addition to the main tabs, there are also panels, such as the `SignalTestingPanel` (`src/apps/workbench/panels/signal_testing_panel.cpp`), which appears to be a self-contained component for testing signals.