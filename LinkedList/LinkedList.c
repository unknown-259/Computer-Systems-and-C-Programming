// Author: Nathan Tran (ntran48@ucsc.edu)

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Support Library
#include "BOARD.h"
#include "LinkedList.h"


ListItem *LinkedListNew(char *data) {
    ListItem *newItem = (ListItem *)malloc(sizeof(ListItem));
    
    // Returns NULL if malloc fails
    if (newItem == NULL) {
        return NULL;
    }
    
    newItem->previousItem = NULL;
    newItem->nextItem = NULL;
    newItem->data = data;
    return newItem;
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
    ListItem *newItem = (ListItem *)malloc(sizeof(ListItem));
    
    // Returns NULL if malloc fails
    if (newItem == NULL) {
        return NULL;
    }
    
    newItem->data = data;
    if (item != NULL) {
        
        // Insert newItem after item
        newItem->nextItem = item->nextItem;
        newItem->previousItem = item;
        if (item->nextItem != NULL) {
            item->nextItem->previousItem = newItem;
        }
        item->nextItem = newItem;
        
        // If NULL, still create new ListItem
    } else {
        newItem->nextItem = NULL;
        newItem->previousItem = NULL;
    }
    return newItem;
}

char *LinkedListRemove(ListItem *item) {
    
    // Returns NULL to signal error if item is NULL
    if (item == NULL) {
        return NULL;
    }
    
    char *data = item->data;
    
    // Adjust pointers of neighboring items
    if (item->previousItem != NULL) {
        item->previousItem->nextItem = item->nextItem;
    }
    if (item->nextItem != NULL) {
        item->nextItem->previousItem = item->previousItem;
    }
    
    // Free memory
    free(item);
    return data;
}

int LinkedListSize(ListItem *list) {
    int size = 0;
    
    // While list isn't NULL, code below will run
    while (list != NULL) {
        size++;
        list = list->nextItem;
    }
    
    // Returns 0 if NULL
    return size;
}

ListItem *LinkedListGetFirst(ListItem *list) {
    
    // Returns NULL if passed NULL
    if (list == NULL) {
        return NULL;
    }
    
    // Traverse list backwards
    while (list->previousItem != NULL) {
        list = list->previousItem;
    }
    return list;
}

ListItem *LinkedListGetLast(ListItem *list) {
    
    // Returns NULL if passed NULL
    if (list == NULL) {
        return NULL;
    }
    
    // Traverse list forwards
    while (list->nextItem != NULL) {
        list = list->nextItem;
    }
    return list;
}

int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem) {
    
    // Returns STANDARD_ERROR if either arguments is NULL
    if (firstItem == NULL || secondItem == NULL) {
        return STANDARD_ERROR;
    }
    
    // Swap data pointers
    char *temp = firstItem->data;
    firstItem->data = secondItem->data;
    secondItem->data = temp;
    return SUCCESS;
}

int LinkedListPrint(ListItem *list) {
    
    // Returns STANDARD_ERROR if argument is NULL
    if (list == NULL) {
        return STANDARD_ERROR;
    }
    
    // Prints list
    printf("[");
    while (list != NULL) {
        printf("%s", list->data);
        list = list->nextItem;
        if (list != NULL) {
            printf(", ");
        }
    }
    printf("]\n");
    return SUCCESS;
}
