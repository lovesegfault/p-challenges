//
// Created by bemeurer on 3/1/18.
//

#ifndef INC_05_ENC_DEC_FIFO_H
#define INC_05_ENC_DEC_FIFO_H
#include <stdlib.h>
#include <stdint.h>

typedef struct NODE {
    struct NODE *prev;
    struct NODE *next;
    uint8_t *data;
} node_t;

typedef struct FIFO {
    node_t *first;
    node_t *last;

    void (*enqueue)(struct FIFO*, uint8_t*);
    uint8_t* (*dequeue)(struct FIFO*);
    void (*free)(struct FIFO**);
} fifo_t;

fifo_t *fifo_init();
void fifo_enqueue(fifo_t *queue, uint8_t *data);
uint8_t *fifo_dequeue(fifo_t *queue);
void fifo_free(fifo_t **queue);

#endif //INC_05_ENC_DEC_FIFO_H
