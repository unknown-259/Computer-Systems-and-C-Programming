// Author: Nathan Tran (ntran48@ucsc.edu)

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Support Library
#include "BOARD.h"
#include "LinkedList.h"

void test_LinkedListNew() {
    
    // Test 1: Create a new list item with data
    char *data = "Hello";
    ListItem *item1 = LinkedListNew(data);
    if (item1 != NULL && strcmp(item1->data, data) == 0) {
        printf("    Test 1: PASSED\n");
    } else {
        printf("    Test 1: FAILED\n");
    }

    // Test 2: Create a new list item with NULL
    ListItem *item2 = LinkedListNew(NULL);
    if (item2 != NULL && item2->data == NULL) {
        printf("    Test 2: PASSED\n");
    } else {
        printf("    Test 2: FAILED\n");
    }
}

void test_LinkedListCreateAfter() {
    
    // Test 1: Insert a new item after an existing item
    char *data1 = "Hello";
    char *data2 = "World";
    ListItem *item1 = LinkedListNew(data1);
    ListItem *newItem = LinkedListCreateAfter(item1, data2);
    if (newItem != NULL && strcmp(newItem->data, data2) == 0 && item1->nextItem == newItem) {
        printf("    Test 1: PASSED\n");
    } else {
        printf("    Test 1: FAILED\n");
    }

    // Test 2: Insert a new item with NULL parameters
    ListItem *newItem2 = LinkedListCreateAfter(NULL, NULL);
    if (newItem2 != NULL && newItem2->data == NULL) {
        printf("    Test 2: PASSED\n");
    } else {
        printf("    Test 2: FAILED\n");
    }
}

void test_LinkedListRemove() {
    // Test 1: Remove an item from the list
    char *data1 = "Hello";
    ListItem *item1 = LinkedListNew(data1);
    char *removedData = LinkedListRemove(item1);
    if (removedData != NULL && strcmp(removedData, data1) == 0) {
        printf("    Test 1: PASSED\n");
    } else {
        printf("    Test 1: FAILED\n");
    }

    // Test 2: Attempt to remove NULL item
    char *removedData2 = LinkedListRemove(NULL);
    if (removedData2 == NULL) {
        printf("    Test 2: PASSED\n");
    } else {
        printf("    Test 2: FAILED\n");
    }
}

void test_LinkedListSize() {
    // Test 1: Calculate size of a non-empty list
    char *data1 = "Hello";
    ListItem *item1 = LinkedListNew(data1);
    LinkedListCreateAfter(item1, "World");
    int size = LinkedListSize(item1);
    if (size == 2) {
        printf("    Test 1: PASSED\n");
    } else {
        printf("    Test 1: FAILED\n");
    }

    // Test 2: Calculate size of an empty list
    int size2 = LinkedListSize(NULL);
    if (size2 == 0) {
        printf("    Test 2: PASSED\n");
    } else {
        printf("    Test 2: FAILED\n");
    }
}


void test_LinkedListGetFirst() {
    // Test 1: Get the first item of a list
    char *data1 = "Hello";
    ListItem *item1 = LinkedListNew(data1);
    ListItem *firstItem = LinkedListGetFirst(item1);
    if (firstItem == item1) {
        printf("    Test 1: PASSED\n");
    } else {
        printf("    Test 1: FAILED\n");
    }

    // Test 2: Get the first item of an empty list
    ListItem *firstItem2 = LinkedListGetFirst(NULL);
    if (firstItem2 == NULL) {
        printf("    Test 2: PASSED\n");
    } else {
        printf("    Test 2: FAILED\n");
    }
}

void test_LinkedListGetLast() {
    // Test 1: Get the last item of a list
    char *data1 = "Hello";
    char *data2 = "World";
    ListItem *item1 = LinkedListNew(data1);
    ListItem *item2 = LinkedListCreateAfter(item1, data2);
    ListItem *lastItem = LinkedListGetLast(item1);
    if (lastItem == item2) {
        printf("    Test 1: PASSED\n");
    } else {
        printf("    Test 1: FAILED\n");
    }

    // Test 2: Get the last item of an empty list
    ListItem *lastItem2 = LinkedListGetLast(NULL);
    if (lastItem2 == NULL) {
        printf("    Test 2: PASSED\n");
    } else {
        printf("    Test 2: FAILED\n");
    }
}

void test_LinkedListSwapData() {
    // Test 1: Swap data of two items
    char *data1 = "Hello";
    char *data2 = "World";
    ListItem *item1 = LinkedListNew(data1);
    ListItem *item2 = LinkedListNew(data2);
    int result = LinkedListSwapData(item1, item2);
    if (result == SUCCESS && strcmp(item1->data, "World") == 0 && strcmp(item2->data, "Hello") == 0) {
        printf("    Test 1: PASSED\n");
    } else {
        printf("    Test 1: FAILED\n");;
    }

    // Test 2: Attempt to swap data with NULL item
    int result2 = LinkedListSwapData(item1, NULL);
    if (result2 == STANDARD_ERROR && strcmp(item1->data, "World") == 0) {
        printf("    Test 2: PASSED\n");
    } else {
        printf("    Test 2: FAILED\n");
    }
}

void test_LinkedListPrint() {
    // Test 1: Print a non-empty list
    char *data1 = "A";
    ListItem *item1 = LinkedListNew(data1);
    ListItem *item2 = LinkedListCreateAfter(item1, "B");
    LinkedListCreateAfter(item2, "C");
    printf("    Expected output: [A, B, C]\n");
    printf("    Actual output: ");
    LinkedListPrint(item1);
    printf("\n");

    // Test 2: Print an empty list
    printf("    Expected output: []\n");
    printf("    Actual output: ");
    LinkedListPrint(NULL);
    printf("\n");
}


int main() {
    printf("Testing LinkedListNew()\n");
    test_LinkedListNew();

    printf("\nTesting LinkedListCreateAfter()\n");
    test_LinkedListCreateAfter();

    printf("\nTesting LinkedListRemove()\n");
    test_LinkedListRemove();

    printf("\nTesting LinkedListSize()\n");
    test_LinkedListSize();

    printf("\nTesting LinkedListGetFirst()\n");
    test_LinkedListGetFirst();

    printf("\nTesting LinkedListGetLast()\n");
    test_LinkedListGetLast();

    printf("\nTesting LinkedListSwapData()\n");
    test_LinkedListSwapData();

    printf("\nTesting LinkedListPrint()\n");
    test_LinkedListPrint();

    return 0;
}
