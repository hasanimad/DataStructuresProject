//
// Created by sdds on 12/25/2023.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Headers/ActionUndo.h"
#include  "../Headers/strux.h"



struct ActionOG{
    enum actionDone actionDone;
    int index;
    char* data;
    pAction __next;
};
struct ActionStack{
    pAction __top;
    int __capacity;
};
char* __getActionData(pAction self){
    return self->action->data;

}
int __getActionIndex(pAction  self){
    return self->action->index;
}
enum actionDone __getActionType(pAction self){
    return self->action->actionDone;
}
void __delAction(pAction self){
    DEBUG_INFO("Entered __delAction\n");
    free((void*)self->action->data);
    free((void*)self->action);
    free((void*)self);
    DEBUG_INFO("Exiting __delAction...\n");
}
void __push(pActionStack self, pAction doneAction){
    DEBUG_INFO("Entered __push function\n");
    doneAction->action->__next = self->__actionStack->__top;
    self->__actionStack->__top = doneAction;
    DEBUG_INFO("Exiting __push function...\n");
}
pAction __pop(pActionStack self) {
    DEBUG_INFO("Entered __pop function\n");
    pAction poppedAction = self->__actionStack->__top;
    self->__actionStack->__top = self->__actionStack->__top->action->__next;
    poppedAction->action->__next = NULL;
    DEBUG_INFO("Exiting __pop  function...\n");
    return poppedAction;
}
void __delStack(pActionStack self){
    DEBUG_INFO("Entered __delStack\n");
    free((void*)self->__actionStack->__top);
    free((void*)self->__actionStack);
    free((void*)self);
    DEBUG_INFO("Exiting __delStack...\n");
}
pAction newAction(enum actionDone actionType, int index, char* data) {
    pAction newAct = (pAction)malloc(sizeof(*newAct));
    if (!newAct) return NULL;
    newAct->action = (pActionOG)malloc(sizeof(*(newAct->action)));
    if(newAct->action == NULL){
        free((void*)newAct);
        return newAct;
    }

    newAct->action->actionDone = actionType;
    newAct->action->index = index;
    newAct->action->data = (char*)malloc(strlen(data)+1);
    strcpy(newAct->action->data, data);
    newAct->action->__next = NULL;

    newAct->getAction = &__getActionType;
    newAct->getData = &__getActionData;
    newAct->getIndex = &__getActionIndex;
    newAct->delAction = &__delAction;
    return newAct;
}
pActionStack newActionStack(){
    pActionStack newStack = (pActionStack)malloc(sizeof(*newStack));
    newStack->__actionStack = (pActionStackOG)malloc(sizeof(*(newStack->__actionStack)));
    newStack->__actionStack->__top = NULL;
    newStack->__actionStack->__capacity = 10;

    newStack->push = &__push;
    newStack->pop = &__pop;
    newStack->delStack = &__delStack;
    return newStack;
}
