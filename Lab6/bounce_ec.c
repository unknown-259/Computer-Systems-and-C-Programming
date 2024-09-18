// Author: Nathan Tran (ntran48@ucsc.edu)

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include <string.h>

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Buttons.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

struct AdcResult {
    uint8_t event;
    uint16_t voltage;
};

// **** Define global, module-level, or external variables here ****
static uint8_t timerEvent;
static struct AdcResult AdcResult1;
static struct Timer Timer1;
enum direction {
    LEFT,
    RIGHT
};
#define SIZE 5
#define MAX_NUM 1023

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


    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time

    AD1CON1bits.ADON = 1; // and finally turn it on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to ntran48's lab6 part6 (Extra Credit).  Compiled on %s %s.\n",__TIME__,__DATE__);

    OledInit();
    ButtonsInit();
    LEDS_INIT();
    
    AdcResult1.event = TRUE;
    Timer1.event = FALSE;
    
    char stringPrint[35];
    
    int paused = 0;
    int direction = -1;
    char currentLED = 0x01;
    char leftMost = 0x80;
    char rightMost = 0x01;
    
    while (1) {
        // Same code as bounce_adc
        if (AdcResult1.event) {
            int percent = AdcResult1.voltage / (float) MAX_NUM * 100;   // Calculates percent voltage out of 1023

            // Stores to print on board
            sprintf(stringPrint, "Potentiometer:\n   %d\n   %d%%", AdcResult1.voltage, percent);

            // Clears oled and draws string
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(stringPrint);
            OledUpdate();
            AdcResult1.event = FALSE;
        }
        
        // Checks if button is pressed
        if (timerEvent != BUTTON_EVENT_NONE) {
            uint8_t state = SWITCH_STATES();
            
            // Checks buttons state
            if ((state & SWITCH_STATE_SW1) && (timerEvent & BUTTON_EVENT_1UP)) {
                paused = 1 - paused;
            } else if (!(state & SWITCH_STATE_SW1) && (timerEvent & BUTTON_EVENT_1DOWN)) {
                paused = 1 - paused;
            } else if ((state & SWITCH_STATE_SW2) && (timerEvent & BUTTON_EVENT_2UP)) {
                paused = 1 - paused;
            } else if (!(state & SWITCH_STATE_SW2) && (timerEvent & BUTTON_EVENT_2DOWN)) {
                paused = 1 - paused;
            } else if ((state & SWITCH_STATE_SW3) && (timerEvent & BUTTON_EVENT_3UP)) {
                paused = 1 - paused;
            } else if (!(state & SWITCH_STATE_SW3) && (timerEvent & BUTTON_EVENT_3DOWN)) {
                paused = 1 - paused;
            } else if ((state & SWITCH_STATE_SW4) && (timerEvent & BUTTON_EVENT_4UP)) {
                paused = 1 - paused;
            } else if (!(state & SWITCH_STATE_SW4) && (timerEvent & BUTTON_EVENT_4DOWN)) {
                paused = 1 - paused;
            }
            timerEvent = BUTTON_EVENT_NONE;
        }
        
        // Similar code to bounce_swtich
        // Checks if timer is true and LED not paused
        if (Timer1.event && !paused) {     // Poll timer events
            LEDS_SET(currentLED);   // Set the led to current state
            
            // Change direction when reached the end
            if (currentLED == leftMost) {
                direction = RIGHT;
            } else if (currentLED == rightMost) {
                direction = LEFT;
            }
            
            // Shift the lit LED to the left or right by 1
            if (direction == RIGHT) {
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

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    Timer1.timeRemaining--;
    if (Timer1.timeRemaining <= 0) {

        Timer1.event = TRUE;
        Timer1.timeRemaining = 100 - ((AdcResult1.voltage / (float) MAX_NUM) * 100);    // Higher % = faster
        
    }
    timerEvent = ButtonsCheckEvents();
}

/**
 * This is the ISR for the ADC1 peripheral.
 */
void __ISR(_ADC_VECTOR, IPL2AUTO) AdcHandler(void) {
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;

    // Same code as in bounce_adc
    uint16_t average = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7) / 8;
    
    if (abs(average - AdcResult1.voltage) >= SIZE) {
        AdcResult1.event = TRUE;
        
        if (average + SIZE >= MAX_NUM) {    // Edge case for trouble reaching max
            AdcResult1.voltage = MAX_NUM;
            
        } else if (average - SIZE <= 0) {   // Edge case for trouble reaching min
            AdcResult1.voltage = 0;
            
        } else {
            AdcResult1.voltage = average;
        }
    }
}