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
void getQueueData(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void iterateQueue(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void peekEnd(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void peekHead(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void insertFront(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void undoAction(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void countElements(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void printDiagnosticData(char** args, pQueueImpl storedData, pActionStack storedActionStack);
void removeAll(char** args, pQueueImpl storedData, pActionStack storedActionStack);
#endif //DATASTRUCTURESPROJECT_ACTIONUNDOLINK_H
