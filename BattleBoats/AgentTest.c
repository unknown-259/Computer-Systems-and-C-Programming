// Author: Nathan Tran (ntran48@ucsc.edu)

#include "Agent.h"
#include "Message.h"
#include "Field.h"
#include <stdio.h>

#define TEST_NEGOTIATION_PHASE
#define TEST_GAMEPLAY_PHASE

int main() {
    // Initialize the agent
    AgentInit();

    // Simulate turn negotiation phase
    #ifdef TEST_NEGOTIATION_PHASE
    BB_Event negotiation_event;
    negotiation_event.type = BB_EVENT_CHA_RECEIVED;
    Message negotiation_response = AgentRun(negotiation_event);
    printf("Agent sending CHA message.\n");
    if (negotiation_response.type == MESSAGE_CHA) {
        printf("Test 1: Passed\n");
    } else {
        printf("Test 1: Failed\n\n");
    }

    negotiation_event.type = BB_EVENT_ACC_RECEIVED;
    negotiation_response = AgentRun(negotiation_event);
    printf("Agent sending a REV message.\n");
    if (negotiation_response.type == MESSAGE_REV) {
        printf("Test 2: Passed\n");
    } else {
        printf("Test 2: Failed\n\n");
    }
    #endif

    // Simulate gameplay phase
    #ifdef TEST_GAMEPLAY_PHASE
    BB_Event gameplay_event;
    gameplay_event.type = BB_EVENT_SHO_RECEIVED;
    Message gameplay_response = AgentRun(gameplay_event);
    // Simulate the opponent's SHO message
    // Adjust the parameters accordingly based on the gameplay scenario
    printf("Agent sending a SHO message.\n");
    if (gameplay_response.type == MESSAGE_SHO) {
        printf("Test 3: Passed\n");
    } else {
        printf("Test 3: Failed\n\n");
    }

    // Repeat the above steps as needed for more gameplay scenarios
    #endif

    return 0;
}
