// Nathan Tran: ntran48@ucsc.edu; Thuy Lan Phan: tphan17@ucsc.edu
// Standard libraries
#include <stdio.h>
#include <math.h>
#include <stdbool.h>


//User libraries:
#include "BOARD.h"

#ifdef PIC32
// Microchip libraries
#include <xc.h>
#endif


// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Divide(double operand1, double operand2);
double AbsoluteValue(double operand);
double Average(double operand1, double operand2);
double FahrenheitToCelsius(double operand);
double CelsiusToFahrenheit(double operand);
double Tangent(double operand);
//add more prototypes here
double Round(double operand);

void CalculatorRun(void) {
    char operand;
    printf("\nWelcome to Thuy Lan's and Nathan's calculator program! Compiled at %s %s\n", __DATE__, __TIME__);
    
    // Your code here
    while (1) {
        printf("\nEnter a mathematical operation to perform (*,/,+,-,m,a,c,f,t,r):  \b");
        scanf("%s", &operand);

        // Binary operation
        if ((operand == '*') || (operand == '/') || (operand == '+') || (operand == '-') || (operand == 'm')) {
            double a, b;
            printf("   Enter the first operand:  \b");
            scanf("%lf", &a);
            printf("   Enter the second operand:  \b");
            scanf("%lf", &b);

            // Print operations
            if (operand == '*') {
                printf("   Result of (%lf * %lf): %lf\n", a, b, Multiply(a, b));
            } else if (operand == '/') {
                
                // Checks if the 2nd operand is 0
                if (Divide(a, b)==HUGE_VAL){
                    printf("   Divide by zero error!\n");
                }else{
                    printf("   Result of (%lf / %lf): %lf\n", a, b, Divide(a, b));
                }
                
            } else if (operand == '+') {
                printf("   Result of (%lf + %lf): %lf\n", a, b, Add(a, b));
            } else if (operand == '-') {
                printf("   Result of (%lf - %lf): %lf\n", a, b, Subtract(a, b));
            } else if (operand == 'm') {
                printf("   Result of avg(%lf, %lf): %lf\n", a, b, Average(a, b));
            }
            
            // Clears operand for edge case where user doesn't put in valid char
            operand = ' ';
            
        // Unary operation  
        } else if ((operand == 'a') || (operand == 'c') || (operand == 'f') || (operand == 't') || (operand == 'r')) {
            double a;
            printf("   Enter the first operand:  \b");
            scanf("%lf", &a);

            // Print operations
            if (operand == 'a') {
                printf("   Result of |%lf|: %lf\n", a, AbsoluteValue(a));
            } else if (operand == 'c') {
                printf("   Result of (%lf deg->C): %lf\n", a, FahrenheitToCelsius(a));
            } else if (operand == 'f') {
                printf("   Result of (%lf deg->F): %lf\n", a, CelsiusToFahrenheit(a));
            } else if (operand == 't') {
                printf("   Result of tan(%lf): %lf\n", a, Tangent(a));
            } else if (operand == 'r') {
                printf("   Result of round(%lf): %lf\n", a, Round(a));
            }

            // Clears operand for edge case where user doesn't put in valid char
            operand = ' ';
            
        // Break loop to end
        } else if (operand == 'q') {
            break;

        // Reprompt if not valid operand
        } else {
            while ((operand != '*') || (operand != '/') || (operand != '+') || (operand != '-') ||
                    (operand != 'm') || (operand != 'a') || (operand != 'c') || (operand != 'f') ||
                    (operand != 't') || (operand != 'r') || (operand != 'q')) {
                printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r):  \b");
                scanf("%s", &operand);
                break;
            }

        }
    }
    BOARD_End();

}

/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2) {
    return operand1 + operand2;
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2) {
    return operand1 - operand2;
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2) {
    return operand1 * operand2;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2) {
    if (operand2 == 0) {
        return HUGE_VAL;
    } else
        return operand1 / operand2;
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand) {
    if (operand < 0) {
        return operand * -1;
    } else
        return operand;

}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand) {
    return ((float) (5) / 9) * (operand - 32);
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand) {
    return 32 + (operand * 9 / 5);
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2) {
    return (operand1 + operand2) / 2;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand) {
    // Converts to rad, then find tangent
    return tan(operand * M_PI / 180);
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand) {
    if (operand < 0.0)
        return (double) ((int) (operand - 0.5));
    else
        return (double) ((int) (operand + 0.5));
}


