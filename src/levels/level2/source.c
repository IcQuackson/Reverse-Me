#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void no(void) {
    printf("Nope.\n");
    exit(1);
}

void ok(void) {
    printf("Good job.\n");
}

int main(void) {
    char inputBuffer[24];
    char decodedBuffer[9];
    char tripleDigitBuffer[4];
    int scanfResult;
    size_t inputIndex;
    size_t decodedIndex;
    int cmpResult;

    printf("Please enter key: ");
    scanfResult = scanf("%23s", inputBuffer);  // Limit input to avoid overflow

    if (scanfResult != 1) {
        no();
    }

    // The first two characters must be '0'
    if (inputBuffer[0] != '0' || inputBuffer[1] != '0') {
        no();
    }

    fflush(stdin);

    // Initialize the decoded buffer
    memset(decodedBuffer, 0, sizeof(decodedBuffer));
    decodedBuffer[0] = (char)100; // 'd'

    inputIndex = 2;      // We start reading triple-digits from input[2]
    decodedIndex = 1;     // We already have 'd' at position 0

    // The goal is to decode until we form "delabere" (8 chars: d e l a b e r e)
    // decodedBuffer[0] = 'd', we need 7 more chars: e l a b e r e
    while (true) {
        size_t decodedLengthSoFar = strlen(decodedBuffer);
        bool canContinue = false;

        if (decodedLengthSoFar < 8) {
            size_t inputLength = strlen(inputBuffer);
            canContinue = inputIndex < inputLength;
        }

        if (!canContinue) {
            break;
        }

        // Read three characters from the input
        if (inputIndex + 2 >= strlen(inputBuffer)) {
            // Not enough characters left for a triple-digit code
            break;
        }

        tripleDigitBuffer[0] = inputBuffer[inputIndex];
        tripleDigitBuffer[1] = inputBuffer[inputIndex + 1];
        tripleDigitBuffer[2] = inputBuffer[inputIndex + 2];
        tripleDigitBuffer[3] = '\0';

        int asciiValue = atoi(tripleDigitBuffer);
        decodedBuffer[decodedIndex] = (char)asciiValue;

        inputIndex += 3;
        decodedIndex += 1;
    }

    decodedBuffer[decodedIndex] = '\0';

    cmpResult = strcmp(decodedBuffer, "delabere");
    if (cmpResult == 0) {
        ok();
    } else {
        no();
    }

    return 0;
}
