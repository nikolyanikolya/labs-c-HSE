#include "calc.h"

void skip_spaces(const char **s) {
    while (isspace(**s)) {
        (*s)++;
    }
}

char const prio[128] = {
    ['+'] = 1,
    ['-'] = 1,
    ['*'] = 2,
    ['/'] = 2,
};
calc_function const default_functions[] = {
    {.name = "sqrt", .arity = 1, .func1 = sqrt},
    {.name = "sin", .arity = 1, .func1 = sin},
    {.name = "cos", .arity = 1, .func1 = cos},
    {.name = "pow", .arity = 2, .func2 = pow},
    CALC_FUNCTIONS_SENTINEL};
calc_result_s calc_atom(const char **s,
                        const char *begin,
                        const calc_function *functions);
calc_result_s calc_expr(int cur_prio,
                        const char **s,
                        const char *begin,
                        const calc_function *functions) {
    if (cur_prio == 3) {
        return calc_atom(s, begin, functions);
    }
    calc_result_s prev = calc_expr(cur_prio + 1, s, begin, functions);
    if (!prev.is_error) {
        skip_spaces(s);
        while (prio[(int)**s] == cur_prio) {
            char op = **s;
            (*s)++;
            skip_spaces(s);
            calc_result_s next = calc_expr(cur_prio + 1, s, begin, functions);
            if (!next.is_error) {
                skip_spaces(s);
                switch (op) {
                    case '+':
                        prev.calc_res.value += next.calc_res.value;
                        break;
                    case '-':
                        prev.calc_res.value -= next.calc_res.value;
                        break;
                    case '*':
                        prev.calc_res.value *= next.calc_res.value;
                        break;
                    case '/':
                        prev.calc_res.value /= next.calc_res.value;
                        break;
                }
            } else {
                return next;
            }
        }
    }
    return prev;
}

calc_result_s calc_fuctions(const char **s,
                            const char *begin,
                            const calc_function *functions) {
    calc_result_s res;
    const char *name_f = *s;
    while (isalpha(**s)) {
        (*s)++;
    }
    int func_l = (int)(*s - begin);
    skip_spaces(s);
    if (**s != '(') {
        res.is_error = 1;
        res.calc_res.err.error = CALC_ERROR_EXPECTED_OPEN_PAREN;
        res.calc_res.err.error_position = (int)(*s - begin);
        return res;
    }
    (*s)++;
    skip_spaces(s);
    size_t func_i = 0;
    while (functions[func_i].name != NULL &&
           strncmp(name_f, functions[func_i].name,
                   strlen(functions[func_i].name)) != 0) {
        func_i++;
    }
    if (functions[func_i].name == NULL) {
        res.is_error = 1;
        res.calc_res.err.error = CALC_ERROR_UNKNOWN_FUNCTION;
        res.calc_res.err.error_position = func_l;
        return res;
    }
    size_t arity_ = functions[func_i].arity;
    double args[3];
    for (size_t i = 0; i < arity_; i++) {
        calc_result_s res_expr = calc_expr(1, s, begin, functions);
        if (res_expr.is_error) {
            return res_expr;
        }
        args[i] = res_expr.calc_res.value;
        skip_spaces(s);
        if (i != arity_ - 1 && **s != ',') {
            res.is_error = 1;
            res.calc_res.err.error = CALC_ERROR_EXPECTED_COMMA;
            res.calc_res.err.error_position = (int)(*s - begin);
            return res;
        } else if (i == arity_ - 1) {
            if (**s != ')') {
                res.is_error = 1;
                res.calc_res.err.error = CALC_ERROR_EXPECTED_CLOSE_PAREN;
                res.calc_res.err.error_position = (int)(*s - begin);
                return res;
            }
        }
        (*s)++;
        skip_spaces(s);
    }
    if (arity_ == 0) {
        skip_spaces(s);
        if (**s != ')') {
            res.is_error = 1;
            res.calc_res.err.error = CALC_ERROR_EXPECTED_CLOSE_PAREN;
            res.calc_res.err.error_position = (int)(*s - begin);
            return res;
        }
        (*s)++;
    }
    switch (arity_) {
        case 0:
            res.calc_res.value = functions[func_i].func0();
            break;
        case 1:
            res.calc_res.value = functions[func_i].func1(args[0]);
            break;
        case 2:
            res.calc_res.value = functions[func_i].func2(args[0], args[1]);
            break;
        case 3:
            res.calc_res.value =
                functions[func_i].func3(args[0], args[1], args[2]);
            break;
    }
    res.is_error = 0;
    return res;
}

int calc_evaluate(const char *expr,
                  calc_result *res,
                  const calc_function *functions) {
    const char *begin = expr;
    calc_result_s res_expr = calc_expr(1, &expr, begin, functions);
    if (!res_expr.is_error && !*expr) {
        res->value = res_expr.calc_res.value;
        return CALC_ERROR_OK;
    } else if (*expr && !res_expr.is_error) {
        res->error_position = (int)(expr - begin);
        return CALC_ERROR_EXTRA_INPUT;
    }
    res->error_position = res_expr.calc_res.err.error_position;
    return res_expr.calc_res.err.error;
}

calc_result_s calc_number(const char **s, const char *begin) {
    const char *temp = *s;
    calc_result_s res;
    double result = strtod(*s, (char **)s);
    if (!result && temp == *s) {
        res.is_error = 1;
        res.calc_res.err.error_position = (int)(*s - begin);
        res.calc_res.err.error = CALC_ERROR_BAD_NUMBER;
        return res;
    }
    res.is_error = 0;
    res.calc_res.value = result;
    return res;
}

calc_result_s calc_atom(const char **s,
                        const char *begin,
                        const calc_function *functions) {
    if (isdigit(**s) || **s == '.' || **s == '-' || **s == '+') {
        return calc_number(s, begin);
    }

    if (**s != '(') {
        if (functions != NULL) {
            return calc_fuctions(s, begin, functions);
        } else {
            return calc_fuctions(s, begin, default_functions);
        }
    }
    calc_result_s res;
    if (**s != '(') {
        res.is_error = 1;
        res.calc_res.err.error = CALC_ERROR_EXPECTED_OPEN_PAREN;
        res.calc_res.err.error_position = (int)(*s - begin);
        return res;
    }
    (*s)++;
    skip_spaces(s);
    calc_result_s res_expr = calc_expr(1, s, begin, functions);
    if (res_expr.is_error) {
        return res_expr;
    }

    skip_spaces(s);
    if (**s != ')') {
        res.is_error = 1;
        res.calc_res.err.error = CALC_ERROR_EXPECTED_CLOSE_PAREN;
        res.calc_res.err.error_position = (int)(*s - begin);
        return res;
    }
    (*s)++;
    return res_expr;
}
