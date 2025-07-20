#pragma once

#include "engine/shim.h"

#ifdef NDEBUG
#define SEP_ASSERT(condition, message) ((void)0)
#else
#define SEP_ASSERT(condition, message)                                                             \
    do                                                                                             \
    {                                                                                              \
        if (!(condition))                                                                          \
        {                                                                                          \
            sep::shim::cerr << "Assertion `" #condition "` failed in " << __FILE__ << " line "     \
                            << __LINE__ << ": " << message << std::endl;                           \
            sep::shim::terminate();                                                                \
        }                                                                                          \
    } while (false)
#endif