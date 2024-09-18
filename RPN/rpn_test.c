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

#define MAX_CHAR_LENGTH 255


void fPrint(char test[MAX_CHAR_LENGTH], int error, double expected, double *result) {
    printf("Testing RPN_Evaluate with \"%s\"... \n", test);
    error = RPN_Evaluate(test, result);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n\n");
    } else if (*result != expected) {
        printf("   Failed, expected = %f , result = %f\n\n", expected, *result);
    } else {
        printf("   Success!\n\n");
    }
}


int main()
{
    BOARD_Init();
    int error = 0;

    printf("\n###### Beginning ntran48's rpn test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    char test[MAX_CHAR_LENGTH];
    double result;
    double expected;
    
    
    // Minimal RPN expressions
    // RPN Test 1: 1+1
    strncpy(test, "1 1 +", 6);
    expected = 2;
    fPrint(test, error, expected, &result);
    
    // RPN Test 2: 2-4
    strncpy(test, "2 4 -", 6);
    expected = -2;
    fPrint(test, error, expected, &result);
    
    // RPN Test 3: 3*9
    strncpy(test, "3 9 *", 6);
    expected = 27;
    fPrint(test, error, expected, &result);
    
    // RPN Test 4: 5/2
    strncpy(test, "5 2 /", 6);
    expected = 2.5;
    fPrint(test, error, expected, &result);
    
    
    
    
    // Complex RPN expressions
    // RPN Test 1: 3+6*8
    strncpy(test, "3 6 8 * +", 10);
    expected = 51;
    fPrint(test, error, expected, &result);
    
    // RPN Test 2: 1/2*8-4
    strncpy(test, "1 2 / 8 * 4 -", 14);
    expected = 0;
    fPrint(test, error, expected, &result);
    
    // RPN Test 3: 5+1/2-6/4*3
    strncpy(test, "5 1 2 / + 6 4 / 3 * -", 22);
    expected = 1;
    fPrint(test, error, expected, &result);
    
    
   
    
    // Handling RPN errors
    printf("\nTesting RPN errors:\n");

    // Test case for RPN_ERROR_STACK_OVERFLOW
    strncpy(test, "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 +", 56);
    error = RPN_Evaluate(test, &result);
    printf("RPN_ERROR_STACK_OVERFLOW Test: %s\n", error == RPN_ERROR_STACK_OVERFLOW ? "PASS" : "FAIL");

    // Test case for RPN_ERROR_STACK_UNDERFLOW
    strncpy(test, "5 +", 4);
    error = RPN_Evaluate(test, &result);
    printf("RPN_ERROR_STACK_UNDERFLOW Test: %s\n", error == RPN_ERROR_STACK_UNDERFLOW ? "PASS" : "FAIL");

    // Test case for RPN_ERROR_INVALID_TOKEN
    strncpy(test, "5 B +", 6);
    error = RPN_Evaluate(test, &result);
    printf("RPN_ERROR_INVALID_TOKEN Test: %s\n", error == RPN_ERROR_INVALID_TOKEN ? "PASS" : "FAIL");

    // Test case for RPN_ERROR_DIVIDE_BY_ZERO
    strncpy(test, "5 0 /", 6);
    error = RPN_Evaluate(test, &result);
    printf("RPN_ERROR_DIVIDE_BY_ZERO Test: %s\n", error == RPN_ERROR_DIVIDE_BY_ZERO ? "PASS" : "FAIL");

    // Test case for RPN_ERROR_TOO_FEW_ITEMS_REMAIN
    strncpy(test, "+", 2);
    error = RPN_Evaluate(test, &result);
    printf("RPN_ERROR_TOO_FEW_ITEMS_REMAIN Test: %s\n", error == RPN_ERROR_TOO_FEW_ITEMS_REMAIN ? "PASS" : "FAIL");

    // Test case for RPN_ERROR_TOO_MANY_ITEMS_REMAIN
    strncpy(test, "5 5 + 5", 8);
    error = RPN_Evaluate(test, &result);
    printf("RPN_ERROR_TOO_MANY_ITEMS_REMAIN Test: %s\n", error == RPN_ERROR_TOO_MANY_ITEMS_REMAIN ? "PASS" : "FAIL");
    
    
    
    
    
    // Backspace Test 1 (given)
    printf("\n\nTesting ProcessBackspaces:\n");
    printf("Test 1: ");
    char test_pb1[] = "123\b34";
    ProcessBackspaces(test_pb1);
    printf("result    : %s\n", test_pb1);
    printf("        should be : 1234\n");
    
    // Backspace Test 2 (Multiple Backspace)
    printf("Test 2: ");
    char test_pb2[] = "12\b23\b34\b45\b56\b";
    result = ProcessBackspaces(test_pb2);
    printf("result    : %s\n", test_pb2);
    printf("        should be : 12345\n");
    
    // Backspace Test 3 (NULL)
    printf("Test 3: ");
    char *rpn_sentence = NULL;
    result = ProcessBackspaces(rpn_sentence);
    printf("result    : %f\n", result);
    printf("        should be : 0.000000\n");
    
    
    // Backspace Test 4 (No Backspace)
    printf("Test 4: ");
    char sentence_no_bs[] = "4321";
    ProcessBackspaces(sentence_no_bs);
    printf("result    : %s\n", sentence_no_bs);
    printf("        should be : 4321\n");
    
    // Backspace Test 5 (All Backspace)
    printf("Test 5: ");
    char sentence_backspace[] = "\b\b\b\b\b";
    result = ProcessBackspaces(sentence_backspace);
    printf("result    : %s\n", sentence_backspace);
    printf("        should be : \n");
    
    
    BOARD_End();
    while (1);
}


