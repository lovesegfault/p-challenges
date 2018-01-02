#ifndef INC_00_NAME_GENERATOR_NAME_GENERATOR_H
#define INC_00_NAME_GENERATOR_NAME_GENERATOR_H

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

typedef struct {
    char *name;
    char *email;
    char *address;
    char *SSN;
    struct tm DOB;
} person;

char *generate_SSN();

struct tm generate_DOB();

char *generate_address();

char *generate_name();

#endif //INC_00_NAME_GENERATOR_NAME_GENERATOR_H
