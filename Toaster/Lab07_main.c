// Author: Nathan Tran (ntran48@ucsc.edu)

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CSE13E Support Library
#include "Adc.h"
#include "Ascii.h"
#include "BOARD.h"
#include "Buttons.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>



// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)


// **** Set any local typedefs here ****
typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, ALERT
} OvenState;

// Modes of the toaster
typedef enum {
    BAKE, TOAST, BROIL
} CookingStates;

typedef struct {
    OvenState state;
    //add more members to this struct
    uint8_t mode;               // Mode of the toaster
    uint16_t temperature;       // Temperature setting
    uint16_t cookTime;          // Store cooking time
    uint16_t timeRemaining;     // Remaining time
    uint16_t pressTime;         // Measures how long a button was pressed
} OvenData;

// **** Declare any datatypes here ****
OvenData ovenData;
uint16_t TIMER_TICK = FALSE;                // Checks if timer event occurred
uint8_t buttonState = BUTTON_EVENT_NONE;    // Status of button events
uint16_t runningTime = 0;                   // Measures elapsed time
uint8_t adcChanged = FALSE;                 // Checks if ADC has changed
uint16_t adcRead;                           // Value read from ADC
uint8_t tempChanging = FALSE;               // Checks if changing temp or time
uint16_t cookingStartTime = 0;              // Stored value of runningTime
uint8_t invertedDisplay = FALSE;            // Display mode of OLED
uint16_t remainder;                         // Keep track of remainders
uint16_t temp;                              // Temperature when switching between modes
uint16_t ledUpdateCounter = 0;              // Keep track of number of timer ticks
uint16_t ledFrequency;                      // Frequency of LED bar updates
char ledState;                              // Current state of LED bar

// **** Define any module-level, global, or external variables here ****
#define TICKS_PER_SEC 5
#define SECONDS 60
#define DEGREES 300
#define DEFAULT_TEMP 350
#define BROIL_TEMP 500
#define LEDS_ON 0xFF
#define LEDS_OFF 0x00

// **** Put any helper functions here ****
// Format  oven display string
void formatOvenDisplayString(char* toPrint, const char* modeStr, const char* topState, const char* bottomState, int time, int temp, int isTempChanging, int showTemp) {
    if (showTemp) {
        if (isTempChanging) {
            sprintf(toPrint, "|%s| Mode: %s\n|     |  Time: %d:%02d\n|-----| >Temp: %d%sF\n|%s|",
                    topState, modeStr, time / SECONDS, time % SECONDS, temp, DEGREE_SYMBOL, bottomState);
        } else {
            sprintf(toPrint, "|%s| Mode: %s\n|     | >Time: %d:%02d\n|-----|  Temp: %d%sF\n|%s|",
                    topState, modeStr, time / SECONDS, time % SECONDS, temp, DEGREE_SYMBOL, bottomState);
        }
    // Formate string without ">" indicator
    } else {
        sprintf(toPrint, "|%s| Mode: %s\n|     |  Time: %d:%02d\n|-----|\n|%s|",
                topState, modeStr, time / SECONDS, time % SECONDS, bottomState);
    }
}

// Get the display string based on the oven mode and state
void getOvenDisplayString(char* toPrint, OvenData ovenData) {
    // Define strings for heating element states
    char upperActive[30], upperInactive[30], lowerActive[30], lowerInactive[30];
    sprintf(upperActive, "%s%s%s%s%s", OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON);
    sprintf(upperInactive, "%s%s%s%s%s", OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF);
    sprintf(lowerActive, "%s%s%s%s%s", OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON);
    sprintf(lowerInactive, "%s%s%s%s%s", OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF);

    // Determine state of elements
    const char* topState = (ovenData.state == COOKING || ovenData.state == RESET_PENDING) ? upperActive : upperInactive;
    const char* bottomState = (ovenData.state == COOKING || ovenData.state == RESET_PENDING) ? lowerActive : lowerInactive;

    // Generate string based on oven mode
    switch (ovenData.mode) {
        case BAKE:
            formatOvenDisplayString(toPrint, "Bake", topState, bottomState,
                (ovenData.state == COOKING || ovenData.state == RESET_PENDING) ? ovenData.timeRemaining : ovenData.cookTime,
                ovenData.temperature, tempChanging, 1);
            break;
        case TOAST:
            formatOvenDisplayString(toPrint, "Toast", topState, bottomState,
                (ovenData.state == COOKING || ovenData.state == RESET_PENDING) ? ovenData.timeRemaining : ovenData.cookTime,
                0, 0, 0);
            break;
        case BROIL:
            if (ovenData.state == COOKING || ovenData.state == RESET_PENDING) {
                sprintf(toPrint, "|%s| Mode: Broil\n|     |  Time: %d:%02d\n|-----|  Temp: 500%sF\n|%s|",
                topState, ovenData.timeRemaining / SECONDS, ovenData.timeRemaining % SECONDS, DEGREE_SYMBOL, bottomState);
            } else {
                sprintf(toPrint, "|%s| Mode: Broil\n|     |  Time: %d:%02d\n|-----|  Temp: 500%sF\n|%s|",
                topState, ovenData.cookTime / SECONDS, ovenData.cookTime % SECONDS, DEGREE_SYMBOL, bottomState);
            }
            break;
    }
}

/*This function will update your OLED to reflect the state .*/
// Main function to update the OLED display
void updateOvenOLED(OvenData ovenData) {
    char toPrint[120] = "";

    // Get the display string
    getOvenDisplayString(toPrint, ovenData);

    // Clear OLED, draw the string, and update display
    OledClear(0);
    OledDrawString(toPrint);

    // Extra credit state for inversion display
    if (ovenData.state == ALERT) {
        if (invertedDisplay) {
            OledSetDisplayNormal();
        } else {
            OledSetDisplayInverted();
        }
    }
    OledUpdate();
}


/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void)
{
    switch (ovenData.state) {
        case RESET_PENDING:
            // Handle timer ticks while determining if timer reset is needed
            if (TIMER_TICK) {
                ledUpdateCounter++;
                if ((remainder > 0 && ledUpdateCounter == ledFrequency + 1) || (remainder == 0 && ledUpdateCounter == ledFrequency)) {
                    LEDS_SET(LEDS_GET() << 1);
                    ledUpdateCounter = 0;
                    remainder--;
                }

                // Update remaining time every second
                if (ovenData.state == COOKING && (runningTime - cookingStartTime) % 5 == 0) {
                    ovenData.timeRemaining = (ovenData.timeRemaining > 0) ? ovenData.timeRemaining - 1 : 0;
                    updateOvenOLED(ovenData);
                }

                // Check if button is held long enough to reset the timer
                if (ovenData.state == RESET_PENDING && runningTime - ovenData.pressTime >= TICKS_PER_SEC) {
                    ovenData.timeRemaining = ovenData.cookTime; // Reset timer
                    ovenData.state = SETUP;
                    updateOvenOLED(ovenData);
                    LEDS_SET(LEDS_OFF);
                }
            }
            
            // Button 4 released, return to COOKING state if not held long enough
            if (buttonState & BUTTON_EVENT_4DOWN) {
                if (ovenData.state == COOKING) {
                    ovenData.state = RESET_PENDING;
                    ovenData.pressTime = runningTime;
                } else if (ovenData.state == RESET_PENDING && (runningTime - ovenData.pressTime < TICKS_PER_SEC)) {
                    ovenData.state = COOKING;
                }
            }
            break;
            
        case COOKING:
            // Handle timer ticks
            if (TIMER_TICK) {
                ledUpdateCounter++;
                // Update LEDs based on ledFrequency and remainder
                if ((remainder > 0 && ledUpdateCounter == ledFrequency + 1) || (remainder == 0 && ledUpdateCounter == ledFrequency)) {
                    LEDS_SET(LEDS_GET() << 1); // Shift LEDs
                    ledUpdateCounter = 0;
                    remainder--;
                }

                // Check if cooking is done
                if (ovenData.timeRemaining == 0) {
                    ovenData.timeRemaining = ovenData.cookTime;
                    ovenData.state = ALERT;
                    updateOvenOLED(ovenData);
                    LEDS_SET(LEDS_OFF);
                    break;
                }

                // Update remaining time every second
                if ((runningTime - cookingStartTime) % 5 == 0) {
                    ovenData.timeRemaining--;
                    updateOvenOLED(ovenData);
                }
            }
            
            // Button 4 pressed
            if (buttonState & BUTTON_EVENT_4DOWN) {
                ovenData.state = RESET_PENDING;
                ovenData.pressTime = runningTime;
            }
            break;
            
        case SELECTOR_CHANGE_PENDING:
            // Button 3 released, determine action based on hold duration
            if (buttonState & BUTTON_EVENT_3UP) {
                uint16_t elapsedTime = runningTime - ovenData.pressTime;
                // Toggle between editing temperature and time
                if ((elapsedTime >= TICKS_PER_SEC) && (ovenData.mode == BAKE)) {
                        tempChanging = !tempChanging;
                } else {
                    // Change cooking mode
                    ovenData.mode = (ovenData.mode == BROIL) ? BAKE : ovenData.mode + 1;
                    if (ovenData.mode == BROIL) {
                        temp = (tempChanging) ? temp : ovenData.temperature;
                        ovenData.temperature = (tempChanging) ? 500 : temp;
                    }
                }
                updateOvenOLED(ovenData);
                ovenData.state = SETUP;
            }
            break;
            
        case SETUP:
            if (adcChanged) {
                uint16_t adcRead = AdcRead();
                adcRead = (adcRead & 0x03FC) >> 2;  // Extract the top 8 bits from the 10-bit ADC reading
                
                // Update temperature or cooking time
                if (ovenData.mode == BAKE && tempChanging) {
                    ovenData.temperature = adcRead + DEGREES;   // Adjust temperature
                } else {
                    ovenData.cookTime = adcRead + 1;    // Adjust cook time
                    ovenData.timeRemaining = ovenData.cookTime;
                }
                updateOvenOLED(ovenData);
            }
            
            // Button 3 pressed
            if (buttonState & BUTTON_EVENT_3DOWN) {
                ovenData.pressTime = runningTime;
                ovenData.state = SELECTOR_CHANGE_PENDING;
            // Button 4 pressed
            } else if (buttonState & BUTTON_EVENT_4DOWN) {
                cookingStartTime = runningTime;
                ovenData.state = COOKING;
                updateOvenOLED(ovenData);
                LEDS_SET(LEDS_ON);
                ledFrequency = (ovenData.cookTime * 5) / 8;     // Calculate LED update frequency
                remainder = (ovenData.cookTime * 5) % 8;    // Calculate remainder for LED updates
                ledUpdateCounter = 0;
            }
            break;

        case ALERT:
            // Flash display to indicate cooking is done
            if (TIMER_TICK) {
                invertedDisplay = !invertedDisplay; // Toggle display inversion
                updateOvenOLED(ovenData);
            }

            // Button 4 released, reset display and return to SETUP state
            if (buttonState & BUTTON_EVENT_4UP) {
                invertedDisplay = TRUE; // Ensure display is not inverted
                updateOvenOLED(ovenData);
                ovenData.state = SETUP;
            }
            break;
    }
}


int main()
{
    BOARD_Init();

     //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">
    
    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>
   
    printf("Welcome to ntran48's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here
    
    ovenData.mode = BAKE;
    ovenData.state = SETUP;
    ovenData.pressTime = 0;
    ovenData.timeRemaining = 1;
    ovenData.cookTime = 1;
    ovenData.temperature = DEFAULT_TEMP;
    
    AdcInit();
    OledInit();
    ButtonsInit();
    LEDS_INIT();
    
    updateOvenOLED(ovenData);
    
    while (1){
        if (buttonState != BUTTON_EVENT_NONE || buttonState != TIMER_TICK || buttonState != adcChanged) {
            runOvenSM();
            buttonState = BUTTON_EVENT_NONE;
            TIMER_TICK = FALSE;
            adcChanged = FALSE;
        }
    };
}


/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    //add event-checking code here
    TIMER_TICK = TRUE;
    runningTime++;
}


/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //add event-checking code here
    adcChanged = AdcChanged();
    buttonState = ButtonsCheckEvents();
}