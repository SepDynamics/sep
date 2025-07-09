#pragma once

// Lightweight wrapper utilities for Crow applications.
// Use our isolation shim instead of trying to include the real Crow headers

// Include our isolation shim which defines all necessary types
#include "crow/crow_isolation.h"

namespace crow {

// Factory helper to create a Crow application.
template <typename... Middlewares>
inline Crow<Middlewares...>* make_app() {
    return new Crow<Middlewares...>();
}

// Convenience wrapper around Crow::route_dynamic.
template <typename... Middlewares>
inline auto route(Crow<Middlewares...>* app, const std::string& url) {
    return app->route_dynamic(url);
}

template <typename... Middlewares>
inline void run(Crow<Middlewares...>* app) {
    if (app)
        app->run();
}

template <typename... Middlewares>
inline void stop(Crow<Middlewares...>* app) {
    if (app)
        app->stop();
}

} // namespace crow
