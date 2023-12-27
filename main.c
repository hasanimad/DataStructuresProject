/*
 * Author: Hasan Abu-Freiha
 * Application: Simple Text Editor
*/
/*
 * (Done) TODO Implement the Data Structure to Use
 * (Done) TODO Check time complexity (Queue)
 * (Done) TODO Implement the dequeue function
 * (Done) TODO Implement the updateData function in an ordered manner
 * (DONE) TODO Check the iterate function, cause it looks wonky
 * (DONE) TODO Implement the Undo Function
 * (DONE) TODO Link the undo function with the Queue
 * TODO Implement the delete function in the Queue
 * (DONE) TODO Implement the numeric Interface for the application
 * TODO Implement the text Interface for the application
 */
#include <stdio.h>
#include <stdlib.h> // clean
#include <string.h> // clean
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
    }while(mainWrapper(userInput, storedData, storedActionStack) != 1);

//    printf("Data: %s\n\t\\__", storedActionStack->getTopData(storedActionStack, "Default"));
//    printf("Index: %d\n\t\\___", storedActionStack->getTopIndex(storedActionStack));
//    printf("Action: %d\n", storedActionStack->getTopAction(storedActionStack));
    
    storedData->delQueue(storedData);
    storedActionStack->delStack(storedActionStack);
    return 0;
}

