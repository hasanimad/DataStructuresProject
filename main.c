/*
 * Author: Hasan Abu-Freiha
 * Application: Simple Text Editor
*/

#include <stdio.h>
#include "Headers/MainWrapper.h"

#define MAX_INPUT 1018 // Initial Instruction <= 13, Index 3-digits, 2 spaces, data <= 1000, 1 NULL byte

int main() {
    pQueueImpl storedData = newQueue();
    pActionStack storedActionStack = newActionStack();
    char userInput[MAX_INPUT];
    printMenu();
    do {
        printf("$> ");
        fgets(userInput, MAX_INPUT - 1, stdin);
    } while (mainWrapper(userInput, storedData, storedActionStack) != 1);
    storedData->delQueue(storedData);
    storedActionStack->delStack(storedActionStack);
    return 0;
}

