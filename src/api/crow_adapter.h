/**
 * @file crow_adapter.h
 * @brief Header file for the Crow adapter for the SEP Engine API
 *
 * This file declares the functions needed to integrate the SEP Engine API
 * with the Crow web framework.
 */

#pragma once

#include "crow.h"

#include <memory>
#include <string>
#include "api/types.h"

namespace sep::api {

/**
 * @brief Setup the SEP API routes in a Crow application
 *
 * This function sets up the routes for the SEP Engine API in a Crow application.
 *
 * @param app The Crow application instance
 */
void setupSepApiRoutes(::crow::Crow<>* app);

// Adapter for crow::request to HttpRequest
class CrowRequestAdapter : public HttpRequest {
public:
  explicit CrowRequestAdapter(::crow::request &req);
  std::string url() const override;
  std::string method() const override;
  std::string body() const override;

 private:
  ::crow::request &req_;
  std::string method_str_;
};

// Adapter for crow::response to HttpResponse
class CrowResponseAdapter : public HttpResponse {
public:
  explicit CrowResponseAdapter(::crow::response &res);
  void setCode(int code) override;
  int getCode() const override;
  void setBody(const std::string &body) override;
  void end() override;
  std::string getBody() const override;

 private:
  ::crow::response &res_;
};

std::unique_ptr<HttpResponse> makeResponse(::crow::response &res);
std::unique_ptr<HttpRequest> makeRequest(::crow::request &req);

}  // namespace sep::api
