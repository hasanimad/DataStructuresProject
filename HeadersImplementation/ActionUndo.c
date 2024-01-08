//
// Created by sdds on 12/25/2023.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Headers/ActionUndo.h"
#include  "../Headers/strux.h"

// Original Action Class
struct ActionOG {
    enum actionDone actionDone;
    int index;
    char *data;
    pAction __next;
};
// Original ActionStack Class
struct ActionStack {
    pAction __top;
    int __capacity;
    int __count;
};
// Method to retrieve the data from an action
char *__getActionData(pAction self) {
    return self->action->data;

}
// Method to retrieve the index from an action
int __getActionIndex(pAction self) {
    return self->action->index;
}
// Method to retrieve the action type from an action
enum actionDone __getActionType(pAction self) {
    return self->action->actionDone;
}
// Method to delete (free) an action from the memory
void __delAction(pAction self) {
    DEBUG_INFO("Entered __delAction\n");
    free((void *) self->action->data);
    free((void *) self->action);
    free((void *) self);
    DEBUG_INFO("Exiting __delAction...\n");
}
// Method to print the actionStack
void __printActionStack(pActionStack self){
    pAction cur = self->__actionStack->__top;
    if(cur == NULL) {
        printf("Stack is empty\n\t\\___ Exiting...\n");
        return;
    }
    while(cur->action->__next != NULL){
        printf("Location: 0x%p\n\t\\___ Index: %d\n\t\\___ Data: %s\n",
               cur, cur->getIndex(cur), cur->getData(cur));
        cur = cur->action->__next;
    }
}
// Method to "push" an action onto the ActionStack
void __push(pActionStack self, pAction doneAction) {
    DEBUG_INFO("Entered __push function\n");
    if(self->__actionStack->__count >= self->__actionStack->__capacity){
        DEBUG_INFO("Action Stack FULL...\n\t\\__ Printing it's contents...\n");
#ifdef DEBUG
        __printActionStack(self);
#endif
        DEBUG_INFO("Freeing the actionStack...\n");
        for(pAction cur = self->pop(self); cur != NULL;cur=self->pop(self)){
            pAction temp = cur;
            free((void*)temp);
        }
        DEBUG_INFO("Printing the stack after freeing it...\n");
#ifdef DEBUG
        __printActionStack(self);
#endif
    }

    if(self->__actionStack->__top == NULL){
        DEBUG_INFO("Action stack is empty\n\t\\___ Assigning the new action to the top...\n");
        self->__actionStack->__top = doneAction;
        self->__actionStack->__count++;
        DEBUG_INFO("Exiting __push function...\n");
        return;
    }
    DEBUG_INFO("Linking the action to the previous actions...\n");
    doneAction->action->__next = self->__actionStack->__top;
    self->__actionStack->__top = doneAction;
    self->__actionStack->__count++;
    DEBUG_INFO("Exiting __push function...\n");
}
// Method to "pop" an action from the ActionStack
pAction __pop(pActionStack self) {
    DEBUG_INFO("Entered __pop function\n");
    if (self->__actionStack->__top == NULL) return NULL;
    pAction poppedAction = self->__actionStack->__top;
    self->__actionStack->__top = self->__actionStack->__top->action->__next;
    poppedAction->action->__next = NULL;
    self->__actionStack->__count--;
    DEBUG_INFO("Exiting __pop  function...\n");
    return poppedAction;
}
// Method to delete (free) stack from the memory
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
// Method to get the index of the top action on the stack
int __getTopIndex(pActionStack self) {
    return self->__actionStack->__top->getIndex(self->__actionStack->__top);
}
// Method to get the data of the top action on the stack
char *__getTopData(pActionStack self, char *def) {
    if (self->__actionStack->__top->getData(self->__actionStack->__top) == NULL) return def;
    return self->__actionStack->__top->getData(self->__actionStack->__top);
}
// Method to get the actionType of the top action on the stack
enum actionDone __getTopAction(pActionStack self) {
    return self->__actionStack->__top->getAction(self->__actionStack->__top);
}
// Constructor for the encapsulated Action Class
pAction newAction(enum actionDone actionType, int index, char *data) {
    DEBUG_INFO("Entered newAction function\n");
    pAction newAct = (pAction) malloc(sizeof(*newAct));
    if (!newAct) return NULL;
    DEBUG_OKAY("Allocated %lld bytes at 0x%p\n", sizeof(*newAct), newAct);
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
    DEBUG_INFO("Copied the action data to the Action\n");
    newAct->getAction = &__getActionType;
    newAct->getData = &__getActionData;
    newAct->getIndex = &__getActionIndex;
    newAct->delAction = &__delAction;
    DEBUG_INFO("Linked the action methods to the object\n");
    DEBUG_INFO("Exiting newAction function...\n");
    return newAct;
}
// Constructor for the encapsulated ActionStack class
pActionStack newActionStack() {
    DEBUG_INFO("Entered newActionStack function\n");
    pActionStack newStack = (pActionStack) malloc(sizeof(*newStack));
    if(newStack == NULL) return newStack;
    DEBUG_OKAY("Allocated %lld bytes at 0x%p\n", sizeof(*newStack), newStack);
    newStack->__actionStack = (pActionStackOG) malloc(sizeof(*(newStack->__actionStack)));
    if(newStack->__actionStack == NULL){
        free((void*)newStack);
        return newStack;
    }
    DEBUG_OKAY("Allocated %lld bytes at 0x%p for the action stack\n", sizeof(*(newStack->__actionStack)), newStack->__actionStack);
    newStack->__actionStack->__top = NULL;
    newStack->__actionStack->__capacity = 10;
    newStack->__actionStack->__count = 0;
    DEBUG_OKAY("Initialized the default values\n");
    newStack->getTopAction = &__getTopAction;
    newStack->getTopIndex = &__getTopIndex;
    newStack->getTopData = &__getTopData;
    newStack->push = &__push;
    newStack->pop = &__pop;
    newStack->delStack = &__delStack;
    DEBUG_OKAY("Linked the methods for the action stack\n");
    DEBUG_INFO("Exiting the newActionStack function...\n");
    return newStack;
}
