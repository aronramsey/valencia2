#pragma once

#include "platform/m256.h"

#include "network_messages/special_command.h"

#define MAX_NUMBER_OF_SOLUTIONS 65536 // Must be 2^N



static struct System
{
    short version;
    unsigned short epoch;
    unsigned int tick;
    unsigned int initialTick;
    unsigned int latestCreatedTick, latestLedTick;

    unsigned short initialMillisecond;
    unsigned char initialSecond;
    unsigned char initialMinute;
    unsigned char initialHour;
    unsigned char initialDay;
    unsigned char initialMonth;
    unsigned char initialYear;

    unsigned long long latestOperatorNonce;

    // proposal and ballot are replaced by SC
    // TODO: remove completely for epoch 124
    ComputorProposal proposals[NUMBER_OF_COMPUTORS];
    ComputorBallot ballots[NUMBER_OF_COMPUTORS];

    unsigned int numberOfSolutions;
    struct Solution
    {
        m256i computorPublicKey;
        m256i nonce;
    } solutions[MAX_NUMBER_OF_SOLUTIONS];

    m256i futureComputors[NUMBER_OF_COMPUTORS];
} system;
//static_assert(sizeof(System) == 20 + 8 + 8 + 8 + 4 + 64 * MAX_NUMBER_OF_SOLUTIONS + 32 * NUMBER_OF_COMPUTORS, "Unexpected size");
