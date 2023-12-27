//
// Created by sdds on 12/27/2023.
//

#ifndef DATASTRUCTURESPROJECT_MAINWRAPPER_H
#define DATASTRUCTURESPROJECT_MAINWRAPPER_H

#include "../Headers/Queue.h"
#include "../Headers/ActionUndo.h"

void printMenu();
int mainWrapper(char* userInput, pQueueImpl DataQueue, pActionStack ActionStack);

#endif //DATASTRUCTURESPROJECT_MAINWRAPPER_H
