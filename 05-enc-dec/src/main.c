#include "input.h"
#include "fifo.h"
#include <pthread.h>

int main(int argc, char **argv) {
    // Create input thread
    pthread_t input;
    pthread_create(&input, NULL, stdin_mode, NULL);

    // Input FIFO
    fifo_t *in_fifo = fifo_init();

    // Process thread

    // Output FIFO
    fifo_t *out_fifo = fifo_init();

    // Output thread
    return 0;
}

