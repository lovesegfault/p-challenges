#include "base64.h"

void *process_loop(void *arg){
    bus_t *bus = (bus_t*)(arg);
    uint8_t **buf = calloc(3, sizeof(uint8_t*));
    while (true) {
        if(bus->kill){
            break;
        }

        if(bus->input->count >= 3 && !bus->kill) {
            for(int i = 0; i < 3; ++i) {
                buf[i] = bus->input->dequeue(bus->input);
                bus->output->enqueue(bus->output, buf[i]);
            }
        }
    }
    free(buf);
    return NULL;
}