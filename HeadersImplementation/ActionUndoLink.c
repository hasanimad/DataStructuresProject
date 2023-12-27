//
// Created by sdds on 12/27/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../Headers/ActionUndoLink.h"

char *__initializeSingleData(char **args) {
    if (args[1] != NULL && args[2] != NULL) {
        char *data = (char *) malloc(strlen(args[1]) + strlen(args[2]) + 2);
        if (!data) return NULL;
        strcpy(data, args[1]);
        strcat(data, " ");
        strcat(data, args[2]);
        return data;
    } else return args[1];

}

void enQueueData(char **args, pQueueImpl storedData, pActionStack storedActionStack) {
    char *dataToStore;
    pAction enqueueAction;

    dataToStore = __initializeSingleData(args);
    if (!dataToStore) return;

    enqueueAction = newAction(
            INSERT,
            storedData->enqueueData(storedData, dataToStore),
            dataToStore
    );

    storedActionStack->push(storedActionStack, enqueueAction);

    free((void *) dataToStore);
}

void deQueueData(char **args, pQueueImpl storedData, pActionStack storedActionStack) {
    char *dataToStore,
            *temp;
    int headIndex;
    pAction dequeueAction;

    dataToStore = __initializeSingleData(args);
    headIndex = storedData->getHeadIndex(storedData);
    temp = storedData->dequeueData(storedData, dataToStore);
    if (headIndex != -1) {
        dequeueAction = newAction(DELETE, headIndex, temp);
        storedActionStack->push(storedActionStack, dequeueAction);
    }
    printf("%s\n", temp);
    free((void *) temp);
}

void modifyData(char **args, pQueueImpl storedData, pActionStack storedActionStack) {
    pAction modifyAction;

    storedData->updateData(storedData, (int) strtol(args[1], NULL, 10), args[2], true);

    modifyAction = newAction(MODIFY, (int) strtol(args[1], NULL, 10), args[2]);
    storedActionStack->push(storedActionStack, modifyAction);
}

void getQueueData(char **args, pQueueImpl storedData) {
    printf("%s\n", storedData->getQueue(storedData, (int) strtol(args[1], NULL, 10), args[2]));
}

void iterateQueue(pQueueImpl storedData) {
    storedData->iterQueue(storedData);
}

void peekEnd(char **args, pQueueImpl storedData) {
    char *dataToStore = __initializeSingleData(args);
    printf("%s\n", storedData->getTail(storedData, dataToStore));
    free((void *) dataToStore);
}

void peekHead(char **args, pQueueImpl storedData) {
    char *dataToStore = __initializeSingleData(args);
    printf("%s\n", storedData->getHead(storedData, dataToStore));
    free((void *) dataToStore);
}

void insertFront(char **args, pQueueImpl storedData, pActionStack storedActionStack) {
    char *dataToStore;
    pAction insertAction;

    dataToStore = __initializeSingleData(args);
    if (!dataToStore) return;

    insertAction = newAction(INSERT, storedData->putHead(storedData, dataToStore, true), dataToStore);

    storedActionStack->push(storedActionStack, insertAction);
    free((void *) dataToStore);
}

void countElements(pQueueImpl storedData) {
    printf("Number of sentences: %d\n", storedData->sizeQueue(storedData));
}

void undoAction(pQueueImpl storedData, pActionStack storedActionStack) {

    pAction prevAction = storedActionStack->pop(storedActionStack);
    if(prevAction == NULL) return;
    int prevIndex = prevAction->getIndex(prevAction);
    char *prevData = prevAction->getData(prevAction);
    switch (prevAction->getAction(prevAction)) {
        case INSERT:
            storedData->deleteData(storedData, prevIndex);
            break;
        case DELETE:
            storedData->updateData(storedData, prevIndex, prevData, false);
            break;
        case MODIFY:

            break;
    }
    free((void*)prevData);
    free((void*)prevAction);
}