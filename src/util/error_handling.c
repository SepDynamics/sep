/**
 * @file error_handling.c
 * @brief Implementation of standardized error handling utilities
 */

#include "engine/error_handling.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Static variables for error handling */
static FILE* error_log_file = NULL;
static void (*error_callback)(SEP_ERROR_CONTEXT) = NULL;

/* Maximum size of formatted error message */
#define MAX_ERROR_MSG_SIZE 1024

/* Array of error category names for logging */
static const char* error_category_names[] = {
    "SYSTEM",
    "MEMORY",
    "IO",
    "NETWORK",
    "CUDA",
    "QUANTUM",
    "API",
    "DATABASE",
    "UNKNOWN"
};

/* Array of error level names for logging */
static const char* error_level_names[] = {
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL"
};

/**
 * @brief Initialize the error handling system
 */
int sep_error_init(const char* log_file) {
    /* Close any existing log file */
    if (error_log_file != NULL && error_log_file != stderr) {
        fclose(error_log_file);
        error_log_file = NULL;
    }
    
    /* If no log file specified, use stderr */
    if (log_file == NULL) {
        error_log_file = stderr;
        return 0;
    }
    
    /* Open the log file */
    error_log_file = fopen(log_file, "a");
    if (error_log_file == NULL) {
        /* Fall back to stderr if log file can't be opened */
        error_log_file = stderr;
        fprintf(stderr, "Failed to open log file %s: %s\n", log_file, strerror(errno));
        return -1;
    }
    
    return 0;
}

/**
 * @brief Clean up the error handling system
 */
void sep_error_cleanup(void) {
    if (error_log_file != NULL && error_log_file != stderr) {
        fclose(error_log_file);
        error_log_file = stderr;
    }
    error_callback = NULL;
}

/**
 * @brief Format the current time as a string
 */
static void format_time(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    if (tm_info != NULL) {
        strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
    } else {
        strncpy(buffer, "UNKNOWN TIME", size);
        buffer[size - 1] = '\0';
    }
}

/**
 * @brief Report an error with context information
 */
void sep_error_report(SEP_ERROR_CONTEXT ctx) {
    char time_buffer[32];
    char message_buffer[MAX_ERROR_MSG_SIZE];
    
    /* Format the current time */
    format_time(time_buffer, sizeof(time_buffer));
    
    /* Make sure the log file is initialized */
    if (error_log_file == NULL) {
        error_log_file = stderr;
    }
    
    /* Ensure the category and level indices are valid */
    if (ctx.category >= (sizeof(error_category_names) / sizeof(error_category_names[0]))) {
        ctx.category = SEP_ERROR_CATEGORY_UNKNOWN;
    }
    
    if (ctx.level >= (sizeof(error_level_names) / sizeof(error_level_names[0]))) {
        ctx.level = SEP_ERROR_ERROR;
    }
    
    /* Format the error message */
    snprintf(message_buffer, sizeof(message_buffer),
             "[%s] %s [%s] (%s:%d in %s): %s (Code: %d)\n",
             time_buffer,
             error_level_names[ctx.level],
             error_category_names[ctx.category],
             ctx.file,
             ctx.line,
             ctx.function,
             ctx.message ? ctx.message : "No message",
             ctx.error_code);
    
    /* Write to the log file */
    fputs(message_buffer, error_log_file);
    fflush(error_log_file);
    
    /* Call the error callback if registered */
    if (error_callback != NULL) {
        error_callback(ctx);
    }
    
    /* For fatal errors, terminate the program */
    if (ctx.level == SEP_ERROR_FATAL) {
        fflush(NULL);  /* Flush all stdio output streams */
        abort();
    }
}

/**
 * @brief Get string representation of an error code
 */
const char* sep_error_string(SEP_ERROR_CATEGORY category, int error_code) {
    switch (category) {
        case SEP_ERROR_CATEGORY_SYSTEM:
            return strerror(error_code);
            
        case SEP_ERROR_CATEGORY_CUDA:
            /* CUDA error strings will be handled by the CUDA API */
            return "See CUDA error string";
            
        /* Add more categories as needed */
            
        default:
            return "Unknown error";
    }
}

/**
 * @brief Set the error callback function
 */
void sep_error_set_callback(void (*callback)(SEP_ERROR_CONTEXT)) {
    error_callback = callback;
}