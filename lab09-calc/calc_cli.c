#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input> <output>\n", argv[0]);
        return 1;
    }
    int ret = 1;
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Unable to open '%s' for reading", argv[1]);
        goto f_closed_reading_closed_writing;
    }
    FILE *output = fopen(argv[2], "w");
    if (!output) {
        fprintf(stderr, "Unable to open '%s' for writing", argv[2]);
        goto f_opened_reading_closed_writing;
    }
    size_t capacity = 1;
    char *expr = malloc(capacity);
    if (expr == NULL) {
        fprintf(stderr, "Unable to allocate buffer");
        goto buf_freed;
    }
    while (!feof(input)) {
        size_t number_of_read = 0;
        char current_char = (char)fgetc(input);
        if (current_char == EOF && !feof(input)) {
            fprintf(stderr, "Error while reading from the input");
            goto buf_not_freed;
        }
        while (current_char != '\n' && !feof(input)) {
            expr[number_of_read++] = current_char;
            if (number_of_read >= capacity) {
                char *temp = (char *)realloc(expr, capacity *= 2);
                if (temp == NULL) {
                    fprintf(stderr, "Unable to allocate buffer");
                    goto buf_not_freed;
                } else {
                    expr = temp;
                }
            }
            current_char = (char)(fgetc(input));
        }
        expr[number_of_read] = '\0';
        if (*expr == '\0') {
            continue;
        }
        calc_result res;
        int return_code = calc_evaluate(expr, &res, NULL);

        if (!return_code) {
            fprintf(output, "%.3lf\n", res.value);
        } else {
            fprintf(output, "Error %d:\n  ", return_code);
            fprintf(output, "%s", expr);
            fprintf(output, "\n  ");
            for (int i = 0; i <= res.error_position - 1; i++) {
                fprintf(output, ".");
            }
            fprintf(output, "^\n");
        }
    }
    ret = 0;
buf_not_freed:
    free(expr);
buf_freed:
    fclose(output);
f_opened_reading_closed_writing:
    fclose(input);
f_closed_reading_closed_writing:;
    return ret;
}