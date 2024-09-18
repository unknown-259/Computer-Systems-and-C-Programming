// Author: Nathan Tran (ntran48@ucsc.edu)

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>
#include <string.h>

// User libraries
#include "Leds_Lab06.h"
#include "Oled.h"
#include "OledDriver.h"

// **** Set macros and preprocessor directives ****
struct AdcResult {
    uint8_t event;
    uint16_t voltage;
};

// **** Declare any datatypes here ****
static struct AdcResult AdcResult1;
#define SIZE 5
#define MAX_NUM 1023

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

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
    printf("Welcome to ntran48's lab6 part3 (bounce_adc).  Compiled on %s %s.\n",__TIME__,__DATE__);
    OledInit();
    AdcResult1.event = TRUE;
    char stringPrint[35];

    while (1) {
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
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new ADC reading is available
 * in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void) {
    IFS1bits.AD1IF = 0;     // Clear interrupt flag

    // Find the new average reading
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