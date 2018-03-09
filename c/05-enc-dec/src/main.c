#include <pthread.h>
#include "fifo.h"
#include "input.h"
#include "base64.h"
#include "output.h"

int main(int argc, char **argv) {
    // FIFO's
    struct thread_bus *bus = calloc(1, sizeof(struct thread_bus));
    bus->input = fifo_init();
    bus->output = fifo_init();

    // Create input thread
    pthread_t input;
    pthread_create(&input, NULL, stdin_window, bus);

    // Process thread
    pthread_t process;
    pthread_create(&process, NULL, process_loop, bus);

    // Output thread
    pthread_t output;
    pthread_create(&output, NULL, output_file_loop, bus);

    pthread_join(input, NULL);
    pthread_join(process, NULL);
    pthread_join(output, NULL);

    bus->input->free(&(bus->input), true);
    bus->output->free(&(bus->output), true);
    free(bus);
    return 0;
}

