#ifndef INC_05_ENC_DEC_INPUT_H
#define INC_05_ENC_DEC_INPUT_H

#include <stdbool.h>
#include <ncurses.h>
#include "fifo.h"

void *stdin_loop(void *arg);
void *stdin_window(void *arg);
void *stdin_termbox(void *arg);

#endif //INC_05_ENC_DEC_INPUT_H
