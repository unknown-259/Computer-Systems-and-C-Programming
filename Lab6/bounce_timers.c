// Author: Nathan Tran (ntran48@ucsc.edu)

// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"


// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
#define TIMER_A_COUNT 2
#define TIMER_B_COUNT 3
#define TIMER_C_COUNT 5

static struct Timer TimerA = {0, TIMER_A_COUNT};
static struct Timer TimerB = {0, TIMER_B_COUNT};
static struct Timer TimerC = {0, TIMER_C_COUNT};

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to ntran48's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);
    
    while (1) {
        if (TimerA.event) {     // Poll timer A
            printf("A\n");      // React to timer A events
            TimerA.event = 0;   // Clear timer A event flag
        }
        
        if (TimerB.event) {
            printf("B\n");
            TimerB.event = 0;
        }
        
        if (TimerC.event) {
            printf("C\n");
            TimerC.event = 0;
        }
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    
    // Decrement value member of each Timer
    TimerA.timeRemaining--;
    TimerB.timeRemaining--;
    TimerC.timeRemaining--;
    
    // Update timerA
    if (TimerA.timeRemaining == 0) {    // If timerA has counted down,
        TimerA.event = 1;   // Generate timerA event
        TimerA.timeRemaining = TIMER_A_COUNT;   // Reset timerA
    }
    
    // Update timerB
    if (TimerB.timeRemaining == 0) {
        TimerB.event = 1;
        TimerB.timeRemaining = TIMER_B_COUNT;
    }
    
    // Update timerC
    if (TimerC.timeRemaining == 0) {
        TimerC.event = 1;
        TimerC.timeRemaining = TIMER_C_COUNT;
    }
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}