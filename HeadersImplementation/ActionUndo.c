//
// Created by sdds on 12/25/2023.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Headers/ActionUndo.h"
#include  "../Headers/strux.h"


struct ActionOG {
    enum actionDone actionDone;
    int index;
    char *data;
    pAction __next;
};
struct ActionStack {
    pAction __top;
    int __capacity;
};

char *__getActionData(pAction self) {
    return self->action->data;

}

int __getActionIndex(pAction self) {
    return self->action->index;
}

enum actionDone __getActionType(pAction self) {
    return self->action->actionDone;
}

void __delAction(pAction self) {
    DEBUG_INFO("Entered __delAction\n");
    free((void *) self->action->data);
    free((void *) self->action);
    free((void *) self);
    DEBUG_INFO("Exiting __delAction...\n");
}

void __push(pActionStack self, pAction doneAction) {
    DEBUG_INFO("Entered __push function\n");
    doneAction->action->__next = self->__actionStack->__top;
    self->__actionStack->__top = doneAction;
    DEBUG_INFO("Exiting __push function...\n");
}

pAction __pop(pActionStack self) {
    DEBUG_INFO("Entered __pop function\n");
    if (self->__actionStack->__top == NULL) return NULL;
    pAction poppedAction = self->__actionStack->__top;
    self->__actionStack->__top = self->__actionStack->__top->action->__next;
    poppedAction->action->__next = NULL;
    DEBUG_INFO("Exiting __pop  function...\n");
    return poppedAction;
}

void __delStack(pActionStack self) {
    DEBUG_INFO("Entered __delStack\n");
    if (self->__actionStack->__top != NULL) {
        pAction current = self->__actionStack->__top;
        pAction stored;
        while (current->action->__next != NULL) {
            DEBUG_INFO("Found More than one action\n\t\\___ Deleting...\n");
            stored = current->action->__next;
            __delAction(current);
            current = stored;
        }
        free((void *) current);
    }
    free((void *) self->__actionStack);
    free((void *) self);
    self = NULL;
    DEBUG_INFO("Exiting __delStack...\n");
}

int __getTopIndex(pActionStack self) {
    return self->__actionStack->__top->getIndex(self->__actionStack->__top);
}

char *__getTopData(pActionStack self, char *def) {
    if (self->__actionStack->__top->getData(self->__actionStack->__top) == NULL) return def;
    return self->__actionStack->__top->getData(self->__actionStack->__top);
}

enum actionDone __getTopAction(pActionStack self) {
    return self->__actionStack->__top->getAction(self->__actionStack->__top);
}

pAction newAction(enum actionDone actionType, int index, char *data) {

    pAction newAct = (pAction) malloc(sizeof(*newAct));
    if (!newAct) return NULL;
    newAct->action = (pActionOG) malloc(sizeof(*(newAct->action)));
    if (newAct->action == NULL) {
        free((void *) newAct);
        return newAct;
    }

    newAct->action->actionDone = actionType;
    newAct->action->index = index;
    newAct->action->data = (char *) malloc(strlen(data) + 1);
    strcpy(newAct->action->data, data);
    newAct->action->__next = NULL;

    newAct->getAction = &__getActionType;
    newAct->getData = &__getActionData;
    newAct->getIndex = &__getActionIndex;
    newAct->delAction = &__delAction;
    return newAct;
}

pActionStack newActionStack() {
    pActionStack newStack = (pActionStack) malloc(sizeof(*newStack));
    newStack->__actionStack = (pActionStackOG) malloc(sizeof(*(newStack->__actionStack)));
    newStack->__actionStack->__top = NULL;
    newStack->__actionStack->__capacity = 10;

    newStack->getTopAction = &__getTopAction;
    newStack->getTopIndex = &__getTopIndex;
    newStack->getTopData = &__getTopData;
    newStack->push = &__push;
    newStack->pop = &__pop;
    newStack->delStack = &__delStack;
    return newStack;
}
