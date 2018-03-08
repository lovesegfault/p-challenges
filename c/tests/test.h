#ifndef TESTS_LIBRARY_H
#define TESTS_LIBRARY_H
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

typedef bool (*test_fn)();

struct t_result{
    size_t succeeded;
    size_t failed;
    long double total;
};

typedef enum {
    RED,
    RED_B,
    GREEN,
    GREEN_B,
    YELLOW,
    YELLOW_B,
    BLUE,
    BLUE_B,
    MAGENTA,
    MAGENTA_B,
    CYAN,
    CYAN_B,
    NORMAL,
} color;

void section(const char *name);
void assert_section(const char *name, struct t_result *results);

bool assert(const char *name, test_fn test, struct t_result *results, size_t runs);

#endif