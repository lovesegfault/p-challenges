#include "test.h"

static char *get_color(color c) {
    static char *ansi = NULL;
    switch (c) {
        case RED:
            ansi = "\033[31m";
            break;
        case RED_B:
            ansi = "\033[1m\033[31m";
            break;
        case GREEN:
            ansi = "\033[32m";
            break;
        case GREEN_B:
            ansi = "\033[1m\033[32m";
            break;
        case YELLOW:
            ansi = "\x1b[33m";
            break;
        case YELLOW_B:
            ansi = "\033[1m\x1b[33m";
            break;
        case BLUE:
            ansi = "\x1b[34m";
            break;
        case BLUE_B:
            ansi = "\033[1m\x1b[34m";
            break;
        case MAGENTA:
            ansi = "\x1b[35m";
            break;
        case MAGENTA_B:
            ansi = "\033[1m\x1b[35m";
            break;
        case CYAN:
            ansi = "\x1b[36m";
            break;
        case CYAN_B:
            ansi = "\033[1m\x1b[36m";
            break;
        case NORMAL:
            ansi = "\033[0m";
            break;
    }
    return ansi;
}

void printc(color c, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char *color = get_color(c);
    char *rst = get_color(NORMAL);
    size_t len = strlen(color) + strlen(format) + strlen(rst) + 1;

    char *colored = calloc(len, sizeof(char));
    snprintf(colored, len, "%s%s%s", color, format, rst);

    vprintf(colored, args);
    va_end(args);

    free(colored);
}

void section(const char *name) {
    printc(BLUE, "Testing ");
    printc(BLUE_B, name);
    printc(BLUE, ":\n");
}

void assert_section(const char *name, struct t_result *results) {
    printc(BLUE, "    » ");
    printc(BLUE_B, "%s", name);
    printc(BLUE, " | ");
    color status = (results->failed > 0) ? RED_B : GREEN_B;
    size_t total = results->failed + results->succeeded;
    printc(status, "PASSED %zu/%zu", results->succeeded, total);
    printc(BLUE, " | TOOK %Lf\n", results->total / 1000000);
}

double elapsed_micros(struct timeval start, struct timeval end) {
    double start_micros, end_micros;
    start_micros = (double)(start.tv_sec*(1000000)) + (double)(start.tv_usec);
    end_micros = (double)(end.tv_sec*(1000000)) + (double)(end.tv_usec);
    return end_micros - start_micros;
}

long double avg(const long double arr[], size_t len) {
    long double sum = 0;
    for(size_t i = 0; i < len; ++i)
        sum += arr[i];
    return (sum/len);
}

typedef struct {
    long double max;
    long double min;
    long double avg;
    long double total;
} timing;

timing compute_timing(const long double *arr, size_t len) {
    timing t = {.total = 0, .avg = 0, .min = arr[0], .max = 0};
    for(size_t i = 0; i < len; ++i){
        if(arr[i] > t.max) t.max = arr[i];
        if(arr[i] < t.min) t.min = arr[i];
        t.total += arr[i];
    }
    t.avg = t.total / len;
    return t;
}

void print_timing(timing *t){
    printc(GREEN, "MIN: %Lf", t->min / 1000000);
    printc(BLUE, " | ");
    printc(MAGENTA, "MAX: %Lf", t->max / 1000000);
    printc(BLUE, " | ");
    printc(CYAN, "AVG: %Lf", t->avg / 1000000);
}



bool assert(const char *name, test_fn test, struct t_result *results, size_t runs) {
    long double *buf = calloc(runs, sizeof(long double));
    bool result = true;

    for(size_t i = 0; i < runs; ++i){
        struct timeval start, end;

        gettimeofday(&start, 0);
        result &= (*test)();
        gettimeofday(&end, 0);

        // We could use a rolling average, but precision is crucial here
        buf[i] = elapsed_micros(start, end);
    }
    timing t = compute_timing(buf, runs);
    free(buf);
    results->total += t.total;

    if (result) {
        printc(GREEN_B, "    ✓ ");
        printc(GREEN, "SUCCESS");
        ++(results->succeeded);
    } else {
        printc(RED_B, "    ✕ ");
        printc(RED, "FAIL   ");
        ++(results->failed);
    }
    printc(BLUE, " | ");
    print_timing(&t);
    printc(BLUE, " | ");
    printc(YELLOW_B, "%s\n", name);
    return result;
}