/*
 * SEP Engine Type Safety Utilities
 * 
 * This header provides utilities and macros to ensure type safety,
 * prevent undefined behavior, and handle floating-point operations safely.
 */

#ifndef SEP_TYPE_SAFETY_H
#define SEP_TYPE_SAFETY_H

#include <cstddef>  /* For size_t */
#include <stdint.h>  /* For fixed-width integer types */
#include <cfloat>   /* For floating-point limits */
#include <math.h>    /* For isnan, isinf, etc. */
#include <stdbool.h> /* For bool type */
#include <limits.h>  /* For INT_MAX, INT_MIN */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Floating-point comparison utilities
 * 
 * These functions provide safe ways to compare floating-point values
 * while accounting for floating-point representation issues.
 */

/* Check if a floating-point value is valid (not NaN or infinity) */
static inline bool sep_float_is_valid(double value) {
    return !isnan(value) && !isinf(value);
}

/* Check if two floating-point values are approximately equal */
static inline bool sep_float_approx_equal(double a, double b, double epsilon) {
    if (a == b) {
        return true;  /* Handle exact equality, including both zero */
    }
    
    /* If either value is not valid, they are not equal */
    if (!sep_float_is_valid(a) || !sep_float_is_valid(b)) {
        return false;
    }
    
    /* Handle relative comparison for non-zero values */
    double abs_a = fabs(a);
    double abs_b = fabs(b);
    double diff = fabs(a - b);
    
    /* Use relative error unless one value is very close to zero */
    if (a == 0.0 || b == 0.0 || (abs_a + abs_b < DBL_MIN)) {
        /* For values very close to zero, use absolute error */
        return diff < epsilon;
    } else {
        /* For non-zero values, use relative error */
        return diff < epsilon * fmax(abs_a, abs_b);
    }
}

/* Default epsilon for double-precision floating-point comparisons */
#define SEP_FLOAT_EPSILON 1e-9

/* Macro for comparing floating-point values with default epsilon */
#define SEP_FLOAT_EQUAL(a, b) sep_float_approx_equal(a, b, SEP_FLOAT_EPSILON)

/*
 * Integer safety utilities
 * 
 * These macros and functions help prevent integer overflow, underflow,
 * and other undefined behaviors when working with integers.
 */

/* Check if addition would overflow */
static inline bool sep_int_add_would_overflow(int a, int b) {
    return (b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b);
}

/* Safe addition that detects overflow */
static inline bool sep_int_add_safe(int a, int b, int* result) {
    if (sep_int_add_would_overflow(a, b)) {
        return false;  /* Would overflow */
    }
    
    *result = a + b;
    return true;
}

/* Check if multiplication would overflow */
static inline bool sep_int_mul_would_overflow(int a, int b) {
    if (a == 0 || b == 0) {
        return false;
    }
    
    /* Check for overflow in various cases */
    if (a > 0 && b > 0) {
        return a > INT_MAX / b;
    } else if (a < 0 && b < 0) {
        return a < INT_MAX / b;
    } else if (a < 0) {
        return a < INT_MIN / b;
    } else {
        return b < INT_MIN / a;
    }
}

/* Safe multiplication that detects overflow */
static inline bool sep_int_mul_safe(int a, int b, int* result) {
    if (sep_int_mul_would_overflow(a, b)) {
        return false;  /* Would overflow */
    }
    
    *result = a * b;
    return true;
}

/* Pointer validity check */
#define SEP_PTR_CHECK(ptr) ((ptr) != NULL)

/* Pointer array bounds check */
#define SEP_ARRAY_BOUNDS_CHECK(array, index, length) \
    ((array) != NULL && (size_t)(index) < (size_t)(length))

/* Cast utilities that document intent */
#define SEP_STATIC_CAST(Type, expr) ((Type)(expr))
#define SEP_REINTERPRET_CAST(Type, expr) ((Type)(expr))

#ifdef __cplusplus
}
#endif

#endif /* SEP_TYPE_SAFETY_H */