#include "input.h"

#define KEY_EOT 4
#define KEY_ESC 27
#define KEY_LF 10

WINDOW *create_win(int height, int width, int y, int x) {
    WINDOW *local_win;

    local_win = newwin(height, width, y, x);
    box(local_win, 0, 0);
    wrefresh(local_win);

    return local_win;
}

void destroy_win(WINDOW *win) {
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

void draw() {

}


void *stdin_window(void *arg) {
    bus_t *bus = (bus_t *) (arg);
    fifo_t *fifo = bus->input;

    initscr();
    cbreak();
    noecho();

    WINDOW *input_win;
    int startx, starty, width, height;
    height = LINES;
    width = (COLS / 2) - 1;
    starty = 0;    /* Calculating for a center placement */
    startx = 0;    /* of the window		*/
    input_win = create_win(height, width, starty, startx);
    wmove(input_win, 1, 1);
    int ch;
    while ((ch = wgetch(input_win))) {
        if (ch == KEY_EOT || ch == KEY_ESC)
            break;
        if ((ch >= 0 && ch <= 31 && ch != KEY_LF))
            continue;
        wprintw(input_win, "%c", ch);
        uint8_t *byte = calloc(1, sizeof(uint8_t));
        *byte = (uint8_t) (ch);
        fifo->enqueue(fifo, byte);
        //refresh();
        box(input_win, 0, 0);
        wrefresh(input_win);
    }
    bus->kill = true;
    endwin();

    return NULL;

}

void *stdin_loop(void *arg) {
    bus_t *bus = (bus_t *) (arg);
    fifo_t *fifo = bus->input;
    initscr();
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
        *byte = (uint8_t) (ch);
        fifo->enqueue(fifo, byte);
        refresh();

    }
    bus->kill = true;
    endwin();
    return (NULL);
}

