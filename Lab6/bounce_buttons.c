// Author: Nathan Tran (ntran48@ucsc.edu)

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"
#include "Leds_Lab06.h"

// **** Set macros and preprocessor directives ****
enum LEDRegisters{
    LED1 = 0x01,
    LED2 = 0x02,
    LED3 = 0x04,
    LED4 = 0x08,
    LED5 = 0x10,
    LED6 = 0x20,
    LED7 = 0x40,
    LED8 = 0x80
};

// **** Declare any datatypes here ****
static uint8_t timerEvent;

// **** Define global, module-level, or external variables here ****

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
    printf("Welcome to ntran48's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);

    LEDS_INIT();
    ButtonsInit();

    while (1) {
        if (timerEvent != BUTTON_EVENT_NONE) {
            char led = LEDS_GET();
            uint8_t switchesState = SWITCH_STATES();

            // Toggle LED1 and LED2 if SW1 is pressed and BUTTON_EVENT_1UP occurs, 
            // or if SW1 is not pressed and BUTTON_EVENT_1DOWN occurs
            led ^= ((switchesState & SWITCH_STATE_SW1) && (timerEvent & BUTTON_EVENT_1UP)) ? LED1 ^ LED2 : 0;
            led ^= (!(switchesState & SWITCH_STATE_SW1) && (timerEvent & BUTTON_EVENT_1DOWN)) ? LED1 ^ LED2 : 0;

            led ^= ((switchesState & SWITCH_STATE_SW2) && (timerEvent & BUTTON_EVENT_2UP)) ? LED3 ^ LED4 : 0;
            led ^= (!(switchesState & SWITCH_STATE_SW2) && (timerEvent & BUTTON_EVENT_2DOWN)) ? LED3 ^ LED4 : 0;

            led ^= ((switchesState & SWITCH_STATE_SW3) && (timerEvent & BUTTON_EVENT_3UP)) ? LED5 ^ LED6 : 0;
            led ^= (!(switchesState & SWITCH_STATE_SW3) && (timerEvent & BUTTON_EVENT_3DOWN)) ? LED5 ^ LED6 : 0;

            led ^= ((switchesState & SWITCH_STATE_SW4) && (timerEvent & BUTTON_EVENT_4UP)) ? LED7 ^ LED8 : 0;
            led ^= (!(switchesState & SWITCH_STATE_SW4) && (timerEvent & BUTTON_EVENT_4DOWN)) ? LED7 ^ LED8 : 0;

            // Set LED pattern
            LEDS_SET(led);
            // Reset timer event
            timerEvent = BUTTON_EVENT_NONE;
        }
    }

    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    timerEvent = ButtonsCheckEvents();

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}