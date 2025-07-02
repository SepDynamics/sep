#pragma once

// Lightweight wrapper utilities for Crow applications.  When building without
// CUDA the real Crow headers are included by crow_isolation.h.

#include "crow_isolation.h"

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
