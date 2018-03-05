#include "fifo.h"

/*
 * Initializes a FIFO queue
 */
fifo_t *fifo_init() {
    fifo_t *queue = calloc(1, sizeof(fifo_t)); // Allocate struct
    queue->last = NULL;
    queue->first = NULL;
    queue->mutex = calloc(1, sizeof(pthread_mutex_t)); // Allocate mutex
    pthread_mutex_init(queue->mutex, NULL); // Initialize the mutex, no attributes needed
    queue->count = 0; // To allow for O(1) counting

    // Function pointers
    queue->count_mutex = fifo_count;
    queue->enqueue = fifo_enqueue;
    queue->dequeue = fifo_dequeue;
    queue->free = fifo_free;

    return queue;
}

/*
 * Enqueues into FIFO
 */
void fifo_enqueue(fifo_t *queue, uint8_t *data) {
    pthread_mutex_lock(queue->mutex); // Lock
    // Allocate the node
    node_t *new = calloc(1, sizeof(node_t));
    new->data = data; // Link data

    // If we are enqueuing on an empty list, set first and last to be the singleton node
    if (queue->first == NULL) {
        queue->first = queue->last = new;
        ++queue->count;
        pthread_mutex_unlock(queue->mutex);
        return;
    }

    // Attach node
    queue->last->next = new;
    new->prev = queue->last;

    queue->last = new;
     ++queue->count; // Increment element count

    pthread_mutex_unlock(queue->mutex); // Unlock
}

/*
 * Dequeue from FIFO
 */
uint8_t *fifo_dequeue(fifo_t *queue) {
    pthread_mutex_lock(queue->mutex); // Lock
    // Dequeueing on an empty FIFO returns NULL
    if (queue->first == NULL) {
        pthread_mutex_unlock(queue->mutex);
        return NULL;
    }

    // Detach node
    node_t *first = queue->first;
    queue->first = first->next;
    first->prev = NULL;

    // Save data pointer, free node
    uint8_t *data = first->data;
    free(first);

    --queue->count; // Decrement element count
    pthread_mutex_unlock(queue->mutex); // Unlock

    return data;
}

/*
 * Free FIFO
 */
void fifo_free(fifo_t **queue, bool free_data) {
    pthread_mutex_lock((*queue)->mutex); // Lock

    // Iterate over FIFO, freeing nodes
    node_t *index;
    while ((index = (*queue)->first) != NULL) {
        (*queue)->first = (*queue)->first->next;
        // Optional data freeing
        if (free_data) {
            free(index->data);
            index->data = NULL;
        }
        free(index);
    }
    // Clean mutex
    pthread_mutex_unlock((*queue)->mutex);
    pthread_mutex_destroy(((*queue)->mutex));
    free((*queue)->mutex);
    // Free structure
    free(*queue);
    *queue = NULL;
}

size_t fifo_count(fifo_t *queue) {
    pthread_mutex_lock(queue->mutex); // Lock
    size_t res = queue->count; // Save count
    pthread_mutex_unlock(queue->mutex); // Unlock
    return res;
}

size_t fifo_debug_count(fifo_t *queue) {
    pthread_mutex_lock(queue->mutex); // Lock
    size_t count = 0;
    node_t *index = queue->first;
    while (index != NULL) {
        ++count;
        index = index->prev;
    }
    pthread_mutex_unlock(queue->mutex);
    return count;
}

void fifo_debug_print(fifo_t *queue) {
    pthread_mutex_lock(queue->mutex); // Lock
    node_t *idx = queue->first;
    while (idx != NULL) {
        if(idx->prev)
            printf("<-");
        printf(" [%c] ", (char)*(idx->data));
        if(idx->next)
            printf("->");
    }
    printf("\n");
    pthread_mutex_unlock(queue->mutex); // Unlock
}

/*
 * Tests
 */

#ifndef TESTS_DISABLED
#define SAMPLE_SIZE 10000000

bool test_fifo_initialize() {
    fifo_t *fifo = fifo_init();
    bool test_result = fifo != NULL;
    fifo->free(&fifo, true);
    return test_result;
}

bool test_fifo_debug_count() {
    fifo_t *fifo = fifo_init();
    size_t count = fifo_debug_count(fifo);
    fifo->free(&fifo, false);
    return count == 0;
}

bool test_fifo_empty_enqueue() {
    uint8_t *byte = calloc(1, sizeof(uint8_t));
    if (byte == NULL)
        return false;
    *byte = 127;

    fifo_t *fifo = fifo_init();
    fifo->enqueue(fifo, byte);

    size_t count = fifo->count;

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
    if (byte == NULL)
        return false;
    *byte = 253;

    fifo->enqueue(fifo, byte);
    uint8_t data = *fifo->dequeue(fifo);
    size_t count = fifo->count;
    fifo->free(&fifo, false);
    free(byte);
    return (count == 0) && (data == 253);
}

bool test_fifo_multiple_enqueues() {
    fifo_t *fifo = fifo_init();
    uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
    if (bytes == NULL)
        return false;
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
        test_result &= fifo->count == SAMPLE_SIZE;
    }

    free(bytes);
    fifo->free(&fifo, false);

    return test_result;
}

bool test_fifo_multiple_dequeues() {
    fifo_t *fifo = fifo_init();
    uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
    if (bytes == NULL)
        return false;

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
        test_result &= (fifo->count == 0);
    }

    free(bytes);
    fifo->free(&fifo, false);

    return test_result;
}

bool test_fifo_free() {
    fifo_t *fifo = fifo_init();
    uint8_t **bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t*));
    if (bytes == NULL)
        return false;

    for (size_t i = 0; i < SAMPLE_SIZE; ++i) {
        bytes[i] = calloc(1, sizeof(uint8_t));
        *(bytes[i]) = (uint8_t) (i % (2 << 7));
        fifo->enqueue(fifo, bytes[i]);
    }

    bool test_result = true;

    fifo->free(&fifo, true);
    test_result &= fifo == NULL;
    free(bytes);
    return test_result;
}
#endif