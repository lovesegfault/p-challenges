//
// Created by bemeurer on 1/15/18.
//

#ifndef INC_01_HEADS_TAILS_HEADS_TAILS_H
#define INC_01_HEADS_TAILS_HEADS_TAILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    heads,
    tails,
} face;

typedef struct _play {
    face result;

    void (*play)(struct _play *);
} play;

void play_init(play *self);

void heads_or_tails(play *self);

void game();

#endif //INC_01_HEADS_TAILS_HEADS_TAILS_H
