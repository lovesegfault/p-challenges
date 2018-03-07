#ifndef INC_00_NAME_GENERATOR_NAME_GENERATOR_H
#define INC_00_NAME_GENERATOR_NAME_GENERATOR_H

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct _person {
    char *name;
    char *email;
    char *address;
    char *SSN;
    struct tm *DOB;

    void (*print)(struct _person *);
    void (*kill)(struct _person *);
} person;

char *generate_SSN();

struct tm *generate_DOB();

char *generate_address();

char *generate_name();

char *generate_email(char *name, struct tm *DOB);

void generate_person(person *);

void person_print(person *);

void person_kill(person *);


#endif //INC_00_NAME_GENERATOR_NAME_GENERATOR_H
