#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int main() {
    char *input = calloc(10, sizeof(char));
    struct tm birthday ={};
    while(true) {
        printf("Please input your birthday in format YYYY-MM-DD: ");
        if (fgets(input, 10, stdin) != NULL) {
            if(*strptime(input, "%F", &birthday) == '\0') {
                break;
            } else {
                continue;
            }
        } else {
            continue;
        }
    }
    time_t age = time(NULL) - mktime(&birthday);
    printf("You are %zu seconds old!\n", age);
    free(input);
    return 0;
}
