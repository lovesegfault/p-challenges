#include "base64.h"

static const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(uint8_t *buf[3]){

    return NULL;
}

void *process_loop(void *arg){
    struct thread_bus *bus = (struct thread_bus*)(arg);
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