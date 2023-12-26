//
// Created by sdds on 12/25/2023.
//

#ifndef DATASTRUCTURESPROJECT_ACTIONUNDO_H
#define DATASTRUCTURESPROJECT_ACTIONUNDO_H

typedef struct Action Action, *pAction;
typedef struct ActionStack{
    Action* actions;
    int top;
    int capacity;
    void (*push)(struct ActionStack* stack, Action action);
    pAction (*pop)(struct ActionStack* stack);
} ActionStack, *pActionStack;

pActionStack newActionStack();
#endif //DATASTRUCTURESPROJECT_ACTIONUNDO_H
