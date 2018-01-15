//
// Created by bemeurer on 1/15/18.
//

#include "heads-tails.h"

static bool HAS_URANDOM = true; // Global

unsigned int random_uint() {
    unsigned int r_uint;

    FILE *f = fopen("/dev/urandom", "r");
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

bool rand_bool() {
    return (bool) (random_uint() & 1);
}

void play_init(play *self) {
    self->play = heads_or_tails;
}

void heads_or_tails(play *self) {
    if (rand_bool()) {
        self->result = heads;
    } else {
        self->result = tails;
    }
}

void lower_str(char *str) {
    for (size_t i = 0; i < strlen(str); ++i) {
        str[i] = (char) tolower(str[i]);
    }
}


void game() {
    play p;
    play_init(&p);
    printf("Heads or Tails:\n");
    bool valid_pick = false;
    bool keep_playing = true;
    char *input = NULL;
    size_t n = 0;
    face *choice = NULL;
    while (keep_playing) {
        while (!valid_pick) {
            printf("What's your pick: ");
            getline(&input, &n, stdin);
            lower_str(input);
            if ((strlen(input) > 6) || ((strncmp(input, "heads", 5) != 0) && (strncmp(input, "tails", 5)) != 0)) {
                free(input);
                printf("\r");
                continue;
            } else {
                valid_pick = true;
            }
            choice = calloc(1, sizeof(face));
            if (strncmp(input, "heads", 5) == 0) {
                *choice = heads;
            } else {
                *choice = tails;
            }
            free(input);
            input = NULL;
            n = 0;
        }

        p.play(&p);

        if (p.result == *choice) {
            printf("You won!\n");
        } else {
            printf("You lost :(\n");

        }
        printf("Play again? [Y/n] ");
        getline(&input, &n, stdin);
        lower_str(input);
        if (strncmp(input, "n", 1) == 0){
            keep_playing = false;
        } else {
            valid_pick = false;
        }
    }
    free(choice);
}

