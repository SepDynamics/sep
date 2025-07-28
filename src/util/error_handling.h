/**
 * @file error_handling.h
 * @brief Standardized error handling utilities for the SEP Engine
 * 
 * This file provides common error handling mechanisms that comply with
 * CERT-ERR33-C (detect and handle standard library errors). It includes
 * error codes, error reporting functions, and macros to simplify error
 * handling throughout the codebase.
 */

#ifndef SEP_UTIL_ERROR_HANDLING_H
#define SEP_UTIL_ERROR_HANDLING_H

#include <cstddef>
#include <cstdio>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Error severity levels
 */
typedef enum SEP_ERROR_LEVEL {
    SEP_ERROR_INFO = 0,     /* Informational message, not an error */
    SEP_ERROR_WARNING = 1,  /* Warning, operation can continue */
    SEP_ERROR_ERROR = 2,    /* Error, operation failed but program can continue */
    SEP_ERROR_FATAL = 3     /* Fatal error, program should terminate */
} SEP_ERROR_LEVEL;

/**
 * @brief Error categories
 */
typedef enum SEP_ERROR_CATEGORY {
    SEP_ERROR_CATEGORY_SYSTEM = 0,    /* System/OS errors */
    SEP_ERROR_CATEGORY_MEMORY = 1,    /* Memory allocation/management errors */
    SEP_ERROR_CATEGORY_IO = 2,        /* I/O and file operations errors */
    SEP_ERROR_CATEGORY_NETWORK = 3,   /* Network-related errors */
    SEP_ERROR_CATEGORY_CUDA = 4,      /* CUDA/GPU-related errors */
    SEP_ERROR_CATEGORY_QUANTUM = 5,   /* Quantum computation errors */
    SEP_ERROR_CATEGORY_API = 6,       /* API errors */
    SEP_ERROR_CATEGORY_DATABASE = 7,  /* Database errors */
    SEP_ERROR_CATEGORY_UNKNOWN = 8    /* Unknown error source */
} SEP_ERROR_CATEGORY;

/**
 * @brief Error context information
 */
typedef struct SEP_ERROR_CONTEXT {
    const char* file;       /* Source file where error occurred */
    int line;               /* Line number where error occurred */
    const char* function;   /* Function where error occurred */
    SEP_ERROR_LEVEL level;  /* Error severity level */
    SEP_ERROR_CATEGORY category;  /* Error category */
    int error_code;         /* Error code (e.g. errno value) */
    const char* message;    /* Custom error message */
} SEP_ERROR_CONTEXT;

/**
 * @brief Initialize the error handling system
 * 
 * @param log_file Path to the log file (NULL for stderr only)
 * @return 0 on success, non-zero on failure
 */
int sep_error_init(const char* log_file);

/**
 * @brief Clean up the error handling system
 */
void sep_error_cleanup(void);

/**
 * @brief Report an error with context information
 * 
 * @param ctx Error context information
 */
void sep_error_report(SEP_ERROR_CONTEXT ctx);

/**
 * @brief Get string representation of an error code
 * 
 * @param category Error category
 * @param error_code Error code
 * @return String representation of the error
 */
const char* sep_error_string(SEP_ERROR_CATEGORY category, int error_code);

/**
 * @brief Set the error callback function
 * 
 * @param callback Function to call when an error is reported
 */
void sep_error_set_callback(void (*callback)(SEP_ERROR_CONTEXT));

/* Convenience macros for error reporting */

/**
 * @brief Report a system error (errno-based)
 */
#define SEP_REPORT_SYSTEM_ERROR(level, error_code, message) \
    sep_error_report((SEP_ERROR_CONTEXT){ \
        .file = __FILE__, \
        .line = __LINE__, \
        .function = __func__, \
        .level = (level), \
        .category = SEP_ERROR_CATEGORY_SYSTEM, \
        .error_code = (error_code), \
        .message = (message) \
    })

/**
 * @brief Report a CUDA error
 */
#define SEP_REPORT_CUDA_ERROR(level, error_code, message) \
    sep_error_report((SEP_ERROR_CONTEXT){ \
        .file = __FILE__, \
        .line = __LINE__, \
        .function = __func__, \
        .level = (level), \
        .category = SEP_ERROR_CATEGORY_CUDA, \
        .error_code = (error_code), \
        .message = (message) \
    })

/**
 * @brief Check return value and report error if condition is true
 */
#define SEP_CHECK_ERROR(condition, level, category, error_code, message) \
    do { \
        if (condition) { \
            sep_error_report((SEP_ERROR_CONTEXT){ \
                .file = __FILE__, \
                .line = __LINE__, \
                .function = __func__, \
                .level = (level), \
                .category = (category), \
                .error_code = (error_code), \
                .message = (message) \
            }); \
            return (error_code); \
        } \
    } while (0)

/**
 * @brief Check for NULL pointer and report error if NULL
 */
#define SEP_CHECK_NULL(ptr, level, category, error_code, message) \
    SEP_CHECK_ERROR((ptr) == NULL, level, category, error_code, message)

/**
 * @brief Check for memory allocation failure
 */
#define SEP_CHECK_MEMORY(ptr) \
    SEP_CHECK_NULL(ptr, SEP_ERROR_ERROR, SEP_ERROR_CATEGORY_MEMORY, ENOMEM, \
                   "Memory allocation failed")

/**
 * @brief Check a file operation result
 */
#define SEP_CHECK_FILE_OP(condition, error_code, message) \
    SEP_CHECK_ERROR(condition, SEP_ERROR_ERROR, SEP_ERROR_CATEGORY_IO, \
                    error_code, message)

#ifdef __cplusplus
}
#endif

#endif /* SEP_UTIL_ERROR_HANDLING_H */