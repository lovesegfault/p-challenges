#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "temperature-converter.h"

temp_unit str_to_unit(char *str, bool *valid);

void lower_str(char *str);


int main() {
    printf("Temperature converter\n");
    printf("Available units:\nCelsius  | Fahrenheit\nKelvin   | Rankine\nDelisle  | Newton\n");
    printf("Reaumur  | Romer\n");

    bool valid_input = false;
    char *input = NULL;
    size_t n = 0;
    temp_unit in_unit = celsius;
    temp_unit out_unit = celsius;

    while (!valid_input) {
        printf("What unit would you like to convert from? ");
        getline(&input, &n, stdin);
        in_unit = str_to_unit(input, &valid_input);
        free(input);
        input = NULL;
        n = 0;
    }

    valid_input = false;
    double in_temp = 0;
    while (!valid_input) {
        printf("Input temperature: ");
        getline(&input, &n, stdin);
        char *p_end;
        in_temp = strtod(input, &p_end);
        if(input == p_end) {
            valid_input = false;
            continue;
        }
        if(errno == ERANGE) {
            printf("---- Input would cause overflow.\n");
            valid_input = false;
            continue;
        }
        valid_input = true;
        free(input);
        input = NULL;
        n = 0;
    }

    valid_input = false;
    while (!valid_input) {
        printf("What unit would you like to convert to? ");
        getline(&input, &n, stdin);
        out_unit = str_to_unit(input, &valid_input);
        free(input);
        input = NULL;
        n = 0;
    }

    printf("Result: %f", convert_temperature(in_temp, in_unit, out_unit));
    return 0;
}

temp_unit str_to_unit(char *str, bool *valid) {
    lower_str(str);
    if (strncmp(str, "celsius", 7) == 0) {
        *valid = true;
        return celsius;
    } else if (strncmp(str, "fahrenheit", 10) == 0) {
        *valid = true;
        return fahrenheit;
    } else if (strncmp(str, "kelvin", 6) == 0) {
        *valid = true;
        return kelvin;
    } else if (strncmp(str, "rankine", 7) == 0) {
        *valid = true;
        return rankine;
    } else if (strncmp(str, "delisle", 7) == 0) {
        *valid = true;
        return delisle;
    } else if (strncmp(str, "newton", 6) == 0) {
        *valid = true;
        return newton;
    } else if (strncmp(str, "reaumur", 7) == 0) {
        *valid = true;
        return reaumur;
    } else if (strncmp(str, "romer", 5) == 0) {
        *valid = true;
        return romer;
    }
    *valid = false;
    return celsius;
}

void lower_str(char *str) {
    for (size_t i = 0; i < strlen(str); ++i) {
        str[i] = (char) tolower(str[i]);
    }
}