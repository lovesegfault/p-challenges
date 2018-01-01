//
// Created by bemeurer on 12/30/17.
//

#ifndef INC_00_NAME_GENERATOR_NAME_GENERATOR_H
#define INC_00_NAME_GENERATOR_NAME_GENERATOR_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

typedef struct person {
    char *name;
    char *email;
    char *address;
    char *SSN;
    struct tm DOB;
};

char *generate_SSN();

struct tm generate_DOB();

char *generate_address();

#endif //INC_00_NAME_GENERATOR_NAME_GENERATOR_H
