#pragma once

// Include only crow_isolation.h instead of direct http header files
#include "crow/crow_isolation.h"
#include <vector>
#include <string>

namespace sep::api {

struct AuthMiddleware {
  struct context {
    bool authorized{false};
  };

  void set_tokens(std::vector<std::string> tokens);

  template <typename AllContext>
  void before_handle(crow::request& req, crow::response& res, context& ctx, AllContext&);

  template <typename AllContext>
  void after_handle(crow::request&, crow::response&, context&, AllContext&) {}

protected:
  bool validate_token(const std::string& header) const;
  std::vector<std::string> tokens_;
};

}  // namespace sep::api

// Template implementation
namespace sep::api {

template <typename AllContext>
void AuthMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx,
                                    AllContext&) {
  if (validate_token(req.get_header_value("Authorization"))) {
    ctx.authorized = true;
    return;
  }

  // Use the crow::status enum directly
  res.code = static_cast<int>(crow::status::UNAUTHORIZED);
  res.body = "{\"error\":\"unauthorized\"}";
  res.end();
  ctx.authorized = false;
}

}  // namespace sep::api

