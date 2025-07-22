#pragma once

// Handle ASIO/Crow includes based on RTTI availability
#ifndef CROW_DISABLE_RTTI
// Use real headers in non-CUDA mode
#include "crow/http_request.h"
#include "crow/http_response.h"
#else
// Use isolation headers in CUDA mode

#endif
#include <vector>

namespace sep::api {

struct AuthMiddleware {
  struct context {
    bool authorized{false};
  };

  void set_tokens(std::vector<std::string> tokens);

  template <typename AllContext>
  void before_handle(::crow::request& req, ::crow::response& res, context& ctx, AllContext&);

  template <typename AllContext>
  void after_handle(::crow::request&, ::crow::response&, context&, AllContext&) {}

protected:
  bool validate_token(const std::string& header) const;
  std::vector<std::string> tokens_;
};

}  // namespace sep::api

// Template implementation
namespace sep::api {

template <typename AllContext>
void AuthMiddleware::before_handle(::crow::request& req, ::crow::response& res, context& ctx,
                                   AllContext&) {
  if (validate_token(req.get_header_value("Authorization"))) {
    ctx.authorized = true;
    return;
  }

  // Use the global crow::status enum explicitly to avoid accidental
  // lookup of a similarly named namespace under sep::crow
  res.code = static_cast<int>(::crow::status::UNAUTHORIZED);
  res.body = "{\"error\":\"unauthorized\"}";
  res.end();
  ctx.authorized = false;
}

}  // namespace sep::api

