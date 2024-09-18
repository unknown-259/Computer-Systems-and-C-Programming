// Nathan Tran (ntran48@ucsc.edu)

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"

// **** Set macros and preprocessor directives ****
// The lab calls for 60 characters of user input
#define MAX_INPUT_LENGTH 60

int main() {
    BOARD_Init();

    //What follows is starter code.  You will need to modify it!

    char rpn_sentence[MAX_INPUT_LENGTH];
    double result;

    printf("\nWelcome to ntran48'S RPN calculator.  Compiled on %s %s\n", __DATE__, __TIME__);
    while (1) {

        printf("Enter floats and + - / * in RPN format:\n");

        fgets(rpn_sentence, sizeof(rpn_sentence), stdin);
        
        // Removes the new line character at the end
        strtok(rpn_sentence, "\n");
        int error = RPN_Evaluate(rpn_sentence, &result);
        
        if (error == RPN_ERROR_STACK_OVERFLOW){
        printf("ERROR: No more room in stack\n\n");
        
        } else if (error == RPN_ERROR_STACK_UNDERFLOW){
        printf("ERROR: Not enough operands\n\n");
        
        } else if (error == RPN_ERROR_INVALID_TOKEN){
        printf("ERROR: Invalid character in RPN string\n\n");
        
        } else if (error == RPN_ERROR_DIVIDE_BY_ZERO){
        printf("ERROR: Can't divide by zero\n\n");
        
        } else if (error == RPN_ERROR_TOO_FEW_ITEMS_REMAIN){
        printf("ERROR: Not enough to perform an operation\n\n");
        
        } else if (error == RPN_ERROR_TOO_MANY_ITEMS_REMAIN){
        printf("ERROR: Invalid RPN calculation\n\n");
        } else {
        
        printf("result = %f\n\n", result);
        }
        
    }
    while (1);
}
