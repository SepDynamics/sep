#pragma once

#ifdef SEP_HAS_AUDIO

#ifdef _WIN32
    #ifdef SEP_AUDIO_EXPORTS
        #define SEP_AUDIO_API __declspec(dllexport)
    #else
        #define SEP_AUDIO_API __declspec(dllimport)
    #endif
#else
    #ifdef SEP_AUDIO_EXPORTS
        #define SEP_AUDIO_API __attribute__((visibility("default")))
    #else
        #define SEP_AUDIO_API
    #endif
#endif

#else
    #define SEP_AUDIO_API
#endif