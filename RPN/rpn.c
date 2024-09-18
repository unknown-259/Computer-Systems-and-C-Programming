// Author: Nathan Tran (ntran48@ucsc.edu)

// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Support Library
#include "rpn.h"
#include "stack.h"

int RPN_Evaluate(char *rpn_string, double *result) {
    // Initialize the stack
    struct Stack stack;
    StackInit(&stack);
    double operand2, operand1;
    int bools1 = 0;
    int bools2 = 0;

    char *token = strtok(rpn_string, " ");
    while (token != NULL) {
         // Checks when token is an operator
        if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
            
            bools1 = StackPop(&stack, &operand2);
            bools2 = StackPop(&stack, &operand1);
            if (bools2 == STANDARD_ERROR && bools1 == STANDARD_ERROR) {
                return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
            }
            
            if (bools2 != SUCCESS || bools1 != SUCCESS) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            
            if (strcmp(token, "/") == 0 && operand2 == 0) {
                return RPN_ERROR_DIVIDE_BY_ZERO;
            }
            
            if (StackGetSize(&stack) == STACK_SIZE) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
            
            if (strcmp(token, "+") == 0) {
                *result = operand1 + operand2;
                StackPush(&stack, operand1 + operand2);
            } else if (strcmp(token, "-") == 0) {
                *result = operand1 - operand2;
                StackPush(&stack, operand1 - operand2);
            } else if (strcmp(token, "*") == 0) {
                *result = operand1 * operand2;
                StackPush(&stack, operand1 * operand2);
            } else if (strcmp(token, "/") == 0) {
                *result = operand1 / operand2;
                StackPush(&stack, operand1 / operand2);
            }
            
        } else {
            // Checks if token is a number 
            
            // Checks if token is the actual value 0
            if (strcmp(token, "0") == 0) {
                StackPush(&stack, atof(token));
            } else if (atof(token) == 0) {
                return RPN_ERROR_INVALID_TOKEN;
            } else {
                double num = atof(token);
                
                if (StackGetSize(&stack) == STACK_SIZE) {
                return RPN_ERROR_STACK_OVERFLOW;
                }
                StackPush(&stack, num);
            }
        }
        token = strtok(NULL, " ");
    }

    if (StackGetSize(&stack) != 1) {
        return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
    }

    return RPN_NO_ERROR;
}

int ProcessBackspaces(char *rpn_sentence) {
    if (rpn_sentence == NULL)
        return 0;

    int index = 0;

    for (int i = 0; i < strlen(rpn_sentence); i++) {
        if (rpn_sentence[i] == '\b') {
            if (index > 0) {
                index--;
            }
        } else {
            rpn_sentence[index++] = rpn_sentence[i];
        }
    }

    rpn_sentence[index] = '\0';

    return index;
}