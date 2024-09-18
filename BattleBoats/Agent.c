// Author: Nathan Tran (ntran48@ucsc.edu)

#include "Agent.h"
#include "Message.h"
#include "BattleBoats.h"
#include "Field.h"
#include <stdlib.h>

// Define constants if not already defined
#define RESULT_HIT 1
#define RESULT_MISS 0

// Define missing constants
#define SHIP_SUNK_NONE 0
#define HIT 1
#define MISS 0

static AgentState currentState; // Current state of the agent
static uint8_t turnCounter;
static Field ownField; // Player's field
static Field oppField; // Opponent's field

void AgentInit(void) {
    currentState = AGENT_STATE_START;
    turnCounter = 0;
}

Message AgentRun(BB_Event event) {
    Message messageToSend;
    messageToSend.type = MESSAGE_NONE;

    // Determine the next state based on the current state and received event
    switch (currentState) {
        case AGENT_STATE_START:
            currentState = AGENT_STATE_CHALLENGING; // Next state
            messageToSend.type = MESSAGE_CHA;
            break;

        case AGENT_STATE_CHALLENGING:
            if (event.type == BB_EVENT_ACC_RECEIVED) {
                currentState = AGENT_STATE_SETUP_BOATS; // Setup boats state
                messageToSend.type = MESSAGE_REV;
            }
            break;

        case AGENT_STATE_SETUP_BOATS:
            if (event.type == BB_EVENT_REV_RECEIVED) {
                currentState = AGENT_STATE_ATTACKING; // Attacking state

                // Attack randomly
                messageToSend.type = MESSAGE_SHO;
                messageToSend.param0 = rand() % FIELD_ROWS; // Random row
                messageToSend.param1 = rand() % FIELD_COLS; // Random column
            }
            break;

        case AGENT_STATE_ATTACKING:
            if (event.param0 == RESULT_HIT) {
                FieldSetSquareStatus(&oppField, event.param1, event.param2, FIELD_SQUARE_HIT);

                // Check if any ship has been sunk and update field
                if (event.param2 != SHIP_SUNK_NONE) {
                    switch (event.param2) {
                        case RESULT_SMALL_BOAT_SUNK:
                            // Update small boat to sunk status
                            for (uint8_t i = 0; i < FIELD_ROWS; i++) {
                                for (uint8_t j = 0; j < FIELD_COLS; j++) {
                                    if (FieldGetSquareStatus(&oppField, i, j) == FIELD_SQUARE_SMALL_BOAT) {
                                        FieldSetSquareStatus(&oppField, i, j, FIELD_SQUARE_HIT);
                                    }
                                }
                            }
                            break;
                            
                        case RESULT_MEDIUM_BOAT_SUNK:
                            // Update medium boat to sunk status
                            for (uint8_t i = 0; i < FIELD_ROWS; i++) {
                                for (uint8_t j = 0; j < FIELD_COLS; j++) {
                                    if (FieldGetSquareStatus(&oppField, i, j) == FIELD_SQUARE_MEDIUM_BOAT) {
                                        FieldSetSquareStatus(&oppField, i, j, FIELD_SQUARE_HIT);
                                    }
                                }
                            }
                            break;
                            
                        case RESULT_LARGE_BOAT_SUNK:
                            // Update large boat to sunk status
                            for (uint8_t i = 0; i < FIELD_ROWS; i++) {
                                for (uint8_t j = 0; j < FIELD_COLS; j++) {
                                    if (FieldGetSquareStatus(&oppField, i, j) == FIELD_SQUARE_LARGE_BOAT) {
                                        FieldSetSquareStatus(&oppField, i, j, FIELD_SQUARE_HIT);
                                    }
                                }
                            }
                            break;
                            
                        case RESULT_HUGE_BOAT_SUNK:
                            // Update huge boat to sunk status
                            for (uint8_t i = 0; i < FIELD_ROWS; i++) {
                                for (uint8_t j = 0; j < FIELD_COLS; j++) {
                                    if (FieldGetSquareStatus(&oppField, i, j) == FIELD_SQUARE_HUGE_BOAT) {
                                        FieldSetSquareStatus(&oppField, i, j, FIELD_SQUARE_HIT);
                                    }
                                }
                            }
                            break;
                    }
                }
            } else if (event.param0 == RESULT_MISS) {
                FieldSetSquareStatus(&oppField, event.param1, event.param2, FIELD_SQUARE_MISS);
            }

            if (FieldGetBoatStates(&ownField) == 0) {   // Game over
                currentState = AGENT_STATE_END_SCREEN;  // End screen state
            } else {
                currentState = AGENT_STATE_DEFENDING;   // Defending state
            }
            break;


        case AGENT_STATE_DEFENDING:
            if (event.type == BB_EVENT_SHO_RECEIVED) {
                // Process the attack from the opponent
                GuessData oppGuess;
                oppGuess.row = event.param0;
                oppGuess.col = event.param1;
                FieldRegisterEnemyAttack(&ownField, &oppGuess);

                // Respond to the opponent's attack
                messageToSend.type = MESSAGE_RES;
                messageToSend.param0 = oppGuess.row;
                messageToSend.param1 = oppGuess.col;
                messageToSend.param2 = oppGuess.result;

                // Check if any boats were sunk or if the game is over
                if (FieldGetBoatStates(&ownField) == 0) {
                    currentState = AGENT_STATE_END_SCREEN;
                } else {
                    currentState = AGENT_STATE_WAITING_TO_SEND;
                }
            }
            break;

        case AGENT_STATE_WAITING_TO_SEND:
            if (event.type == BB_EVENT_SHO_RECEIVED) {
                currentState = AGENT_STATE_ATTACKING; // Attacking state
            }
            break;

        case AGENT_STATE_END_SCREEN:
            if (event.type == BB_EVENT_RESET_BUTTON) {
                AgentInit(); // Reset game state
            }
            break;
    }

    return messageToSend;
}

AgentState AgentGetState(void) {
    return currentState;
}

void AgentSetState(AgentState newState) {
    currentState = newState;
}
