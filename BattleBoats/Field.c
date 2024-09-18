// Author: Nathan Tran (ntran48@ucsc.edu)

#include "Field.h"
#include <stdlib.h>
#include <time.h>

#define SUCCESS 1
#define STANDARD_ERROR 0


void FieldInit(Field *own_field, Field *opp_field) {
    for (int i = 0; i < FIELD_ROWS; i++) {
        for (int j = 0; j < FIELD_COLS; j++) {
            own_field->grid[i][j] = FIELD_SQUARE_EMPTY;
        }
    }
    own_field->smallBoatLives = 0;
    own_field->mediumBoatLives = 0;
    own_field->largeBoatLives = 0;
    own_field->hugeBoatLives = 0;

    // Initialize opponent's field
    for (int i = 0; i < FIELD_ROWS; i++) {
        for (int j = 0; j < FIELD_COLS; j++) {
            opp_field->grid[i][j] = FIELD_SQUARE_UNKNOWN;
        }
    }
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
}

// Retrieve the status of a field square
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col) {
    if (row >= FIELD_ROWS || col >= FIELD_COLS) {
        return FIELD_SQUARE_INVALID;
    }
    return f->grid[row][col];
}

// Set the status of a specific field square and return the previous status
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p) {
    if (row >= FIELD_ROWS || col >= FIELD_COLS) {
        return FIELD_SQUARE_INVALID;
    }
    SquareStatus previousStatus = f->grid[row][col];
    f->grid[row][col] = p;
    return previousStatus;
}

// Add a boat to the field
uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type) {
    uint8_t boatSize;
    SquareStatus boatStatus;

    switch (boat_type) {
        case FIELD_BOAT_TYPE_SMALL:
            boatSize = FIELD_BOAT_SIZE_SMALL;
            boatStatus = FIELD_SQUARE_SMALL_BOAT;
            break;
        case FIELD_BOAT_TYPE_MEDIUM:
            boatSize = FIELD_BOAT_SIZE_MEDIUM;
            boatStatus = FIELD_SQUARE_MEDIUM_BOAT;
            break;
        case FIELD_BOAT_TYPE_LARGE:
            boatSize = FIELD_BOAT_SIZE_LARGE;
            boatStatus = FIELD_SQUARE_LARGE_BOAT;
            break;
        case FIELD_BOAT_TYPE_HUGE:
            boatSize = FIELD_BOAT_SIZE_HUGE;
            boatStatus = FIELD_SQUARE_HUGE_BOAT;
            break;
        default:
            return STANDARD_ERROR;
    }

    // Check if boat fits
    for (uint8_t i = 0; i < boatSize; i++) {
        uint8_t r = row + (dir == FIELD_DIR_SOUTH ? i : 0);
        uint8_t c = col + (dir == FIELD_DIR_EAST ? i : 0);

        if (r >= FIELD_ROWS || c >= FIELD_COLS || own_field->grid[r][c] != FIELD_SQUARE_EMPTY) {
            return STANDARD_ERROR;
        }
    }

    // Place boat
    for (uint8_t i = 0; i < boatSize; i++) {
        uint8_t r = row + (dir == FIELD_DIR_SOUTH ? i : 0);
        uint8_t c = col + (dir == FIELD_DIR_EAST ? i : 0);
        own_field->grid[r][c] = boatStatus;
    }

    // Update boat lives
    switch (boat_type) {
        case FIELD_BOAT_TYPE_SMALL:
            own_field->smallBoatLives = boatSize;
            break;
        case FIELD_BOAT_TYPE_MEDIUM:
            own_field->mediumBoatLives = boatSize;
            break;
        case FIELD_BOAT_TYPE_LARGE:
            own_field->largeBoatLives = boatSize;
            break;
        case FIELD_BOAT_TYPE_HUGE:
            own_field->hugeBoatLives = boatSize;
            break;
    }

    return SUCCESS;
}

// Register an attack on player's field and update guess data with the result
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess) {
    uint8_t row = opp_guess->row;
    uint8_t col = opp_guess->col;

    if (row >= FIELD_ROWS || col >= FIELD_COLS) {
        return FIELD_SQUARE_INVALID;
    }

    SquareStatus status = own_field->grid[row][col];

    switch (status) {
        case FIELD_SQUARE_EMPTY:
            opp_guess->result = RESULT_MISS;
            own_field->grid[row][col] = FIELD_SQUARE_MISS;
            break;

        case FIELD_SQUARE_SMALL_BOAT:
        case FIELD_SQUARE_MEDIUM_BOAT:
        case FIELD_SQUARE_LARGE_BOAT:
        case FIELD_SQUARE_HUGE_BOAT:
            opp_guess->result = RESULT_HIT;
            own_field->grid[row][col] = FIELD_SQUARE_HIT;

            // Decrease boat lives
            if (status == FIELD_SQUARE_SMALL_BOAT) {
                own_field->smallBoatLives--;
                if (own_field->smallBoatLives == 0) {
                    opp_guess->result = RESULT_SMALL_BOAT_SUNK;
                }
            } else if (status == FIELD_SQUARE_MEDIUM_BOAT) {
                own_field->mediumBoatLives--;
                if (own_field->mediumBoatLives == 0) {
                    opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
                }
            } else if (status == FIELD_SQUARE_LARGE_BOAT) {
                own_field->largeBoatLives--;
                if (own_field->largeBoatLives == 0) {
                    opp_guess->result = RESULT_LARGE_BOAT_SUNK;
                }
            } else if (status == FIELD_SQUARE_HUGE_BOAT) {
                own_field->hugeBoatLives--;
                if (own_field->hugeBoatLives == 0) {
                    opp_guess->result = RESULT_HUGE_BOAT_SUNK;
                }
            }
            break;

        default:
            return FIELD_SQUARE_INVALID;
    }

    return status;
}

// Update opponent's field based on the result of player's guess
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess) {
    uint8_t row = own_guess->row;
    uint8_t col = own_guess->col;

    if (row >= FIELD_ROWS || col >= FIELD_COLS) {
        return FIELD_SQUARE_INVALID;
    }

    SquareStatus previousStatus = opp_field->grid[row][col];

    if (own_guess->result == RESULT_HIT || own_guess->result == RESULT_SMALL_BOAT_SUNK ||
        own_guess->result == RESULT_MEDIUM_BOAT_SUNK || own_guess->result == RESULT_LARGE_BOAT_SUNK ||
        own_guess->result == RESULT_HUGE_BOAT_SUNK) {
        opp_field->grid[row][col] = FIELD_SQUARE_HIT;
    } else {
        opp_field->grid[row][col] = FIELD_SQUARE_MISS;
    }

    return previousStatus;
}

// Return alive states of all boats
uint8_t FieldGetBoatStates(const Field *f) {
    uint8_t boatStates = 0;

    if (f->smallBoatLives > 0) {
        boatStates |= FIELD_BOAT_STATUS_SMALL;
    }
    if (f->mediumBoatLives > 0) {
        boatStates |= FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->largeBoatLives > 0) {
        boatStates |= FIELD_BOAT_STATUS_LARGE;
    }
    if (f->hugeBoatLives > 0) {
        boatStates |= FIELD_BOAT_STATUS_HUGE;
    }

    return boatStates;
}

// AI function to place all boats on the player's field
uint8_t FieldAIPlaceAllBoats(Field *own_field) {
    FieldInit(own_field, NULL);     // Initialize field with empty squares

    uint8_t success = 1;
    success &= FieldAddBoat(own_field, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    success &= FieldAddBoat(own_field, 2, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);
    success &= FieldAddBoat(own_field, 4, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_LARGE);
    success &= FieldAddBoat(own_field, 5, 5, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_HUGE);

    return success ? SUCCESS : STANDARD_ERROR;
}

GuessData FieldAIDecideGuess(const Field *opp_field) {
    // Generate random coordinates
    uint8_t row = rand() % FIELD_ROWS;
    uint8_t col = rand() % FIELD_COLS;

    // Check if randomly selected position has already been guessed
    SquareStatus status = FieldGetSquareStatus(opp_field, row, col);
    while (status != FIELD_SQUARE_UNKNOWN) {
        // If the position has been guessed, generate new random coordinates
        row = rand() % FIELD_ROWS;
        col = rand() % FIELD_COLS;
        status = FieldGetSquareStatus(opp_field, row, col);
    }

    // Set result to RESULT_MISS (miss)
    GuessData guess = {row, col, RESULT_MISS};

    return guess;
}
