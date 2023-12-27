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
 * (DONE) TODO Implement the delete function in the Queue
 * (DONE) TODO Implement the numeric Interface for the application
 * TODO Implement the text Interface for the application
 * TODO Add comments explaining what tf does this code do, sincerely
 * TODO Cleanup the code and add DEBUG statements all over it
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
    }while(mainWrapper(userInput, storedData, storedActionStack) != 1);
    storedData->delQueue(storedData);
    storedActionStack->delStack(storedActionStack);
    return 0;
}

