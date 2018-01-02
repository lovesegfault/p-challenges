#include "name-generator.h"

#define STREET_LIST  "lists/street_names.txt"
#define FIRST_NAME_LIST  "lists/first_names.txt"
#define LAST_NAME_LIST  "lists/last_names.txt"

unsigned int random_uint() {
    unsigned int r_uint;
    FILE *f;

    f = fopen("/dev/urandom", "r");
    fread(&r_uint, sizeof(r_uint), 1, f);
    fclose(f);

    return r_uint;
}

// Inclusive range
// https://stackoverflow.com/a/17554531/2080712
unsigned int generate_int(unsigned int lower, unsigned int upper) {
    int r_uint;
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

    return lower + (r_uint / buckets);
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

struct tm generate_DOB() {
    struct tm DOB = {};

    time_t c_time = time(NULL);
    struct tm *current = localtime(&c_time);

    DOB.tm_year = generate_int(0, (unsigned int) current->tm_year);

    if (DOB.tm_year == (unsigned int) current->tm_year) {
        DOB.tm_mon = generate_int(1, (unsigned int) current->tm_mon);
    } else {
        DOB.tm_mon = generate_int(1, 12);
    }

    if (DOB.tm_mon == (unsigned int) current->tm_mon) {
        DOB.tm_mday = generate_int(1, (unsigned int) current->tm_mday);
    } else {
        DOB.tm_mday = generate_int(1, month_days(DOB.tm_mon, DOB.tm_year));
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

    unsigned int pick = generate_int(1, count_lines(name_list));
    char *name = get_line(name_list, pick);

    const char *suf = suffixes[generate_int(0, 7)];
    size_t sz = (strlen(name) + strlen(suf) + 2);
    char *street = calloc(sz, sizeof(char));
    strncpy(street, suf, sz * sizeof(char));

    strcat(street, " ");
    strcat(street, name);

    free(name);
    fclose(name_list);
    return street;
}

char *generate_name() {
    FILE *first_names = fopen(FIRST_NAME_LIST, "r");
    FILE *last_names = fopen(LAST_NAME_LIST, "r");

    char* first = get_line(first_names, generate_int(1, count_lines(first_names)));
    char* last = get_line(last_names, generate_int(1, count_lines(last_names)));

    size_t sz = strlen(first) + strlen(last) + 2;

    char *name = (char*)calloc(sz, sizeof(char));
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
    char *out = (char*) calloc(strlen(str), sizeof(char));
    for(int i = 0; i < strlen(str); ++i){
        out[i] = (char) tolower(str[i]);
    }
    return out;
}

char* generate_email(char* name) {
    char *track;
   // char *names[];
    track = strtok(name, " ");
}

person generate_person() {
    person p = {};
    p.address = generate_address();
    p.DOB = generate_DOB();
    //p.email
    p.name = generate_name();
    p.SSN = generate_SSN();

    return p;
}