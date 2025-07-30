#pragma once

#include "engine/internal/standard_includes.h"
#include "core_types/result.h"

#ifdef NDEBUG
#define SEP_ASSERT(condition, message) ((void)0)
#else
#define SEP_ASSERT(condition, message)                                                             \
    do                                                                                             \
    {                                                                                              \
        if (!(condition))                                                                          \
        {                                                                                          \
            sep::cerr << "Assertion `" #condition "` failed in " << __FILE__ << " line "      \
                           << __LINE__ << ": " << message << std::endl;                            \
            sep::terminate();                                                                 \
        }                                                                                          \
    } while (false)
#endif