#pragma once

// This is a fixed version of the app.h file from the Crow framework
// It provides stub implementations for the Crow application class

// Include our own headers
#include "compat/shim.h"
#include "common.h"
#include "crow_isolation.h"

namespace crow {
    // The Crow class is already defined in crow_isolation.h as a template
    // This file just ensures that it's properly included and instantiated
    
    // Create a factory function to create a Crow app
    template <typename... Middlewares>
    inline crow<Middlewares...>* make_app() {
        return new crow<Middlewares...>();
    }
    
    // Add any additional Crow app functionality needed here
    
    // Helper function to set up routes
    template <typename... Middlewares>
    inline DummyRoute route(crow<Middlewares...>* app, const sep::shim::string& url) {
        if (app) {
            return app->route(url);
        }
        return {};
    }
    
    // Helper function to run the app
    template <typename... Middlewares>
    inline void run(crow<Middlewares...>* app) {
        if (app) {
            app->run();
        }
    }
    
    // Helper function to stop the app
    template <typename... Middlewares>
    inline void stop(crow<Middlewares...>* app) {
        if (app) {
            app->stop();
        }
    }
}