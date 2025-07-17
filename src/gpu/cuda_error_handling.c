/*
 * CUDA Error Handling Implementation
 * 
 * This file provides the implementation for the CUDA-specific error handling
 * utilities defined in cuda_error_handling.h.
 */

#include "cuda_error_handling.h"
#include <string.h>
#include <stdio.h>

/* Translates a CUDA error code to a standardized error code */
static int translate_cuda_error_code(int cuda_code) {
    switch (cuda_code) {
        case SEP_CUDA_ERROR_INVALID_VALUE:
            return SEP_CUDA_ERROR_GENERAL;
            
        case SEP_CUDA_ERROR_OUT_OF_MEMORY:
            return SEP_CUDA_ERROR_MEMORY;
            
        case SEP_CUDA_ERROR_NOT_INITIALIZED:
        case SEP_CUDA_ERROR_NO_DEVICE:
            return SEP_CUDA_ERROR_INITIALIZATION;
            
        /* Add more mappings as needed when additional CUDA error codes are defined */
            
        default:
            return SEP_CUDA_ERROR_GENERAL;
    }
}

/* 
 * Translates a CUDA error to the standardized error system.
 */
SEP_ERROR_CONTEXT sep_cuda_translate_error(SEP_CUDA_ERROR cuda_error, 
                                          SEP_ERROR_LEVEL level,
                                          const char* file, 
                                          int line, 
                                          const char* function) {
    SEP_ERROR_CONTEXT ctx;
    
    /* Fill in context information */
    ctx.file = file;
    ctx.line = line;
    ctx.function = function;
    ctx.level = level;
    ctx.category = SEP_ERROR_CATEGORY_CUDA;
    ctx.error_code = translate_cuda_error_code(cuda_error.code);
    
    /* Use CUDA error message if available, otherwise use a generic message */
    if (cuda_error.message[0] != '\0') {
        ctx.message = cuda_error.message;
    } else {
        ctx.message = "Unknown CUDA error";
    }
    
    return ctx;
}

/* 
 * Reports a CUDA error through the standardized error system.
 */
void sep_cuda_report_error(SEP_CUDA_ERROR cuda_error, 
                          SEP_ERROR_LEVEL level,
                          const char* file, 
                          int line, 
                          const char* function,
                          const char* message) {
    SEP_ERROR_CONTEXT ctx = sep_cuda_translate_error(
        cuda_error, level, file, line, function);
    
    /* Static buffer for combined messages */
    static char combined_message[256];
    
    /* Append additional message if provided */
    if (message) {
        /* Create a combined message (limited to 255 chars) */
        snprintf(combined_message, sizeof(combined_message), "%s: %s", 
                ctx.message, message);
        combined_message[sizeof(combined_message) - 1] = '\0';
        
        /* Update the message in the context */
        ctx.message = combined_message;
    }
    
    /* Report the error */
    sep_error_report(ctx);
}