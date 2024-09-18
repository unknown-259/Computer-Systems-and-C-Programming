// Nathan Tran (ntran48@ucsc.edu)

// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Support Library
#include "BOARD.h"
#include "stack.h"

// Declare instance of struct
struct Stack stack;

// Initialize the stack
void StackInit(struct Stack *stack) {
    stack->currentItemIndex = -1;
    stack->initialized = TRUE;
}

/**
 * Pushes a value onto the stack.  The stack index is incremented, and the passed value
 * is placed on top of the stack
 * @param:  *stack, a pointer to a stack, which is read and modified in place
 * @param:  value, the value to push.
 * @return: SUCCESS if the stack is modified successfully
 *          STANDARD_ERROR if the stack isn't initialized, or is full.
 * Note that SUCCESS and STANDARD_ERROR are defined in BOARD.h.
 */
int StackPush(struct Stack *stack, double value) {
    // Check if the stack pointer is not initialized
    if (stack == NULL) {
        return STANDARD_ERROR;
    }

    // Check if the stack is full
    if (stack->currentItemIndex >= STACK_SIZE-1) {
        return STANDARD_ERROR;
    }

    // Push value if no errors
    stack->currentItemIndex++;
    stack->stackItems[stack->currentItemIndex] = value;

    return SUCCESS;
}

/**
 * Pop an element off of a stack.   The popped value is passed out by reference, and the 
 * stack index is decremented.  
 * @param:  *stack, a pointer to a stack, which is read and modified in place
 * @param:  *value, pointer to a double which stores the popped value.
 * @return: SUCCESS if the stack is modified successfully
 *          STANDARD_ERROR if the stack isn't initialized, or is empty.
 *
 * NOTE: When trying to return the output into the value variable, you'll need
 * to write it with an extra asterisk, like:
 *   *value = 7.0;
 * This is because "value" is a pointer. K&R 5.1-5.2 should explain this practice of "passing by reference". 
 */
int StackPop(struct Stack *stack, double *value) {
    // Check if the stack pointer is initialized
    if (stack == NULL) {
        return STANDARD_ERROR;
    }

    // Check if the stack is empty
    if (stack->currentItemIndex < 0) {
        return STANDARD_ERROR;
    }

    // Pop the top value from the stack
    *value = stack->stackItems[stack->currentItemIndex];
    stack->currentItemIndex--;

    return SUCCESS;
}

/**
 * This function checks for whether the stack is empty or not. In this simple
 * case this merely checks if the currentItemIndex == -1. 
 * @param:  *stack, a pointer to a stack
 * @return: TRUE if stack is initialized and empty
 *          FALSE if the stack isn't initialized, or is not empty.
 * Note that TRUE and FALSE are defined in BOARD.h.
 */
int StackIsEmpty(const struct Stack *stack) {
    // Check if the stack pointer is NULL
    // not initialized, therefore not empty
    if (stack == NULL) {
        return FALSE;
    }

    // Check if the stack is empty
    if (stack->currentItemIndex < 0) {
        return TRUE; // Stack is empty
    } else {
        return FALSE; // Not empty
    }
}

/**
 * This function checks for whether the stack is empty or not. For this simple
 * implementation that is merely if the currentItemIndex == STACK_SIZE - 1. 
 * @param:  *stack, a pointer to a stack
 * @return: TRUE if the stack is full and FALSE if not.
 *          Also returns FALSE if the stack isn't initialized.
 */
int StackIsFull(const struct Stack *stack) {
    // Check if the stack pointer is NULL
    // not initialized, therefore not full
    if (stack == NULL) {
        return FALSE;
    }

    // Check if the stack is full
    if (stack->currentItemIndex == STACK_SIZE - 1) {
        return TRUE; // Is full
    } else {
        return FALSE; // Not full
    }
}

/**
 * Returns the current size of the stack in terms of how many active elements
 * are in it. 
 * @param:  *stack, a pointer to a stack
 * @return: SIZE_ERROR for uninitialized stacks, 
 *          the number of elements in the stack for initialized stacks, otherwise. 
 *          Note that SIZE_ERROR is declared in the BOARD.h header file.
 */
int StackGetSize(const struct Stack *stack){
    // Check if the stack pointer is NULL
    if (stack == NULL) {
        return SIZE_ERROR;
    }

    // Return the number of elements in the stack
    return stack->currentItemIndex + 1;
}