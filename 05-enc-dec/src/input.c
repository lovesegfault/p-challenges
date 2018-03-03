#include "input.h"

#define KEY_EOT 4
#define KEY_ESC 27
#define KEY_LF 10

void *stdin_mode(fifo_t *fifo) {
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    int ch;
    while((ch = getch())) {
        if (ch == KEY_EOT || ch == KEY_ESC)
            break;
        if ((ch >= 0 && ch <=31 && ch != KEY_LF))
            continue;
        printw("%c", ch);

        refresh();
    }
    endwin();
}