// Author: Nathan Tran (ntran48@ucsc.edu)

// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
					


//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>
#include "Leds_Lab06.h"

// **** Declare any datatypes here ****
struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
static struct Timer Timer1;
enum direction {
    LEFT,
    RIGHT
};												 

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
    printf("Welcome to ntran48's lab6 part2 (bounce_switch).  Compiled on %s %s.\n",__TIME__,__DATE__);

	Timer1.event = FALSE;

    int current_direction = -1;     // Initialized current_direction as nothing
    char leftMost = 0x80;
    char rightMost = 0x01;

    char currentLED = 0x80;     // Starts with left LED
    
    LEDS_INIT();	
    
	while(1){
        if (Timer1.event) {     // Poll timer events
            LEDS_SET(currentLED);   // Set the led to current state
            
            // Change direction when reached the end
            if (currentLED == leftMost) {
                current_direction = RIGHT;
            } else if (currentLED == rightMost) {
                current_direction = LEFT;
            }
            
            // Shift the lit LED to the left or right by 1
            if (current_direction == RIGHT) {
                // Padding edge case
                if (currentLED == leftMost) {
                    currentLED = (currentLED >> 1) ^ currentLED;
                } else {
                    currentLED >>= 1;
                }
            } else {
                currentLED <<= 1;
            }
            Timer1.event = FALSE;   // End of event
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
    
    int mag = 2;    // Num to change timing of LED
    Timer1.timeRemaining--;     // Decrement the time remaining
    
    if (Timer1.timeRemaining <= 0) {    // Checks to light next LED
        Timer1.event = TRUE;
        
        uint8_t count = 0x00;     // Keep track of switches
        uint8_t switchState = SWITCH_STATES();
        
        // Check up switches
        if (switchState & SWITCH_STATE_SW1) {
            count |= SWITCH_STATE_SW1;
        }
        if (switchState & SWITCH_STATE_SW2) {
            count |= SWITCH_STATE_SW2;
        }
        if (switchState & SWITCH_STATE_SW3) {
            count |= SWITCH_STATE_SW3;
        }
        if (switchState & SWITCH_STATE_SW4) {
            count |= SWITCH_STATE_SW4;
        }
        
        Timer1.timeRemaining = count*mag;   // Makes speed difference more noticeable
        
    }
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/									
	 
}