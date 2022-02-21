#ifndef CALC_LIB
#define CALC_LIB

#define CALC_MAX_ARITY 3

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

enum calc_error {
    CALC_ERROR_OK,
    CALC_ERROR_BAD_NUMBER,
    CALC_ERROR_UNKNOWN_FUNCTION,
    CALC_ERROR_EXPECTED_OPEN_PAREN,
    CALC_ERROR_EXPECTED_COMMA,
    CALC_ERROR_EXPECTED_CLOSE_PAREN,
    CALC_ERROR_UNEXPECTED_CHAR,
    CALC_ERROR_EXTRA_INPUT
};

typedef struct calc_function {
    const char *name;
    size_t arity;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201)
#endif
    union {
        double (*func0)(void);

        double (*func1)(double);

        double (*func2)(double, double);

        double (*func3)(double, double, double);
    };
#ifdef _MSC_VER
#pragma warning(pop)  // Восстановить настройки предупреждений
#endif
} calc_function;

#define CALC_FUNCTIONS_SENTINEL \
    {                           \
        NULL, 0, {              \
            NULL                \
        }                       \
    }
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201)
#endif
typedef union {
    double value;
    int error_position;
} calc_result;

typedef struct {
    // cppcheck-suppress unusedStructMember
    short is_error;
    union {
        // cppcheck-suppress unusedStructMember
        double value;
        struct {
            // cppcheck-suppress unusedStructMember
            int error_position;
            // cppcheck-suppress unusedStructMember
            short error;
        } err;
    } calc_res;
} calc_result_s;

int calc_evaluate(const char *expr,
                  calc_result *res,
                  const calc_function *functions
#ifdef __cplusplus
                  = nullptr
#endif
);

#ifdef __cplusplus
}
#endif

#endif  // CALC_LIB