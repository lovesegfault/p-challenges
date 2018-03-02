//
// Created by bemeurer on 3/1/18.
//

#include "fifo.h"

fifo_t *fifo_init() {
    fifo_t *queue = calloc(1, sizeof(fifo_t));
    queue->last = NULL;
    queue->first = NULL;

    queue->enqueue = fifo_enqueue;
    queue->dequeue = fifo_dequeue;
    queue->free = fifo_free;
    return queue;
}

void fifo_enqueue(fifo_t *queue, uint8_t *data) {
    // Allocate the node
    node_t *new_elem = calloc(1, sizeof(node_t));
    new_elem->data = data; // Link data
    new_elem->next = queue->last; // It is inserted last in the list

    queue->last = new_elem;

    // If we are enqueuing on an empty list, set first and last to be the singleton node
    // FIXME: Is this correct / desired?
    if (queue->first == NULL) {
        queue->first = queue->last;
    }
}

uint8_t *fifo_dequeue(fifo_t *queue) {
    if(queue->first == NULL) {
        return NULL;
    }

    node_t *first = queue->first;
    queue->first = queue->first->prev;
    uint8_t *data = first->data;
    free(first);
    return data;
}

void fifo_free(fifo_t **queue) {
    if (!(*queue)->first) return;

    node_t *current;
    while ((current = (*queue)->first) != NULL) {
        (*queue)->first = (*queue)->first->next;
        free(current->data);
        free(current);
    }
    free(*queue);
}