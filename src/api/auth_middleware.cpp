#include "api/auth_middleware.h"

#include <cstring>
#include <string>
#include <vector>

namespace sep::api {

    void AuthMiddleware::set_tokens(shim::vector<shim::string> tokens)
    {
        tokens_ = std::move(tokens);
    }

    bool AuthMiddleware::validate_token(const shim::string& header) const
    {
        if (tokens_.empty())
        {
            return true;
        }

        static const shim::string prefix = "Bearer ";
        if (header.rfind(prefix, 0) != 0)
        {
            return false;
        }

        auto provided = header.substr(prefix.size());
        for (const auto& tok : tokens_)
        {
            if (provided == tok)
            {
                return true;
            }
        }
        return false;
    }

}  // namespace sep::api
