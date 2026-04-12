/*
Command to run (from project root):
make test
Or directly:(from project root):
gcc -Wall -Wextra -g -o _test_heap tests/testMinHeap.c src/minHeap.c -I./src 
./_test_heap
*/

#include <assert.h>
#include <stdio.h>
#include "minHeap.h"

static void test_create_invalid_capacity(void) {
    printf("Test 1: create with invalid capacity\n");
    assert(minHeapCreate(0) == NULL);
    assert(minHeapCreate(-5) == NULL);
    printf("Test 1 passed\n\n");
}

static void test_create_and_initial_state(void) {
    printf("Test 2: create and initial state\n");
    minHeap *heap = minHeapCreate(5);
    assert(heap != NULL);
    assert(heap->capacity == 5);
    assert(heap->size == 0);
    
    // All positions should be -1 initially
    for (int i = 0; i < 5; i++) {
        assert(heap->pos[i] == -1);
    }
    
    minHeapFree(heap);
    printf("Test 2 passed\n\n");
}

static void test_is_empty(void) {
    printf("Test 3: is_empty check\n");
    minHeap *heap = minHeapCreate(5);
    assert(heap != NULL);
    
    // Should be empty initially
    assert(minHeapIsEmpty(heap) == 1);
    
    minHeapFree(heap);
    printf("Test 3 passed\n\n");
}

static void test_contains(void) {
    printf("Test 4: contains check\n");
    minHeap *heap = minHeapCreate(5);
    assert(heap != NULL);
    
    // No nodes should be in heap initially
    for (int i = 0; i < 5; i++) {
        assert(minHeapContains(heap, i) == 0);
    }
    
    minHeapFree(heap);
    printf("Test 4 passed\n\n");
}

static void test_insert_single_node(void) {
    printf("Test 5: insert single node\n");
    minHeap *heap = minHeapCreate(5);
    assert(heap != NULL);
    
    minHeapInsert(heap, 0, 10);
    
    assert(minHeapIsEmpty(heap) == 0);
    assert(minHeapContains(heap, 0) == 1);
    assert(heap->size == 1);
    assert(heap->data[0].node == 0);
    assert(heap->data[0].weight == 10);
    
    minHeapFree(heap);
    printf("Test 5 passed\n\n");
}

static void test_insert_and_extract_min(void) {
    printf("Test 6: insert and extract min\n");
    minHeap *heap = minHeapCreate(5);
    assert(heap != NULL);
    
    minHeapInsert(heap, 0, 10);
    minHeapInsert(heap, 1, 5);
    minHeapInsert(heap, 2, 15);
    
    assert(heap->size == 3);
    
    // Extract minimum
    heapNode min = minHeapExtractMin(heap);
    assert(min.node == 1);
    assert(min.weight == 5);
    assert(heap->size == 2);
    assert(minHeapContains(heap, 1) == 0);
    
    minHeapFree(heap);
    printf("Test 6 passed\n\n");
}

static void test_min_heap_property(void) {
    printf("Test 7: min heap property maintained\n");
    minHeap *heap = minHeapCreate(10);
    assert(heap != NULL);
    
    // Insert in random order
    minHeapInsert(heap, 0, 50);
    minHeapInsert(heap, 1, 30);
    minHeapInsert(heap, 2, 20);
    minHeapInsert(heap, 3, 10);
    minHeapInsert(heap, 4, 40);
    
    // Extract all and verify order
    heapNode n1 = minHeapExtractMin(heap);
    assert(n1.weight == 10);
    
    heapNode n2 = minHeapExtractMin(heap);
    assert(n2.weight == 20);
    
    heapNode n3 = minHeapExtractMin(heap);
    assert(n3.weight == 30);
    
    heapNode n4 = minHeapExtractMin(heap);
    assert(n4.weight == 40);
    
    heapNode n5 = minHeapExtractMin(heap);
    assert(n5.weight == 50);
    
    assert(heap->size == 0);
    
    minHeapFree(heap);
    printf("Test 7 passed\n\n");
}

static void test_extract_from_empty_heap(void) {
    printf("Test 8: extract from empty heap\n");
    minHeap *heap = minHeapCreate(5);
    assert(heap != NULL);
    
    heapNode result = minHeapExtractMin(heap);
    assert(result.node == -1);
    assert(result.weight == -1);
    
    minHeapFree(heap);
    printf("Test 8 passed\n\n");
}

static void test_insert_capacity_overflow(void) {
    printf("Test 9: insert capacity overflow\n");
    minHeap *heap = minHeapCreate(5);  // Larger capacity for pos array
    assert(heap != NULL);
    
    minHeapInsert(heap, 0, 10);
    minHeapInsert(heap, 1, 20);
    
    // Manually set size to capacity to simulate full heap
    heap->size = 5;
    
    // Try to insert beyond capacity - should be ignored
    minHeapInsert(heap, 2, 30);
    
    assert(heap->size == 5);  // Should still be 5 (full)
    assert(minHeapContains(heap, 2) == 0);  // Node 2 should not be inserted
    
    minHeapFree(heap);
    printf("Test 9 passed\n\n");
}

static void test_decrease_key(void) {
    printf("Test 10: decrease key\n");
    minHeap *heap = minHeapCreate(10);
    assert(heap != NULL);
    
    minHeapInsert(heap, 0, 50);
    minHeapInsert(heap, 1, 30);
    minHeapInsert(heap, 2, 20);
    
    // Decrease weight of node 0 from 50 to 5
    minHeapDecreaseKey(heap, 0, 5);
    
    // Node 0 should now be the minimum
    heapNode min = minHeapExtractMin(heap);
    assert(min.node == 0);
    assert(min.weight == 5);
    
    minHeapFree(heap);
    printf("Test 10 passed\n\n");
}

static void test_decrease_key_nonexistent_node(void) {
    printf("Test 11: decrease key on non-existent node\n");
    minHeap *heap = minHeapCreate(5);
    assert(heap != NULL);
    
    minHeapInsert(heap, 0, 10);
    
    // Try to decrease key of node not in heap - should be ignored
    minHeapDecreaseKey(heap, 1, 5);
    
    assert(heap->size == 1);
    assert(minHeapContains(heap, 1) == 0);
    
    minHeapFree(heap);
    printf("Test 11 passed\n\n");
}

static void test_decrease_key_to_higher_value(void) {
    printf("Test 12: decrease key to higher value (should be ignored)\n");
    minHeap *heap = minHeapCreate(5);
    assert(heap != NULL);
    
    minHeapInsert(heap, 0, 10);
    
    // Try to decrease to a higher value - should be ignored
    minHeapDecreaseKey(heap, 0, 20);
    
    assert(heap->data[0].weight == 10);  // Weight should not change
    
    minHeapFree(heap);
    printf("Test 12 passed\n\n");
}

static void test_free_null(void) {
    printf("Test 13: free NULL safety\n");
    minHeapFree(NULL);
    printf("Test 13 passed\n\n");
}

static void test_large_heap_operations(void) {
    printf("Test 14: large heap operations\n");
    minHeap *heap = minHeapCreate(100);
    assert(heap != NULL);
    
    // Insert 100 elements with descending weights
    for (int i = 0; i < 100; i++) {
        minHeapInsert(heap, i, 100 - i);
    }
    
    assert(heap->size == 100);
    
    // Extract all and verify order
    for (int i = 0; i < 100; i++) {
        heapNode node = minHeapExtractMin(heap);
        assert(node.weight == i + 1);  // Should be in ascending order
    }
    
    assert(heap->size == 0);
    
    minHeapFree(heap);
    printf("Test 14 passed\n\n");
}

static void test_mixed_operations(void) {
    printf("Test 15: mixed operations (insert, extract, decrease)\n");
    minHeap *heap = minHeapCreate(20);
    assert(heap != NULL);
    
    // Insert some nodes
    minHeapInsert(heap, 0, 100);
    minHeapInsert(heap, 1, 50);
    minHeapInsert(heap, 2, 75);
    minHeapInsert(heap, 3, 25);
    
    // Extract minimum
    heapNode n1 = minHeapExtractMin(heap);
    assert(n1.weight == 25);
    
    // Decrease key of another node
    minHeapDecreaseKey(heap, 0, 20);
    
    // Insert new node
    minHeapInsert(heap, 4, 60);
    
    // Extract minimum again
    heapNode n2 = minHeapExtractMin(heap);
    assert(n2.weight == 20);
    
    minHeapFree(heap);
    printf("Test 15 passed\n\n");
}

static void test_position_tracking(void) {
    printf("Test 16: position tracking in pos array\n");
    minHeap *heap = minHeapCreate(10);
    assert(heap != NULL);
    
    minHeapInsert(heap, 5, 10);
    assert(heap->pos[5] == 0);  // Node 5 should be at index 0
    
    minHeapInsert(heap, 3, 5);
    assert(heap->pos[3] == 0);  // Node 3 should be at index 0 after heapify
    assert(heap->pos[5] == 1);  // Node 5 should move to index 1
    
    heapNode min = minHeapExtractMin(heap);
    assert(min.node == 3);
    assert(heap->pos[3] == -1);  // Node 3 should be marked as -1 after extraction
    assert(heap->pos[5] == 0);   // Node 5 should move to index 0
    
    minHeapFree(heap);
    printf("Test 16 passed\n\n");
}

static void test_dijkstra_like_scenario(void) {
    printf("Test 17: dijkstra-like usage scenario\n");
    minHeap *heap = minHeapCreate(5);
    assert(heap != NULL);
    
    // Simulate Dijkstra's algorithm
    // Insert nodes with initial distances
    minHeapInsert(heap, 0, 0);      // Starting node
    minHeapInsert(heap, 1, 4);
    minHeapInsert(heap, 2, 2);
    minHeapInsert(heap, 3, 5);
    
    // Process minimum
    heapNode current = minHeapExtractMin(heap);
    assert(current.node == 0 && current.weight == 0);
    
    // Update distances via decrease key
    minHeapDecreaseKey(heap, 1, 2);  // Found shorter path to node 1
    minHeapDecreaseKey(heap, 3, 3);  // Found shorter path to node 3
    
    // Process next minimum
    current = minHeapExtractMin(heap);
    assert(current.weight == 2);  // Should be the updated value
    
    minHeapFree(heap);
    printf("Test 17 passed\n\n");
}

int main(void) {
    printf("=== Running Min-Heap Tests ===\n\n");
    
    test_create_invalid_capacity();
    test_create_and_initial_state();
    test_is_empty();
    test_contains();
    test_insert_single_node();
    test_insert_and_extract_min();
    test_min_heap_property();
    test_extract_from_empty_heap();
    test_insert_capacity_overflow();
    test_decrease_key();
    test_decrease_key_nonexistent_node();
    test_decrease_key_to_higher_value();
    test_free_null();
    test_large_heap_operations();
    test_mixed_operations();
    test_position_tracking();
    test_dijkstra_like_scenario();
    
    printf("=== All Min-Heap tests passed! ===\n");
    return 0;
}
