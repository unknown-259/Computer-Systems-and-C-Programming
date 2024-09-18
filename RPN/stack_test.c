// Nathan Tran (ntran48@ucsc.edu)

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "stack.h"

int main()
{
    BOARD_Init();

    printf("\n###### Beginning ntran48's stack test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    
    // Checks StackInit())
    printf("Does StackInit() set the currentItemIndex appropriately?\n");
    struct Stack stack = {};
    
    // Test 1: Initialize the index to -1
    StackInit(&stack);
    if (stack.currentItemIndex == -1) {
        printf("Test 1: PASSED\n");
    } else {
        printf("Test 1: FAILED\n");
    }
    
    // Test 2: Initialize the boolean to TRUE
    if (stack.initialized == TRUE) {
        printf("Test 2: PASSED\n");
    } else {
        printf("Test 2: FAILED\n");
    }
    
    
    
    
    // Checks StackPush()
    printf("\nDoes StackPush() push values correctly to the stack?\n");

    // Test 1: Push null stack
    double value = 5.0;
    int result = StackPush(NULL, value);
    if (result == STANDARD_ERROR) {
        printf("Test 1: PASSED\n");
    } else {
        printf("Test 1: FAILED\n");
    }
    
    // Test 2: Push a value
    value = 5.0;
    result = StackPush(&stack, value);
    if (result == SUCCESS) {
        printf("Test 2: PASSED\n");
    } else {
        printf("Test 2: FAILED\n");
    }

    // Test 3: Push a char
    char char_value = '+';
    char char_result = StackPush(&stack, char_value);
    if (char_result == SUCCESS) {
        printf("Test 3: PASSED\n");
    } else {
        printf("Test 3: FAILED\n");
    }
    
    // Test 4: Push a value onto a full stack
    for (int i = 0; i < 20; i++){
        result = StackPush(&stack, value);
    }
    if (result == STANDARD_ERROR) {
        printf("Test 4: PASSED\n");
    } else {
        printf("Test 4: FAILED\n");
    }

    
    
    
    // Checks StackPop()
    printf("\nDoes StackPop() pop an element correctly in the stack?\n");
    
    // Test 1: Pop null stack
    double poppedVal = 0;
    if (StackPop(NULL, &poppedVal) == STANDARD_ERROR){
        printf("Test 1: PASSED\n");
    } else {
        printf("Test 1: FAILED\n");
    }
    
    // Test 2:  Pop a non-empty stack
    if (StackPop(&stack, &poppedVal) == SUCCESS){
        printf("Test 2: PASSED\n");
    } else {
        printf("Test 2: FAILED\n");
    }
    
    // Test 3: Pop an empty stack
    for (int i = 0; i < 19; i++){
        StackPop(&stack, &poppedVal);
    }
    if (StackPop(&stack, &poppedVal) == STANDARD_ERROR){
        printf("Test 3: PASSED\n");
    } else {
        printf("Test 3: FAILED\n");
    }
    
    
    
    
    // Checks StackIsEmpty()
    printf("\nDoes StackIsEmpty() correctly checks if the stack is empty?\n");
    
    // Test 1: Check null stack
    if (StackIsEmpty(NULL) == FALSE){
        printf("Test 1: PASSED\n");
    } else {
        printf("Test 1: FAILED\n");
    }
    
    // Test 2: Empty stack
    if (StackIsEmpty(&stack) == TRUE){
        printf("Test 2: PASSED\n");
    } else {
        printf("Test 2: FAILED\n");
    }
    
    // Test 3: Non-empty stack
    StackPush(&stack, value);
    if (StackIsEmpty(&stack) == FALSE){
        printf("Test 3: PASSED\n");
    } else {
        printf("Test 3: FAILED\n");
    }
    
    
    
    
    // Checks StackIsFull()
    printf("\nDoes StackIsFull() correctly checks if the stack is full?\n");
    
    // Test 1: Check null stack
    if (StackIsFull(NULL) == FALSE){
        printf("Test 1: PASSED\n");
    } else {
        printf("Test 1: FAILED\n");
    }
    
    // Test 2: Full stack
    for (int i = 0; i < 19; i++){
        result = StackPush(&stack, value);
    }
    if (StackIsFull(&stack) == TRUE){
        printf("Test 2: PASSED\n");
    } else {
        printf("Test 2: FAILED\n");
    }
    
    // Test 3: Partially filled stack
    for (int i = 0; i < 3; i++){
        StackPop(&stack, &poppedVal);
    }
    if (StackIsFull(&stack) == FALSE){
        printf("Test 3: PASSED\n");
    } else {
        printf("Test 3: FAILED\n");
    }
    
    
    
    
    // Checks StackGetSize()
    printf("\nDoes StackGetSize() return the correct size of the stack?\n");
    
    // Test 1: Check null stack
    if (StackGetSize(NULL) == SIZE_ERROR){
        printf("Test 1: PASSED\n");
    } else {
        printf("Test 1: FAILED\n");
    }
    
    // Test 2: Partial stack
    if (StackGetSize(&stack) == 17) {
        printf("Test 2: PASSED\n");
    } else {
        printf("Test 2: FAILED\n");
    }
    
    // Test 3: Full stack
    for (int i = 0; i < 3; i++){
        result = StackPush(&stack, value);
    }
    if (StackGetSize(&stack) == STACK_SIZE) {
        printf("Test 3: PASSED\n");
    } else {
        printf("Test 3: FAILED\n");
    }
    
    // Test 4: Empty stack
    for (int i = 0; i < STACK_SIZE; i++){
        StackPop(&stack, &poppedVal);
    }
    if (StackGetSize(&stack) == 0) {
        printf("Test 4: PASSED\n");
    } else {
        printf("Test 4: FAILED\n");
    }
    
    

    
    BOARD_End();
    //test stackInit:
    while (1);
    return 0;
}


