#include "output.h"

void *output_file_loop(void *arg){
    bus_t *bus = (bus_t*)(arg);
    fifo_t *fifo = bus->output;
    FILE *out = fopen("out.txt", "aeb+");
    if(out == NULL) {
        bus->kill = true;
        return NULL;
    }

    while (true) {
        if(bus->kill)
            break;
        if(fifo->count > 0){
            char *c = (char*)(fifo->dequeue(fifo));
            fprintf(out, "%c", *c);
            free(c);
            fflush(out);
        }
    }
    fclose(out);
    return NULL;
}