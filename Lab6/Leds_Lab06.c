// Author: Nathan Tran (ntran48@ucsc.edu)

// Standard libraries
#include <stdio.h>
#include <stdlib.h>

// Support libraries
#include "Leds_Lab06.h"
#include "BOARD.h"

void LEDS_INIT(void) {
    // Initialize the LEDs to off
    TRISE = 0x00;
    LATE = 0x00;
}

void LEDS_SET(char newPattern) {
    LATE = newPattern;  // Set LEDs to new pattern
}

char LEDS_GET(void) {
    return LATE;    // Return the current state of LEDs
}