//
// Created by sdds on 12/25/2023.
//
#include <stdlib.h>
#include "../Headers/ActionUndo.h"
struct Action{
    enum actionDone { INSERT, DELETE } actionDone;
    int index;
    char* data;
};
void push(pActionStack self, Action doneAction){

}
pAction pop(pActionStack self) {
    return self->actions;
}
pActionStack newActionStack(){
    pActionStack newStack = (pActionStack) malloc(sizeof(*newStack));
    if(newStack == NULL) return newStack;
    newStack->actions = (pAction)malloc(sizeof(*newStack->actions));
    if(newStack->actions == NULL){
        free((void*)newStack);
        return newStack;
    }
    newStack->top = 0;
    newStack->capacity = 10;
    newStack->pop = &pop;
    newStack->push = &push;
    return newStack;
}
