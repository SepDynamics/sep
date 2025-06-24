#pragma once

// This is a simplified version of http_parser_merged.h that avoids conflicts
// with crow_isolation.h

// Include utility.h first to get CROW_LIKELY and CROW_UNLIKELY macros
#include "utility.h"

// Define CROW_LIKELY and CROW_UNLIKELY if not already defined
#ifndef CROW_LIKELY
#if defined(__GNUG__) || defined(__clang__)
#define CROW_LIKELY(X) __builtin_expect(!!(X), 1)
#define CROW_UNLIKELY(X) __builtin_expect(!!(X), 0)
#else
#define CROW_LIKELY(X) (X)
#define CROW_UNLIKELY(X) (X)
#endif
#endif

// Define CROW_HTTP_PARSER_ERRNO if not already defined
#ifndef CROW_HTTP_PARSER_ERRNO
#define CROW_HTTP_PARSER_ERRNO(parser) ((parser)->http_errno)
#endif

// We don't actually include http_parser_merged.h to avoid conflicts