#ifndef INC_05_ENC_DEC_FIFO_H
#define INC_05_ENC_DEC_FIFO_H
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <jemalloc/jemalloc.h>

struct list_node {
    struct list_node *prev;
    struct list_node *next;
    uint8_t *data;
};

typedef struct FIFO {
    struct list_node *first;
    struct list_node *last;

    size_t count;
    pthread_mutex_t *mutex;

    size_t (*count_mutex)(struct FIFO*);
    void (*enqueue)(struct FIFO*, uint8_t*);
    uint8_t* (*dequeue)(struct FIFO*);
    void (*free)(struct FIFO**, bool);
} fifo_t;

struct thread_bus {
    fifo_t *input;
    fifo_t *output;
    bool kill;
};

fifo_t *fifo_init();
void fifo_enqueue(fifo_t *queue, uint8_t *data);
uint8_t *fifo_dequeue(fifo_t *queue);
void fifo_free(fifo_t **queue, bool free_data);
size_t fifo_count(fifo_t *queue);

#ifndef TESTS_DISABLED

#include <stdio.h>

bool test_fifo_initialize();

bool test_fifo_debug_count();

bool test_fifo_empty_enqueue();

bool test_fifo_empty_dequeue();

bool test_fifo_empty_free();

bool test_fifo_singleton_dequeue();

bool test_fifo_multiple_enqueues();

bool test_fifo_multiple_dequeues();

bool test_fifo_free();

bool test_fifo_multithreading();

#endif
#endif //INC_05_ENC_DEC_FIFO_H
