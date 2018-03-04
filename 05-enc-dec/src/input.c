#include "input.h"
    
#define KEY_EOT 4
#define KEY_ESC 27
#define KEY_LF 10

void *stdin_loop(void *arg) {
    bus_t *bus = (bus_t *) (arg);
    fifo_t *fifo = bus->input;
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    int ch;
    while ((ch = getch())) {
        if (ch == KEY_EOT || ch == KEY_ESC)
            break;
        if ((ch >= 0 && ch <= 31 && ch != KEY_LF))
            continue;
        printw("%c", ch);
        uint8_t *byte = calloc(1, sizeof(uint8_t));
        *byte = (uint8_t)(ch);
        fifo->enqueue(fifo, byte);
        refresh();

    }
    bus->kill = true;
    endwin();
    return (NULL);
}

