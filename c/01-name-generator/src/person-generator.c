#include "person-generator.h"

#define STREET_LIST  "lists/street_names.txt"
#define FIRST_NAME_LIST  "lists/first_names.txt"
#define LAST_NAME_LIST  "lists/last_names.txt"

static bool HAS_URANDOM = true; // Global

unsigned int random_uint() {
    unsigned int r_uint;
    // Try to open the random generator device
    FILE *f = fopen("/dev/urandom", "r");
    if (f == NULL) {
        if (HAS_URANDOM) {
            // Warn that urandom isn't working, but fallthrough to rand()
            printf("---- Failed loading random generator device /dev/urandom. Defaulting to rand().\n");
            srand((unsigned int) time(NULL));
            HAS_URANDOM = false;
        }
        r_uint = (unsigned int) rand();
    } else {
        // If we have urandom, just read from it and cast to uint
        fread(&r_uint, sizeof(r_uint), 1, f);
        fclose(f);
    }
    return r_uint;
}

// Inclusive range
// https://stackoverflow.com/a/17554531/2080712
unsigned int generate_int(unsigned int lower, unsigned int upper) {
    if (upper - lower == UINT_MAX) {
        fprintf(stderr, "Invalid bounds on generate_int().\n");
        exit(EXIT_FAILURE);
    }
    unsigned int r_uint;
    const unsigned int range = 1 + (upper - lower);
    if (range == 0) {
        fprintf(stderr, "Invalid range!\n---- upper=%d\n---- lower=%d\n---- range=%d\n", upper, lower, range);
        exit(EXIT_FAILURE);
    }

    const unsigned int buckets = UINT_MAX / range;
    const unsigned int limit = buckets * range;

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
    if (SSN == NULL) {
        fprintf(stderr, "Failed allocating SSN buffer.\n");
        exit(EXIT_FAILURE);
    }
    // A social security number is composed of:
    size_t group_number = generate_int(1, 99); // A group number (2 digits)
    size_t serial_number = generate_int(1, 9999); // A serial number (4 digits)
    size_t lead_number; // A lead number (3 digits)

    do {
        lead_number = generate_int(1, 999);
    } while ((lead_number == 666) || (lead_number > 900)); // Lead number cannot be 666 or >900 for reasons?

    snprintf(SSN, 10, "%03zu%02zu%04zu", lead_number, group_number, serial_number); // Format with appropriate padding

    // There are known invalid SSNs, if they are generated, try again
    static const char *know_ad = "219099999";
    static const char *woolworth = "219099999";

    if ((strncmp(SSN, know_ad, 9) == 0) || (strncmp(SSN, woolworth, 9) == 0)) {
        free(SSN);
        SSN = generate_SSN();
    }

    return (SSN);
}

struct tm *generate_DOB() {
    time_t t = time(NULL); // Current time
    time_t pick = (time_t) (generate_int(0, (unsigned int) (t))); // Picking a random moment

    struct tm *DOB = malloc(sizeof(struct tm));
    localtime_r(&pick, DOB); // Instantiate a tm from our pick

    return DOB;
}

unsigned int count_lines(FILE *file) {
    if (file == NULL) return 0;

    int ch;
    unsigned int line_count = 0;
    // Walk through file counting \n
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

    // Get out street name list
    FILE *name_list = fopen(STREET_LIST, "re");
    if (name_list == NULL) {
        fprintf(stderr, "File %s failed to open. Please verify your CWD.\n", STREET_LIST);
        exit(EXIT_FAILURE);
    }

    // Pick a line from the list and store it
    unsigned int pick = generate_int(1, count_lines(name_list));
    char *name = get_line(name_list, pick);

    fclose(name_list);

    const char *suf = suffixes[generate_int(0, 7)]; // Pick a suffix
    size_t sz = (strlen(name) + strlen(suf) + 3 + 4); // Compute length
    //           ^ Name         ^ Suffix      ^---^ Spaces and Street number
    char *street = calloc(sz, sizeof(char));
    strncpy(street, suf, sz * sizeof(char));

    strcat(street, " ");
    strcat(street, name);

    free(name);

    // Generate street number, add it to address
    char *number = calloc(5, sizeof(char));
    sprintf(number, "%d", generate_int(1, 9999));

    strcat(street, " ");
    strcat(street, number);

    free(number);
    return street;
}

char *generate_name() {
    // Open first and last names list
    FILE *first_names = fopen(FIRST_NAME_LIST, "re");
    if (first_names == NULL) {
        fprintf(stderr, "File %s failed to open. Please verify your CWD.\n", FIRST_NAME_LIST);
        exit(EXIT_FAILURE);
    }
    FILE *last_names = fopen(LAST_NAME_LIST, "re");
    if (last_names == NULL) {
        fprintf(stderr, "File %s failed to open. Please verify your CWD.\n", LAST_NAME_LIST);
        exit(EXIT_FAILURE);
    }

    // Pick random first and last names
    char *first = get_line(first_names, generate_int(1, count_lines(first_names)));
    char *last = get_line(last_names, generate_int(1, count_lines(last_names)));
    fclose(first_names);
    fclose(last_names);

    size_t sz = strlen(first) + strlen(last) + 2;
    //          ^ First name    ^ Last name    ^ Spaces

    char *name = (char *) calloc(sz, sizeof(char));
    strncpy(name, first, sz * sizeof(char));

    strcat(name, " ");
    strcat(name, last);

    free(first);
    free(last);

    return name;
}

void lower_str(char *str) {
    for (size_t i = 0; i < strlen(str); ++i) {
        str[i] = (char) tolower(str[i]);
    }
}


/*
 * split_str - Splits a string on delimiters
 * @param str Input string to be split
 * @param delim Delimiter to split at
 * @param n Number of tokens split
 * @return Array of tokens
 */
char **split_str(char *str, const char delim, size_t *n) {
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
    if (dup == NULL) {
        free(tokens);
        return 0;
    }

    char *found = NULL;
    delim_count = 0;
    while ((found = strsep(&dup, &delim)) != NULL) {
        tokens[delim_count] = calloc(strlen(found) + 1, sizeof(char));
        strcpy(tokens[delim_count], found);
        ++delim_count;
    }
    free(dup);
    *n = delim_count;

    return tokens;
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

    size_t name_count;
    char **tokens = split_str(name, ' ', &name_count);

    size_t email_size = 1;
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

    if (use_dash && use_dob) {
        email_size += 1;
    }

    char *email = calloc(email_size, sizeof(char));

    shuffle(tokens, name_count);

    for (size_t i = 0; i < name_count; ++i) {
        lower_str(tokens[i]);
        strcat(email, tokens[i]);
        free(tokens[i]);
        if ((use_dash && i < name_count - 1) || (use_dash && use_dob)) {
            strcat(email, "-");
        }
    }
    free(tokens);
    if (use_dob) {
        strcat(email, year);
        free(year);
    }
    strcat(email, provider);
    fflush(stdout);
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
    printf("-------------------------\n");
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
