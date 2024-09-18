// Author: Nathan Tran (ntran48@ucsc.edu)

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "LinkedList.h"


// **** Declare any function prototypes here ***
ListItem *CreateUnsortedList(void); //this function has been written for you
ListItem *SelectionSort(ListItem* list);
ListItem *InsertionSort(ListItem* list);


/* Students should not need to modify main! */
int main(void)
{
    BOARD_Init();

    printf("\n\nWelcome to ntran48's sort.c, compiled on %s %s.\n", __DATE__, __TIME__);

    ListItem* listToSort;

    //Do a timing test 
    printf("\nStarting stopwatch for selection sort...\n");
    listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    listToSort = SelectionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);


    printf("\nStarting stopwatch for insertion sort...\n");
    listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    listToSort = InsertionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);

    BOARD_End();
    while (1);
    return 0;
}

/**
 * SelectionSort() performs a selection sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem *SelectionSort(ListItem* list) {
    // Pointer to the first unsorted (FU) item
    ListItem *FU = list;
    
    // Iterate until FU reaches the tail
    while (FU->nextItem != NULL) {
        // Pointer to scan (S) through the unsorted partition
        ListItem *S = FU->nextItem;
        
        // Pointer to the smallest item found so far
        ListItem *minItem = FU;
        
        // Scan through the unsorted partition
        while (S != NULL) {
            // Compare current and minItem and update if minItem is smaller
            if (strcmp(S->data, minItem->data) < 0) {
                minItem = S;
            }
            
            S = S->nextItem;
        }
        
        // Swap data between FU and minItem
        LinkedListSwapData(FU, minItem);
        
        // Move FU to next unsorted item
        FU = FU->nextItem;
    }
    
    // Return the head of the sorted list
    return list;
}


/**
 * InsertionSort() performs an insertion sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem *InsertionSort(ListItem *list) {
    // Returns list if empty or has only one item
    if (list == NULL || list->nextItem == NULL) {
        return list;
    }

    // Initialize FS (First Sorted) to the head of the list
    ListItem *FS = LinkedListGetFirst(list);
    ListItem *temp = NULL;

    while (FS != NULL) {
        // Set LU (Last Unsorted) to FS's next item
        ListItem *LU = FS->nextItem;

        // swap their data if LU < FS
        if (LU != NULL && strcmp(LU->data, FS->data) < 0) {
            LinkedListSwapData(LU, FS);
            temp = FS;

            // Iterate backwards from FS to find correct position for LU
            while (temp->previousItem != NULL && strcmp(temp->previousItem->data, temp->data) > 0) {
                LinkedListSwapData(temp->previousItem, temp);
                temp = temp->previousItem;
            }
        }

        // Move FS to its next item
        FS = FS->nextItem;
    }

    // Return the head
    return LinkedListGetFirst(list);
}

/* CreateUnsortedList() uses the functions in the LinkedList library to
 * generate an unsorted list.  
 * 
 * Students should not modify this function!
 * If an error occurs in this function, there is almost certainly 
 * a bug in your implementation of LinkedList.h.
 *   
 * @return A pointer to a ListItem which is the head of a new unsorted list. 
 */
ListItem *CreateUnsortedList(void)
/// <editor-fold defaultstate="collapsed" desc="CreateUnsortedList definition">
{
 char* wordList[] = {
        "decide",   "toothpaste",   "lowly",        "robin",        "reign",        "crowd",
        "pies",     "reduce",       "tendency",     "surround",     "finger",       "rake", 
        "alleged",  "hug",          "nest",         "punishment",   "eggnog",       "side", 
        "beef",     "exuberant",    "sort",         "scream",       "zip",          "hair", 
        "ragged",   "damage",       "thought",      "jump",         "frequent",     "substance",
        "head",     "step",         "faithful",     "sidewalk",     "pig",          "raspy",
        "juggle",   "shut",         "maddening",    "rock",         "telephone",    "selective",
        NULL};
    //  char* wordList[] = {"D", "A", "C", "E", "B", NULL};


    int i = 0;
    ListItem* head = LinkedListNew(wordList[i]);
    ListItem* tail = head;
    for (i = 1; wordList[i] != NULL; i++) {
        tail = LinkedListCreateAfter(tail, wordList[i]);
        if(tail == NULL){
            printf("ERROR:  Heap full! Please increase heap size.\n");
            FATAL_ERROR();
        }
    }
    return head;
}
// </editor-fold>
