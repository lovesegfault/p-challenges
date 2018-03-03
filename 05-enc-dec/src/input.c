#include "input.h"
#include "fifo.h"

#define KEY_EOT 4
#define KEY_ESC 27
#define KEY_LF 10

void *stdin_loop(void *queue) {
    bus_t *bus = (bus_t*)(queue);
    fifo_t *fifo = bus->input;
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    uint8_t *ch = calloc(1, sizeof(uint8_t));
    while((*ch = (uint8_t)getch())) {
        if (*ch == KEY_EOT || *ch == KEY_ESC)
            break;
        if ((*ch >= 0 && *ch <=31 && *ch != KEY_LF))
            continue;
        printw("%c", *ch);
        fifo->enqueue(fifo, ch);
        refresh();

    }
    bus->kill = true;
    endwin();
    return (NULL);
}