#include "fifo.h"

fifo_t *fifo_init() {
    fifo_t *queue = calloc(1, sizeof(fifo_t));
    queue->last = NULL;
    queue->first = NULL;

    queue->enqueue = fifo_enqueue;
    queue->dequeue = fifo_dequeue;
    queue->count = fifo_count;
    queue->free = fifo_free;
    return queue;
}

void fifo_enqueue(fifo_t *queue, uint8_t *data) {
    // Allocate the node
    node_t *new_elem = calloc(1, sizeof(node_t));
    new_elem->data = data; // Link data

    // If we are enqueuing on an empty list, set first and last to be the singleton node
    // FIXME: Is this correct / desired?
    if (queue->first == NULL) {
        queue->first = queue->last = new_elem;
        return;
    }
    new_elem->next = queue->last; // It is inserted last in the list
    new_elem->next->prev = new_elem;

    queue->last = new_elem;
}

uint8_t *fifo_dequeue(fifo_t *queue) {
    if (queue->first == NULL) {
        return NULL;
    }

    node_t *first = queue->first;
    queue->first = queue->first->prev;
    uint8_t *data = first->data;
    free(first);
    return data;
}

void fifo_free(fifo_t **queue, bool free_data) {
    node_t *current;
    while ((current = (*queue)->first) != NULL) {
        (*queue)->first = (*queue)->first->next;
        if (free_data) free(current->data);
        free(current);
    }
    free(*queue);
    *queue = NULL;
}

size_t fifo_count(fifo_t *queue) {
    size_t count = 0;
    node_t *current = queue->first;
    while (current != NULL) {
        ++count;
        current = current->prev;
    }
    return count;
}

#ifndef SNOW_DISABLED

#include "../snow/snow/snow.h"

#define SAMPLE_SIZE 10000

#define setup(name, free_data) \
    fifo_t *(name) = fifo_init();\
    defer((name)->free(&(name), (free_data)))


describe(FIFO, {
    it("Initializing FIFO", {
            setup(fifo, false);
            assert(fifo != NULL);
    });
    it("Counting FIFO", {
            setup(fifo, false);
            assert(fifo->count(fifo) == 0);
    });
    it("Enqueing empty FIFO", {
            setup(fifo, false);
            uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
            for (size_t i = 0; i < SAMPLE_SIZE; ++i) {
                bytes[i] = (uint8_t) (i % (2 << 7));
            }
            fifo->enqueue(fifo, &(bytes[0]));
            assert(fifo->count(fifo) == 1);
            assert(*(fifo->first->data) == bytes[0]);
            free(bytes);
    });
    it("Dequeueing empty FIFO", {
            setup(fifo, false);
            assert(fifo->dequeue(fifo) == NULL);
    });
    it("Freeing empty FIFO", {
            fifo_t * fifo = fifo_init();
            fifo->free(&fifo, true);
            assert(fifo == NULL);
    });
    it("Dequeueing singleton", {
            setup(fifo, false);
            uint8_t *byte = calloc(1, sizeof(uint8_t));

            fifo->enqueue(fifo, byte);

            uint8_t *data = fifo->dequeue(fifo);

            assert(*data == *byte);
            assert(fifo->count(fifo) == 0);
    });

});

#endif