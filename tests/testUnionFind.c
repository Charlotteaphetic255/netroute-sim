/*
Command to run (from tests/):
gcc -o testUnionFind testUnionFind.c ../src/unionFind.c -I./../src
.\testUnionFind.exe
*/

#include <assert.h>
#include <stdio.h>
#include "unionFind.h"

static void test_create_invalid_size(void) {
    printf("Test 1: create with invalid size\n");
    assert(ufCreate(0) == NULL);
    assert(ufCreate(-5) == NULL);
    printf("Test 1 passed\n\n");
}

static void test_create_and_initial_state(void) {
    printf("Test 2: create and initial state\n");
    unionFind *uf = ufCreate(5);
    assert(uf != NULL);

    for (int i = 0; i < 5; i++) {
        assert(uf->parent[i] == -1);
        assert(ufFindParent(uf, i) == i);
    }

    ufFree(uf);
    printf("Test 2 passed\n\n");
}

static void test_union_connected_and_size(void) {
    printf("Test 3: union, connected, and size\n");
    unionFind *uf = ufCreate(5);
    assert(uf != NULL);

    assert(ufConnected(uf, 0, 1) == 0);
    assert(ufUnion(uf, 0, 1) == 1);
    assert(ufConnected(uf, 0, 1) == 1);

    int root01 = ufFindParent(uf, 0);
    assert(root01 == ufFindParent(uf, 1));
    assert(uf->parent[root01] == -2);

    assert(ufUnion(uf, 2, 3) == 1);
    assert(ufUnion(uf, 3, 4) == 1);
    int root234 = ufFindParent(uf, 2);
    assert(root234 == ufFindParent(uf, 3));
    assert(root234 == ufFindParent(uf, 4));
    assert(uf->parent[root234] == -3);

    int larger_root_before = root234;
    assert(ufUnion(uf, 0, 2) == 1);
    assert(ufFindParent(uf, 0) == larger_root_before);
    assert(ufConnected(uf, 1, 4) == 1);

    int root_all = ufFindParent(uf, 4);
    assert(uf->parent[root_all] == -5);

    assert(ufUnion(uf, 1, 4) == 0);

    ufFree(uf);
    printf("Test 3 passed\n\n");
}

static void test_path_compression(void) {
    printf("Test 4: path compression\n");
    unionFind *uf = ufCreate(6);
    assert(uf != NULL);

    assert(ufUnion(uf, 0, 1) == 1);
    assert(ufUnion(uf, 2, 3) == 1);
    assert(ufUnion(uf, 0, 2) == 1);
    assert(ufUnion(uf, 4, 5) == 1);
    assert(ufUnion(uf, 4, 0) == 1);

    int root = ufFindParent(uf, 4);
    assert(uf->parent[4] == root);
    assert(ufFindParent(uf, 5) == root);

    ufFree(uf);
    printf("Test 4 passed\n\n");
}

static void test_free_null(void) {
    printf("Test 5: free NULL safety\n");
    ufFree(NULL);
    printf("Test 5 passed\n\n");
}

static void test_deep_nesting_and_compression(void) {
    printf("Test 6: deep nesting and path compression\n");
    unionFind *uf = ufCreate(7);
    assert(uf != NULL);

    // Create a long chain: 0-1-2-3-4-5-6
    assert(ufUnion(uf, 0, 1) == 1);
    assert(ufUnion(uf, 1, 2) == 1);
    assert(ufUnion(uf, 2, 3) == 1);
    assert(ufUnion(uf, 3, 4) == 1);
    assert(ufUnion(uf, 4, 5) == 1);
    assert(ufUnion(uf, 5, 6) == 1);

    // All should have the same root
    int root = ufFindParent(uf, 0);
    assert(ufFindParent(uf, 6) == root);

    // After path compression from deep element, it should point closer to root
    int root_after = ufFindParent(uf, 0);
    assert(uf->parent[0] == root_after);

    // Verify size is correct (all 7 elements in one set)
    assert(uf->parent[root] == -7);

    ufFree(uf);
    printf("Test 6 passed\n\n");
}

static void test_large_scale(void) {
    printf("Test 7: large-scale operations\n");
    int n = 1000;
    unionFind *uf = ufCreate(n);
    assert(uf != NULL);

    // Chain all elements together: 0-1-2-3-...-999
    for (int i = 0; i < n - 1; i++) {
        assert(ufUnion(uf, i, i + 1) == 1);
    }

    // All should be connected to the same root
    int root = ufFindParent(uf, 0);
    for (int i = 1; i < n; i++) {
        assert(ufConnected(uf, 0, i) == 1);
    }

    // Size should be n
    assert(uf->parent[root] == -n);

    ufFree(uf);
    printf("Test 7 passed\n\n");
}

static void test_separate_components(void) {
    printf("Test 8: separate components remain separate\n");
    unionFind *uf = ufCreate(8);
    assert(uf != NULL);

    // Create two separate components: {0,1,2} and {3,4,5}
    assert(ufUnion(uf, 0, 1) == 1);
    assert(ufUnion(uf, 1, 2) == 1);
    assert(ufUnion(uf, 3, 4) == 1);
    assert(ufUnion(uf, 4, 5) == 1);

    // Elements 6 and 7 remain isolated

    // Verify components are separate
    assert(ufConnected(uf, 0, 2) == 1);
    assert(ufConnected(uf, 3, 5) == 1);
    assert(ufConnected(uf, 0, 3) == 0);  // Different components
    assert(ufConnected(uf, 2, 4) == 0);  // Different components
    assert(ufConnected(uf, 0, 6) == 0);  // Isolated element
    assert(ufConnected(uf, 6, 7) == 0);  // Two isolated elements

    // Verify sizes
    int root_012 = ufFindParent(uf, 0);
    int root_345 = ufFindParent(uf, 3);
    assert(uf->parent[root_012] == -3);
    assert(uf->parent[root_345] == -3);
    assert(uf->parent[6] == -1);
    assert(uf->parent[7] == -1);

    ufFree(uf);
    printf("Test 8 passed\n\n");
}

static void test_repeated_union_same_pair(void) {
    printf("Test 9: repeated union of same pair returns 0\n");
    unionFind *uf = ufCreate(4);
    assert(uf != NULL);

    assert(ufUnion(uf, 0, 1) == 1);
    assert(ufUnion(uf, 0, 1) == 0);  // Already connected
    assert(ufUnion(uf, 1, 0) == 0);  // Order doesn't matter, still connected

    // Verify they're still connected and size is still 2
    int root = ufFindParent(uf, 0);
    assert(ufFindParent(uf, 1) == root);
    assert(uf->parent[root] == -2);

    ufFree(uf);
    printf("Test 9 passed\n\n");
}

static void test_single_element(void) {
    printf("Test 10: single element union-find\n");
    unionFind *uf = ufCreate(1);
    assert(uf != NULL);

    assert(ufFindParent(uf, 0) == 0);
    assert(ufConnected(uf, 0, 0) == 1);
    assert(uf->parent[0] == -1);

    ufFree(uf);
    printf("Test 10 passed\n\n");
}

static void test_union_by_size_ordering(void) {
    printf("Test 11: union by size maintains smaller set under larger\n");
    unionFind *uf = ufCreate(10);
    assert(uf != NULL);

    // Create set of size 3: {0,1,2}
    assert(ufUnion(uf, 0, 1) == 1);
    assert(ufUnion(uf, 1, 2) == 1);

    // Create set of size 5: {3,4,5,6,7}
    assert(ufUnion(uf, 3, 4) == 1);
    assert(ufUnion(uf, 4, 5) == 1);
    assert(ufUnion(uf, 5, 6) == 1);
    assert(ufUnion(uf, 6, 7) == 1);

    // Union them: the larger set (size 5) should be the parent
    assert(ufUnion(uf, 0, 3) == 1);

    int root_0 = ufFindParent(uf, 0);
    int root_3 = ufFindParent(uf, 3);
    assert(root_0 == root_3);
    assert(uf->parent[root_0] == -8);  // Combined size is 8

    ufFree(uf);
    printf("Test 11 passed\n\n");
}

int main(void) {
    printf("=== Running Union-Find Tests ===\n\n");

    test_create_invalid_size();
    test_create_and_initial_state();
    test_union_connected_and_size();
    test_path_compression();
    test_free_null();
    test_deep_nesting_and_compression();
    test_large_scale();
    test_separate_components();
    test_repeated_union_same_pair();
    test_single_element();
    test_union_by_size_ordering();

    printf("=== All Union-Find tests passed! ===\n");
    return 0;
}
