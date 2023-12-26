/*
 * Author: Hasan Abu-Freiha
 * Application: Simple Text Editor
*/
/*
 * (Done) TODO Implement the Data Structure to Use
 * (Done) TODO Check time complexity (Queue)
 * (Done) TODO Implement the dequeue function
 * (Done) TODO Implement the updateData function in an ordered manner
 * TODO Implement the Undo Function
 * TODO Implement the Interface for the application
 */
#include <stdio.h>
#include <stdlib.h>
#include "Headers/Queue.h"
int main() {
    pQueue testQueue = newQueue();
    testQueue->enqueueData(testQueue, "Test x3");
    testQueue->enqueueData(testQueue, "Test x4");
    testQueue->enqueueData(testQueue, "Test x5");

    testQueue->updateData(testQueue, 3, "Test x6");
    char* retData = testQueue->dequeueData(testQueue, "Test Value");
    printf("RetData: %s\n", retData);
    free((void*)retData);
    testQueue->enqueueData(testQueue, "Test x7");
    testQueue->updateData(testQueue, 0, "Test x8");
    testQueue->updateData(testQueue, 6, "Test x9");
    testQueue->updateData(testQueue, 5, "Test x10");
    testQueue->dumpQueue(testQueue);
    testQueue->iterQueue(testQueue);
    testQueue->delQueue(testQueue);

    return 0;
}
