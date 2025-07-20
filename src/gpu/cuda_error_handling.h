/*
 * CUDA Error Handling Header
 * 
 * This file provides CUDA-specific error handling utilities that integrate
 * with the standardized error system. It includes functions for translating
 * CUDA errors and convenience macros for CUDA error checking.
 */

#ifndef SEP_CUDA_ERROR_HANDLING_H
#define SEP_CUDA_ERROR_HANDLING_H

#include "engine/../util/error_handling.h"
#include "engine/cuda_api_unified.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Error category for CUDA errors */
#define SEP_ERROR_CATEGORY_CUDA 0x2000

/* CUDA error codes (to be used with SEP_ERROR_CONTEXT) */
#define SEP_CUDA_ERROR_GENERAL         0x2001
#define SEP_CUDA_ERROR_MEMORY          0x2002
#define SEP_CUDA_ERROR_INITIALIZATION  0x2003
#define SEP_CUDA_ERROR_LAUNCH_FAILURE  0x2004
#define SEP_CUDA_ERROR_EXECUTION       0x2005
#define SEP_CUDA_ERROR_SYNCHRONIZATION 0x2006

/* 
 * Translates a CUDA error code (from cuda_api_unified.h) to the standardized error system.
 * 
 * @param cuda_error The CUDA error to translate
 * @param level The severity level of the error
 * @param file The source file where the error occurred
 * @param line The line number where the error occurred
 * @param function The function where the error occurred
 * @return The standardized error context
 */
SEP_ERROR_CONTEXT sep_cuda_translate_error(SEP_CUDA_ERROR cuda_error, 
                                          SEP_ERROR_LEVEL level,
                                          const char* file, 
                                          int line, 
                                          const char* function);

/* 
 * Reports a CUDA error through the standardized error system.
 * 
 * @param cuda_error The CUDA error to report
 * @param level The severity level of the error
 * @param file The source file where the error occurred
 * @param line The line number where the error occurred
 * @param function The function where the error occurred
 * @param message Additional error message (can be NULL)
 */
void sep_cuda_report_error(SEP_CUDA_ERROR cuda_error, 
                          SEP_ERROR_LEVEL level,
                          const char* file, 
                          int line, 
                          const char* function,
                          const char* message);

/* Convenience macro for CUDA error checking */
#define SEP_CUDA_CHECK_ERROR(cuda_call, level, message) \
    do { \
        SEP_CUDA_ERROR error = cuda_call; \
        if (error.code != SEP_CUDA_SUCCESS) { \
            sep_cuda_report_error(error, level, __FILE__, __LINE__, __func__, message); \
            return error; \
        } \
    } while (0)

/* Convenience macro for CUDA error checking with goto error handler */
#define SEP_CUDA_CHECK_ERROR_GOTO(cuda_call, level, message, label) \
    do { \
        SEP_CUDA_ERROR error = cuda_call; \
        if (error.code != SEP_CUDA_SUCCESS) { \
            sep_cuda_report_error(error, level, __FILE__, __LINE__, __func__, message); \
            goto label; \
        } \
    } while (0)

/* Convenience macro for CUDA error checking with custom return value */
#define SEP_CUDA_CHECK_ERROR_RETURN(cuda_call, level, message, ret_val) \
    do { \
        SEP_CUDA_ERROR error = cuda_call; \
        if (error.code != SEP_CUDA_SUCCESS) { \
            sep_cuda_report_error(error, level, __FILE__, __LINE__, __func__, message); \
            return ret_val; \
        } \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* SEP_CUDA_ERROR_HANDLING_H */