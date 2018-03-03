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
        if (free_data) {
            free(current->data);
            current->data = NULL;
        }
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

/*
 * Tests
 */

#ifndef TESTS_DISABLED
#define SAMPLE_SIZE 1000000

bool test_fifo_initialize() {
    fifo_t *fifo = fifo_init();
    bool test_result = fifo != NULL;
    fifo->free(&fifo, true);
    return test_result;
}

bool test_fifo_count() {
    fifo_t *fifo = fifo_init();
    size_t count = fifo->count(fifo);
    fifo->free(&fifo, false);
    return count == 0;
}

bool test_fifo_empty_enqueue() {
    uint8_t *byte = calloc(1, sizeof(uint8_t));
    *byte = 127;


    fifo_t *fifo = fifo_init();
    fifo->enqueue(fifo, byte);

    size_t count = fifo->count(fifo);

    bool test_result = (*(fifo->first->data) == *(fifo->last->data));
    test_result &= *fifo->first->data == 127;
    test_result &= count == 1;

    fifo->free(&fifo, false);
    free(byte);

    return test_result;
}

bool test_fifo_empty_dequeue() {
    fifo_t *fifo = fifo_init();
    uint8_t *data = fifo->dequeue(fifo);
    fifo->free(&fifo, false);
    return data == NULL;
}

bool test_fifo_empty_free() {
    fifo_t *fifo = fifo_init();
    fifo->free(&fifo, false);
    return fifo == NULL;
}

bool test_fifo_singleton_dequeue() {
    fifo_t *fifo = fifo_init();
    uint8_t *byte = calloc(1, sizeof(uint8_t));
    *byte = 253;

    fifo->enqueue(fifo, byte);
    uint8_t data = *fifo->dequeue(fifo);
    size_t count = fifo->count(fifo);
    fifo->free(&fifo, false);
    free(byte);
    return (count == 0) && (data == 253);
}

bool test_fifo_multiple_enqueues() {
    fifo_t *fifo = fifo_init();
    uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
    for (size_t i = 0; i < SAMPLE_SIZE; ++i) {
        bytes[i] = (uint8_t) (i % (2 << 7));
        fifo->enqueue(fifo, &(bytes[i]));
    }

    bool test_result = true;

    size_t bytes_idx = 0;
    node_t *fifo_idx = fifo->first;
    while (fifo_idx->prev != NULL) {
        test_result &= (*fifo_idx->data == bytes[bytes_idx]);
        if (!test_result) break;

        fifo_idx = fifo_idx->prev;
        ++bytes_idx;
    }
    if (test_result) {
        test_result &= fifo->count(fifo) == SAMPLE_SIZE;
    }

    free(bytes);
    fifo->free(&fifo, false);

    return test_result;
}

bool test_fifo_multiple_dequeues() {
    fifo_t *fifo = fifo_init();
    uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
    for (size_t i = 0; i < SAMPLE_SIZE; ++i) {
        bytes[i] = (uint8_t) (i % (2 << 7));
        fifo->enqueue(fifo, &(bytes[i]));
    }

    bool test_result = true;

    for (size_t i = 0; i < SAMPLE_SIZE; ++i) {
        uint8_t data = *fifo->dequeue(fifo);
        test_result &= (data == bytes[i]);
    }
    if (test_result) {
        test_result &= (fifo->count(fifo) == 0);
    }

    free(bytes);
    fifo->free(&fifo, false);

    return test_result;
}

bool test_fifo_free() {
    fifo_t *fifo = fifo_init();
    uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
    for (size_t i = 0; i < SAMPLE_SIZE; ++i) {
        bytes[i] = (uint8_t) (i % (2 << 7));
        fifo->enqueue(fifo, &(bytes[i]));
    }

    bool test_result = true;

    fifo->free(&fifo, true);
    test_result &= fifo == NULL;

    return test_result;
}
#endif