#include "person-generator.h"

#define STREET_LIST  "lists/street_names.txt"
#define FIRST_NAME_LIST  "lists/first_names.txt"
#define LAST_NAME_LIST  "lists/last_names.txt"

static bool HAS_URANDOM = true; // Global

unsigned int random_uint() {
    unsigned int r_uint;
    FILE *f;

    f = fopen("/dev/urandom", "r");
    if (f == NULL) {
        if (HAS_URANDOM) {
            printf("---- Failed loading random generator device /dev/urandom. Defaulting to rand().\n");
            srand((unsigned int) time(NULL));
            HAS_URANDOM = false;
        }
        r_uint = (unsigned int) rand();
    } else {
        fread(&r_uint, sizeof(r_uint), 1, f);
        fclose(f);
    }
    return r_uint;
}

// Inclusive range
// https://stackoverflow.com/a/17554531/2080712
unsigned int generate_int(unsigned int lower, unsigned int upper) {
    unsigned int r_uint;
    const unsigned int range = 1 + (upper - lower);
    const unsigned int buckets = UINT_MAX / range;
    const unsigned int limit = buckets * range;

    if (range >= UINT_MAX) {
        fprintf(stderr, "Range too big!\n");
        exit(EXIT_FAILURE);
    }

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do {
        r_uint = random_uint();
    } while (r_uint >= limit);

    unsigned int res = lower + (r_uint / buckets);
    return res;
}

char *generate_SSN() {
    char *SSN = calloc(10, sizeof(char)); // 9 SSN digits + \0
    size_t group_number;
    size_t serial_number;
    size_t lead_number;

    do {
        group_number = generate_int(1, 99);
    } while (group_number == 0);

    do {
        serial_number = generate_int(1, 9999);
    } while (serial_number == 0);

    do {
        lead_number = generate_int(1, 999);
    } while ((lead_number == 666) || (lead_number > 900));

    snprintf(SSN, 10, "%03zu%02zu%04zu",
             lead_number, group_number, serial_number);

    static const char *know_ad = "219099999";
    static const char *woolworth = "219099999";

    if ((strncmp(SSN, know_ad, 9) == 0) || (strncmp(SSN, woolworth, 9) == 0)) {
        free(SSN);
        SSN = generate_SSN();
    }

    return (SSN);
}

unsigned int month_days(int month, int year) {
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap) return 29;
        return 28;
    } else {
        return 31;
    }
}

struct tm *generate_DOB() {
    struct tm *DOB = calloc(1, sizeof(struct tm));

    time_t c_time = time(NULL);
    struct tm *current = localtime(&c_time);

    DOB->tm_year = generate_int(0, (unsigned int) current->tm_year);

    if (DOB->tm_year == current->tm_year) {
        DOB->tm_mon = generate_int(1, (unsigned int) current->tm_mon);
    } else {
        DOB->tm_mon = generate_int(1, 12);
    }

    if (DOB->tm_mon == current->tm_mon) {
        DOB->tm_mday = generate_int(1, (unsigned int) current->tm_mday);
    } else {
        DOB->tm_mday = generate_int(1, month_days(DOB->tm_mon, DOB->tm_year));
    }

    return DOB;
}

unsigned int count_lines(FILE *file) {
    if (file == NULL) return 0;

    int ch;
    unsigned int line_count = 0;

    while ((ch = getc(file)) != EOF) {
        if (ch == '\n') ++line_count;
    }
    rewind(file);
    return line_count;
}

char *get_line(FILE *file, unsigned int line_number) {
    if (file == NULL) return NULL;

    int track_ch;
    long track_pos;
    size_t line_count = 0;
    char *line = NULL;

    while (((track_ch = getc(file)) != EOF) && ((track_pos = ftell(file)) != -1)) {
        if (track_ch == '\n') ++line_count;

        if (line_count == line_number - 1) {
            size_t char_count = 0;
            fseek(file, track_pos, SEEK_SET);

            int ch = getc(file);
            while (ch != '\n' && ch != EOF) {
                ++char_count;
                ch = getc(file);
            }

            line = calloc(char_count + 1, sizeof(char));
            fseek(file, track_pos, SEEK_SET);

            for (size_t i = 0; i < char_count; ++i) {
                line[i] = (char) getc(file);
            }
            line[char_count] = '\0';

            break;
        }
    }
    rewind(file);
    return line;
}


char *generate_address() {
    static const char *suffixes[8] = {"Street", "Lane", "Avenue", "Row", "Route", "Passage", "Boulevard", "Way"};

    FILE *name_list = fopen(STREET_LIST, "r");
    if (name_list == NULL) {
        printf("File %s failed to open. Please verify your CWD.\n", STREET_LIST);
        exit(EXIT_FAILURE);
    }

    unsigned int pick = generate_int(1, count_lines(name_list));
    char *name = get_line(name_list, pick);

    fclose(name_list);

    const char *suf = suffixes[generate_int(0, 7)];
    size_t sz = (strlen(name) + strlen(suf) + 3 + 4);
    char *street = calloc(sz, sizeof(char));
    strncpy(street, suf, sz * sizeof(char));

    strcat(street, " ");
    strcat(street, name);

    free(name);

    char *number = calloc(4, sizeof(char));
    sprintf(number, "%d", generate_int(1, 9999));

    strcat(street, " ");
    strcat(street, number);

    free(number);
    return street;
}

char *generate_name() {
    FILE *first_names = fopen(FIRST_NAME_LIST, "r");
    if (first_names == NULL) {
        printf("File %s failed to open. Please verify your CWD.\n", FIRST_NAME_LIST);
        exit(EXIT_FAILURE);
    }
    FILE *last_names = fopen(LAST_NAME_LIST, "r");
    if (last_names == NULL) {
        printf("File %s failed to open. Please verify your CWD.\n", LAST_NAME_LIST);
        exit(EXIT_FAILURE);
    }

    char *first = get_line(first_names, generate_int(1, count_lines(first_names)));
    char *last = get_line(last_names, generate_int(1, count_lines(last_names)));

    size_t sz = strlen(first) + strlen(last) + 2;

    char *name = (char *) calloc(sz, sizeof(char));
    strncpy(name, first, sz * sizeof(char));

    strcat(name, " ");
    strcat(name, last);

    free(first);
    free(last);
    fclose(first_names);
    fclose(last_names);
    return name;
}

char *minimize_str(char *str) {
    char *out = (char *) calloc(strlen(str), sizeof(char));
    for (size_t i = 0; i < strlen(str); ++i) {
        out[i] = (char) tolower(str[i]);
    }
    return out;
}


/*
 * split_str - Splits a string on delimiters
 * @param str Input string to be split
 * @param delim Delimiter to split at
 * @param dest Destination pointer of tokens
 * @return Number of tokens in dest
 */
size_t split_str(char *str, const char delim, char ***dest) {
    if (str == NULL) return 0;

    size_t delim_count = 0;
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == delim) {
            ++delim_count;
        }
    }
    char **tokens = calloc(delim_count + 1, sizeof(char *));
    if (tokens == NULL) return 0;

    char *dup = strdup(str);
    if (dup == NULL) return 0;

    char *found;
    delim_count = 0;
    while ((found = strsep(&dup, &delim)) != NULL) {
        tokens[delim_count] = calloc(strlen(found) + 1, sizeof(char));
        strcpy(tokens[delim_count], found);
        ++delim_count;
    }
    free(dup);
    *dest = tokens;

    return delim_count;
}

void shuffle(char **arr, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = n - 1; i > 0; --i) {
            size_t pick = generate_int(0, (unsigned int) n - 1);
            char *save = arr[pick];
            arr[pick] = arr[i];
            arr[i] = save;
        }
    }
}

char *generate_email(char *name, struct tm *DOB) {
    bool use_dob = (bool) (random_uint() & 1);
    bool use_dash = (bool) (random_uint() & 1);

    const char *providers[3] = {"@gmail.com", "@hotmail.com", "@yahoo.com"};

    char *provider = (char *) providers[generate_int(0, 2)];

    char **tokens;
    size_t name_count = split_str(name, ' ', &tokens);

    size_t email_size = 0;
    for (size_t i = 0; i < name_count; ++i) {
        email_size += strlen(tokens[i]);
    }
    email_size += strlen(provider);
    if (use_dash) {
        email_size += name_count - 1;
    }

    char *year = NULL;
    if (use_dob) {
        year = calloc(3, sizeof(char));
        strftime(year, 3, "%y", DOB);
        email_size += strlen(year);
    }

    char *email = calloc(email_size, sizeof(char));

    shuffle(tokens, name_count);

    for (size_t i = 0; i < name_count; ++i) {
        strcat(email, minimize_str(tokens[i]));
        if ((use_dash && i < name_count - 1) || (use_dash && use_dob)) {
            strcat(email, "-");
        }
    }
    if (use_dob) {
        strcat(email, year);
    }
    strcat(email, provider);

    return email;
}

void generate_person(person *self) {
    self->address = generate_address();
    self->DOB = generate_DOB();
    self->name = generate_name();
    self->email = generate_email(self->name, self->DOB);
    self->SSN = generate_SSN();

    self->print = person_print;
    self->kill = person_kill;
}

void person_print(person *self) {
    printf("SSN: %s\n", self->SSN);
    char *dob = calloc(11, sizeof(char));
    strftime(dob, 11, "%F", self->DOB);
    printf("DOB: %s\n", dob);
    free(dob);
    printf("Address: %s\n", self->address);
    printf("Name: %s\n", self->name);
    printf("Email: %s\n", self->email);

}

void person_kill(person *self) {
    free(self->address);
    free(self->SSN);
    free(self->name);
    free(self->email);
    free(self->DOB);
}