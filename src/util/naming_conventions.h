/*
 * SEP Engine Naming Conventions
 * 
 * This header defines the standard naming conventions used throughout the SEP Engine
 * codebase to ensure consistency and avoid reserved identifiers as specified in
 * the C and C++ standards.
 */

#ifndef SEP_NAMING_CONVENTIONS_H
#define SEP_NAMING_CONVENTIONS_H

/*
 * Reserved Identifier Rules:
 * 
 * According to C and C++ standards, the following identifiers are reserved:
 * 
 * 1. Identifiers that begin with an underscore followed by an uppercase letter
 *    (e.g., _Identifier) are reserved in all contexts.
 * 
 * 2. Identifiers that begin with an underscore followed by another underscore
 *    (e.g., __identifier) are reserved in all contexts.
 * 
 * 3. Identifiers that begin with an underscore (e.g., _identifier) are reserved
 *    for use as identifiers with file scope in both the ordinary and tag name spaces.
 * 
 * 4. All identifiers that begin with "is" followed by a lowercase letter are reserved
 *    for future use by the C standard library.
 * 
 * 5. All identifiers that begin with "to" followed by a lowercase letter are reserved
 *    for future use by the C standard library.
 * 
 * 6. Names containing double underscore (__) or beginning with an underscore
 *    followed by a capital letter are reserved for C++ implementations.
 */

/*
 * SEP Engine Naming Conventions:
 * 
 * 1. All public API identifiers should be prefixed with "SEP_" followed by the 
 *    component or subsystem name, e.g., SEP_CUDA_Init().
 * 
 * 2. All macros should be in uppercase with words separated by underscores, 
 *    e.g., SEP_CUDA_CHECK_ERROR().
 * 
 * 3. Constants and enumerations should be in uppercase with words separated by 
 *    underscores, e.g., SEP_CUDA_SUCCESS.
 * 
 * 4. Types (structs, unions, enums, typedefs) should be in CamelCase with the 
 *    appropriate prefix, e.g., SEP_CUDA_DEVICE_PROPS.
 * 
 * 5. Functions should use CamelCase with the appropriate prefix, 
 *    e.g., SEP_CUDA_GetDeviceCount().
 * 
 * 6. Variables should use snake_case without prefixes, e.g., device_count.
 * 
 * 7. Private or internal functions should have a lowercase component/subsystem
 *    prefix, e.g., cuda_translate_error().
 * 
 * 8. Static file-scope variables should use snake_case with a component/subsystem
 *    prefix, e.g., cuda_error_string_buffer.
 */

/*
 * Examples of correct identifiers:
 * 
 * Public API:
 *   SEP_CUDA_Init()
 *   SEP_QUANTUM_ProcessPattern()
 * 
 * Types:
 *   SEP_CUDA_ERROR
 *   SEP_MEMORY_TierStats
 * 
 * Macros:
 *   SEP_CUDA_CHECK_ERROR()
 *   SEP_REPORT_ERROR()
 * 
 * Constants:
 *   SEP_CUDA_SUCCESS
 *   SEP_MEMORY_MAX_PATTERNS
 * 
 * Internal functions:
 *   cuda_translate_error()
 *   memory_calculate_fragmentation()
 * 
 * Variables:
 *   device_count
 *   pattern_coherence
 * 
 * Static file-scope variables:
 *   cuda_error_string_buffer
 *   memory_tier_handles
 */

#endif /* SEP_NAMING_CONVENTIONS_H */