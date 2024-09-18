// Author: Nathan Tran (ntran48@ucsc.edu)

#include "Field.h"
#include <stdio.h>

#define SUCCESS 1
#define TEST_FIELD_INIT
#define TEST_FIELD_ADD_BOAT
#define TEST_FIELD_REGISTER_ENEMY_ATTACK
#define TEST_FIELD_UPDATE_KNOWLEDGE
#define TEST_FIELD_GET_BOAT_STATES
#define TEST_FIELD_AI_PLACE_ALL_BOATS
#define TEST_FIELD_AI_DECIDE_GUESS

int main() {
    // Define test fields
    Field own_field, opp_field;

    // Test FieldInit
    #ifdef TEST_FIELD_INIT
    FieldInit(&own_field, &opp_field);
    #endif

    // Test FieldAddBoat
    #ifdef TEST_FIELD_ADD_BOAT
    printf("\nAdding boats to own field...\n");
    uint8_t add_boat_result = FieldAddBoat(&own_field, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    if (add_boat_result == SUCCESS) {
        printf("Test 1: Passed\n");
    } else {
        printf("Test 1: Failed\n\n");
    }
    #endif

    // Test FieldRegisterEnemyAttack
    #ifdef TEST_FIELD_REGISTER_ENEMY_ATTACK
    printf("\nRegistering enemy attack...\n");
    GuessData opp_guess = {2, 3, RESULT_HIT};
    SquareStatus attack_result = FieldRegisterEnemyAttack(&own_field, &opp_guess);
    if (attack_result == FIELD_SQUARE_HIT) {
        printf("Test 2: Passed\n");
    } else {
        printf("Test 2: Failed\n\n");
    }
    #endif

    // Test FieldUpdateKnowledge
    #ifdef TEST_FIELD_UPDATE_KNOWLEDGE
    printf("\nUpdating opponent's field knowledge...\n");
    GuessData own_guess = {5, 4, RESULT_HIT};
    SquareStatus knowledge_update_result = FieldUpdateKnowledge(&opp_field, &own_guess);
    if (knowledge_update_result == FIELD_SQUARE_UNKNOWN) {
        printf("Test 3: Passed\n");
    } else {
        printf("Test 3: Failed\n\n");
    }
    #endif

    // Test FieldGetBoatStates
    #ifdef TEST_FIELD_GET_BOAT_STATES
    printf("\nGetting boat states...\n");
    uint8_t boat_states = FieldGetBoatStates(&own_field);
    printf("Boat states: %u\n\n", boat_states);
    #endif

    // Test FieldAIPlaceAllBoats
    #ifdef TEST_FIELD_AI_PLACE_ALL_BOATS
    printf("\nPlacing all boats on own field using AI...\n");
    uint8_t ai_place_result = FieldAIPlaceAllBoats(&own_field);
    if (ai_place_result == SUCCESS) {
        printf("Test 4: Passed\n");
    } else {
        printf("Test 4: Failed\n\n");
    }
    #endif

    // Test FieldAIDecideGuess
    #ifdef TEST_FIELD_AI_DECIDE_GUESS
    printf("\nAI deciding guess...\n");
    GuessData ai_guess = FieldAIDecideGuess(&opp_field);
    printf("AI decided guess: Row %u, Col %u\n\n", ai_guess.row, ai_guess.col);
    #endif

    return 0;
}
