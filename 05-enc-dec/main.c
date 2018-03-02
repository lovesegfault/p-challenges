#include <stdio.h>
#include <stdbool.h>
#include "fifo.h"

#define SAMPLE_SIZE 10000

int main(int argc, char **argv) {
    {
        printf("Initializing FIFO : ");
        fifo_t *fifo = fifo_init();
        if (fifo != NULL) {
            fifo->free(&fifo, true);
            printf("OK\n");
        }
        else
            printf("FAIL\n");
    }

    {
        printf("Counting empty FIFO : ");
        fifo_t *fifo = fifo_init();
        size_t count = fifo->count(fifo);
        if (count == 0)
            printf("OK\n");
        else
            printf("FAIL : (count = %zu)\n", count);
        fifo->free(&fifo, true);
    }

    {
        printf("Enqueueing on empty FIFO : ");

        uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
        for(size_t i = 0; i < SAMPLE_SIZE; ++i) {
            bytes[i] = (uint8_t)(i % (2 << 7));
        }

        fifo_t *fifo = fifo_init();
        fifo->enqueue(fifo, &(bytes[0]));
        size_t count = fifo->count(fifo);
        if (count == 1)
            printf("OK\n");
        else
            printf("FAIL : (count=%zu)\n", count);
        fifo->free(&fifo, false);
        free(bytes);
    }

    {
        printf("Dequeuing on empty FIFO : ");
        fifo_t *fifo = fifo_init();
        uint8_t *data = fifo->dequeue(fifo);
        if (data == NULL)
            printf("OK\n");
        else
            printf("FAIL : (data = %d)\n", *data);
        fifo->free(&fifo, true);
    }

    {
        printf("Freeing empty FIFO : ");
        fifo_t *fifo = fifo_init();
        fifo->free(&fifo, true);
        if (fifo == NULL)
            printf("OK\n");
        else
            printf("FAIL : (fifo = %p)\n", fifo);
    }

    {
        printf("Dequeuing a singleton : ");
        fifo_t *fifo = fifo_init();

        uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
        for(size_t i = 0; i < SAMPLE_SIZE; ++i) {
            bytes[i] = (uint8_t)(i % (2 << 7));
        }

        fifo->enqueue(fifo, &(bytes[0]));

        uint8_t *data = fifo->dequeue(fifo);

        size_t count = fifo->count(fifo);
        if (*data == 0 && count == 0)
            printf("OK\n");
        else
            printf("FAIL : (data=%d) (count = %zu)\n", *data, count);
        fifo->free(&fifo, true);
        free(bytes);
    }

    {
        printf("Multiple enqueues : ");
        fifo_t *fifo = fifo_init();

        uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
        for(size_t i = 0; i < SAMPLE_SIZE; ++i) {
            bytes[i] = (uint8_t)(i % (2 << 7));
        }

        bool DONE = false;
        for (size_t i = 0; i < SAMPLE_SIZE; ++i) {
            fifo->enqueue(fifo, &(bytes[i]));
        }
        size_t i = 0;
        node_t *idx = fifo->first;
        while (idx->prev != NULL) {
            if (idx->data != &(bytes[i])) {
                printf("FAIL : (i=%zu)(data=%d)(bytes[i]=%d)\n", i, *(idx->data), bytes[i]);
                DONE = true;
                break;
            }
            idx = idx->prev;
            ++i;
        }
        if (!DONE) {
            size_t count = fifo->count(fifo);
            if (count != SAMPLE_SIZE)
                printf("FAIL : (SAMPLE_SIZE=%d)(count=%zu)", SAMPLE_SIZE, count);
            else
                printf("OK\n");
        }
        fifo->free(&fifo, false);
        free(bytes);
    }

    {
        printf("Multiple dequeues : ");
        fifo_t *fifo = fifo_init();

        uint8_t *bytes = calloc(SAMPLE_SIZE, sizeof(uint8_t));
        for(size_t i = 0; i < SAMPLE_SIZE; ++i) {
            bytes[i] = (uint8_t)(i % (2 << 7));
        }

        bool DONE = false;
        for (size_t i = 0; i < SAMPLE_SIZE; ++i) {
            fifo->enqueue(fifo, &(bytes[i]));
        }

        for (size_t i = 0; i < SAMPLE_SIZE; ++i) {
            uint8_t *data = fifo->dequeue(fifo);
            if (*data != bytes[i]) {
                printf("FAIL : (i=%zu)(data=%d)(bytes[i]=%d)\n", i, *data, bytes[i]);
                DONE = true;
                break;
            }
        }
        if (!DONE) {
            size_t count = fifo->count(fifo);
            if(count != 0)
                printf("FAIL : (count=%zu)(expected=0)\n", count);
            else
                printf("OK\n");
        }
        fifo->free(&fifo, true);
        free(bytes);
    }

    return 0;
}

