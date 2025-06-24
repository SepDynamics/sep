# SEP API Build Fix Game Plan

## Overview
This document provides a step-by-step plan to resolve the remaining build errors in the SEP API module. The approach prioritizes minimal changes, leveraging existing shared components, and maintaining clean architecture.

## Phase 1: Fix shim::string Compatibility

### Issue
The `sep::shim::string` type is incompatible with nlohmann::json's input adapters and spdlog's formatters.

### Solution Steps

1. **Enhance shim::string with Standard Library Compatibility**
   - Add iterator support to shim::string
   - Add data() method returning const char*
   - Ensure proper implicit conversions to/from std::string
   
   File: `include/compat/shim.h`
   ```cpp
   class string {
   public:
       // Add these methods:
       const char* data() const { return c_str(); }
       size_t length() const { return size(); }
       
       // Add iterator support
       using const_iterator = const char*;
       using iterator = char*;
       const_iterator begin() const { return data_; }
       const_iterator end() const { return data_ + size_; }
       iterator begin() { return data_; }
       iterator end() { return data_ + size_; }
   };
   ```

2. **Create JSON Adapter Helpers**
   
   File: `include/api/json_helpers.h`
   ```cpp
   #pragma once
   #include "compat/shim.h"
   #include <nlohmann/json.hpp>
   #include <string>
   
   namespace sep::api {
   
   // Helper to parse JSON from shim::string
   inline nlohmann::json parse_json(const sep::shim::string& str) {
       // Convert to std::string first for compatibility
       return nlohmann::json::parse(std::string(str.c_str()));
   }
   
   // Helper to parse JSON from crow request body
   inline nlohmann::json parse_json(const std::string& str) {
       return nlohmann::json::parse(str);
   }
   
   } // namespace sep::api
   ```

3. **Update crow_adapter.cpp to use helpers**
   - Replace direct json::parse calls with the helper function
   - Use std::string conversions where needed

## Phase 2: Fix Crow Method Literals

### Issue
The `"POST"_method` syntax is not being recognized by the compiler.

### Solution Steps

1. **Replace method literals with enum values**
   
   In `src/api/server.cpp`, replace all occurrences:
   ```cpp
   // Replace: .methods("POST"_method)
   // With:    .methods(::crow::HTTPMethod::Post)
   
   // Replace: .methods("GET"_method)  
   // With:    .methods(::crow::HTTPMethod::Get)
   ```

2. **Update all route definitions**
   - Line 361: `"POST"_method` → `::crow::HTTPMethod::Post`
   - Line 407: `"POST"_method` → `::crow::HTTPMethod::Post`
   - Line 450: `"POST"_method` → `::crow::HTTPMethod::Post`
   - Line 493: `"POST"_method` → `::crow::HTTPMethod::Post`
   - Line 536: `"POST"_method` → `::crow::HTTPMethod::Post`
   - Line 579: `"POST"_method` → `::crow::HTTPMethod::Post`
   - Line 622: `"POST"_method` → `::crow::HTTPMethod::Post`
   - Line 665: `"GET"_method` → `::crow::HTTPMethod::Get`

## Phase 3: Fix OllamaClient Incomplete Type

### Issue
OllamaClient is only forward declared but needs complete type for unique_ptr operations.

### Solution Steps

1. **Include the full header in server.cpp**
   
   In `src/api/server.cpp`, add at the top:
   ```cpp
   #include "api/ollama_client.h"
   ```

2. **Ensure ollama_client.h is complete**
   - The header should define the full OllamaClient class
   - Move implementation details to ollama_client.cpp if needed

## Phase 4: Fix Crow Template Instantiation

### Issue
The Crow template with middlewares is not being properly instantiated.

### Solution Steps

1. **Include full Crow definition**
   
   In `src/api/server.cpp`, ensure proper includes:
   ```cpp
   // Add after other includes
   #include "crow/crow.h"
   #include "crow/middlewares/cors.h"
   ```

2. **Fix forward declaration issues**
   
   In `include/api/crow_adapter.h` and `include/api/server.h`:
   ```cpp
   // Change: struct request;
   // To:     class request;
   
   // Change: struct response;
   // To:     class response;
   ```

## Phase 5: Fix Reference to Temporary Warnings

### Issue
Methods returning `const std::string&` from crow request/response members.

### Solution Steps

1. **Update CrowRequestAdapter methods**
   
   In `src/api/crow_adapter.cpp`:
   ```cpp
   // Change these methods to return by value:
   std::string url() const { return std::string(req_.url); }
   std::string body() const { return std::string(req_.body); }
   
   // Keep method() as is since method_str_ is a member
   const std::string& method() const { return method_str_; }
   ```

2. **Update CrowResponseAdapter methods**
   ```cpp
   std::string getBody() const { return std::string(res_.body); }
   ```

3. **Update the interface in crow_adapter.h accordingly**

## Phase 6: Fix spdlog Formatting Issues

### Issue
spdlog/fmt cannot format sep::shim::string types.

### Solution Steps

1. **Convert shim::string to std::string for logging**
   
   In `src/api/server.cpp`, line 270:
   ```cpp
   // Replace the logger call with explicit conversions:
   logger_->info("Request: {} {} - Status: {} - Duration: {}ms", 
                 std::string(::crow::method_name(req.method)),
                 std::string(req.url), 
                 code, 
                 duration);
   ```

## Phase 7: Fix app_ Member Access

### Issue
Line 81 error suggests app_ might be void or incomplete.

### Solution Steps

1. **Ensure app_ is properly initialized**
   
   In the SEPApiServer constructor, verify:
   ```cpp
   app_ = std::make_unique<::crow::Crow<RateLimitMiddleware, AuthMiddleware>>();
   ```

2. **Fix the chaining syntax**
   ```cpp
   // If chaining is causing issues, split it:
   app_->port(config_.port);
   app_->multithreaded();
   app_->run();
   ```

## Implementation Order

1. **Start with Phase 1** - Fix shim::string compatibility (highest priority)
2. **Then Phase 2** - Fix method literals (quick fix)
3. **Then Phase 3** - Fix OllamaClient includes
4. **Then Phase 4** - Fix Crow template issues
5. **Then Phase 5** - Fix reference warnings
6. **Then Phase 6** - Fix logging format issues
7. **Finally Phase 7** - Fix any remaining app_ issues

## Testing Strategy

After each phase:
1. Run `make -j$(nproc)` to check if errors are reduced
2. Save the build log to track progress
3. Don't proceed to next phase until current phase compiles

## Key Principles

- **No new dependencies** - Work within existing framework
- **Minimal changes** - Small, targeted fixes
- **Preserve architecture** - Don't restructure, just fix compatibility
- **Use existing patterns** - Follow patterns already in the codebase
- **Type safety** - Ensure all conversions are explicit and safe

## Expected Outcome

After implementing all phases:
- All compilation errors should be resolved
- The API module should build successfully
- No new complexity introduced
- Clean integration with existing shared components