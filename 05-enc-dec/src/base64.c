#include "base64.h"
#include "fifo.h"

void *process_loop(void *arg){
    bus_t *bus = (bus_t*)(arg);
    uint8_t **buf = calloc(3, sizeof(uint8_t));
    while (true) {
        if(bus->kill){
            break;
        }

        if(bus->input->count(bus->input) >= 3 && !bus->kill) {
           // fifo_debug_print(bus->input);
            for(int i = 0; i < 3; ++i)
                bus->output->enqueue(bus->output, bus->input->dequeue(bus->input));
        }
    }
    free(buf);
    return NULL;
}