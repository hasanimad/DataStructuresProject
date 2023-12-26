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
 * TODO Implement the Undo Function
 * TODO Implement the Interface for the application
 */

#include <stdio.h>
#include <stdlib.h>
#include "Headers/Queue.h"
// testing grounds
int main() {
    pQueue testQueue = newQueue();
    testQueue->enqueueData(testQueue, "Data 0x0");
    testQueue->enqueueData(testQueue, "Data 0x1");
    testQueue->iterQueue(testQueue);
    testQueue->delQueue(testQueue);
    return 0;
}
