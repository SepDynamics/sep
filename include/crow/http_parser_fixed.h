#pragma once

// This is a fixed version of the http_parser_fixed.h file from the Crow framework
// It provides stub implementations for HTTP parser functionality

// Include our own headers
#include <http_parser.h>
#include "compat/shim.h"
#include "common.h"

namespace crow {
    namespace http_parser_stub {
        // These are already defined in crow_isolation.h, so we don't need to redefine them
        // enum class http_errno {...};
        // enum class http_parser_type {...};
        // struct http_parser {...};
        // struct http_parser_settings {...};

        // Delegate http_parser_* functions to the system http-parser library

        inline void http_parser_init(http_parser* parser, http_parser_type type) {
            ::http_parser_init(parser, type);
        }

        inline size_t http_parser_execute(http_parser* parser,
                                         const http_parser_settings* settings,
                                         const char* data,
                                         size_t len) {
            return ::http_parser_execute(parser, settings, data, len);
        }

        using ::http_parser_url;

        inline void http_parser_url_init(http_parser_url* u) {
            ::http_parser_url_init(u);
        }

        inline int http_parser_parse_url(const char* buf, size_t buflen,
                                        int is_connect,
                                        http_parser_url* u) {
            return ::http_parser_parse_url(buf, buflen, is_connect, u);
        }

        inline const char* http_errno_name(http_errno err) {
            return ::http_errno_name(err);
        }

        inline const char* http_errno_description(http_errno err) {
            return ::http_errno_description(err);
        }

        inline const char* http_method_str(unsigned int method) {
            return ::http_method_str(static_cast<http_method>(method));
        }
    }  // namespace http_parser_stub
}  // namespace crow
