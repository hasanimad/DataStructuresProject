//
// Created by sdds on 12/27/2023.
//

#ifndef DATASTRUCTURESPROJECT_ACTIONUNDOLINK_H
#define DATASTRUCTURESPROJECT_ACTIONUNDOLINK_H

#include "Queue.h"
#include "ActionUndo.h"

void enQueueData(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void deQueueData(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void modifyData(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void getQueueData(char** args, pQueueImpl storedData);
void iterateQueue(pQueueImpl storedData);
void peekEnd(char** args, pQueueImpl storedData);
void peekHead(char** args,pQueueImpl storedData);
void insertFront(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void undoAction(pQueueImpl storedData, pActionStack storedActionStack);
void countElements(pQueueImpl storedData);
#endif //DATASTRUCTURESPROJECT_ACTIONUNDOLINK_H
