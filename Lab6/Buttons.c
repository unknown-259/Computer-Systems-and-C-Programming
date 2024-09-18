// Author: Nathan Tran (ntran48@ucsc.edu)

#include <stdint.h>
#include <genericTypeDefs.h>

// We rely on this file to define various macros for working with the hardware buttons.
#include "BOARD.h"
#include "Buttons.h"

static uint8_t debounce;
static uint8_t previous;


void ButtonsInit(void) {
    // Buttons set to input
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
    
    debounce = 0;
    previous = 0x00;
}


uint8_t ButtonsCheckEvents(void) {
    uint8_t buttonStatus = BUTTON_EVENT_NONE;
    uint8_t state = BUTTON_STATES();
    
    // Check if status stayed the same
    if (state == previous) {
        return BUTTON_EVENT_NONE;
    }
    
    // Don't check for events if debounce > 0
    if (debounce > 0) {
        debounce--;
        return buttonStatus;    // Returns no event
    }
    
    // Updates buttonStatus based on changes in button position
    buttonStatus |= ((state & BUTTON_STATE_1) && !(previous & BUTTON_STATE_1)) ? BUTTON_EVENT_1DOWN : 0;
    buttonStatus |= (!(state & BUTTON_STATE_1) && (previous & BUTTON_STATE_1)) ? BUTTON_EVENT_1UP : 0;

    buttonStatus |= ((state & BUTTON_STATE_2) && !(previous & BUTTON_STATE_2)) ? BUTTON_EVENT_2DOWN : 0;
    buttonStatus |= (!(state & BUTTON_STATE_2) && (previous & BUTTON_STATE_2)) ? BUTTON_EVENT_2UP : 0;

    buttonStatus |= ((state & BUTTON_STATE_3) && !(previous & BUTTON_STATE_3)) ? BUTTON_EVENT_3DOWN : 0;
    buttonStatus |= (!(state & BUTTON_STATE_3) && (previous & BUTTON_STATE_3)) ? BUTTON_EVENT_3UP : 0;

    buttonStatus |= ((state & BUTTON_STATE_4) && !(previous & BUTTON_STATE_4)) ? BUTTON_EVENT_4DOWN : 0;
    buttonStatus |= (!(state & BUTTON_STATE_4) && (previous & BUTTON_STATE_4)) ? BUTTON_EVENT_4UP : 0;

    previous = state;
    debounce = BUTTONS_DEBOUNCE_PERIOD;
    return buttonStatus;
}