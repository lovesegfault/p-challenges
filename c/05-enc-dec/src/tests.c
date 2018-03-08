#include "fifo.h"
#include "../../tests/test.h"

#define RUNS 100

void fifo_tests() {
    section("FIFO");
    struct t_result results = {};
    assert("Initializing FIFO", test_fifo_initialize, &results, RUNS);
    assert("Counting FIFO", test_fifo_debug_count, &results, RUNS);
    assert("Enqueuing empty FIFO", test_fifo_empty_enqueue, &results, RUNS);
    assert("Dequeuing empty FIFO", test_fifo_empty_dequeue, &results, RUNS);
    assert("Freeing empty FIFO", test_fifo_empty_free, &results, RUNS);
    assert("Dequeuing Singleton FIFO", test_fifo_singleton_dequeue, &results, RUNS);
    assert("Multiple enqueues", test_fifo_multiple_enqueues, &results, RUNS);
    assert("Multiple dequeues", test_fifo_multiple_dequeues, &results, RUNS);
    assert("Freeing FIFO", test_fifo_free, &results, RUNS);
    assert("Multithreading", test_fifo_multithreading, &results, RUNS);
    assert_section("FIFO", &results);
}

void base64_tests() {
    section("Base64");
    //result_t results = {};
}

int main() {
    fifo_tests();
    base64_tests();
}