# Integration Tasks

This file outlines conventions for moving features from the testbed into the main codebase.

## Namespace Style

Subsystems should use the C++17 nested namespace syntax with `sep` as the root followed by the module name. For example:

```cpp
namespace sep::quantum {
    // ...
}
```


## Subsystem Startup

Validate startup logic in the testbed before migrating code to `src/`.
