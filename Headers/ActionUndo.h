//
// Created by sdds on 12/25/2023.
//

#ifndef DATASTRUCTURESPROJECT_ACTIONUNDO_H
#define DATASTRUCTURESPROJECT_ACTIONUNDO_H

// A holder for the values of ops
typedef enum actionDone {INSERT, MODIFY, DELETE} ActionDone, *pActionDone;
// Defining a struct from our original Action Class to encapsulate
typedef struct ActionOG *pActionOG;
// Defining the encapsulated Action Class
typedef struct Action{
    pActionOG action;

    char* (*getData)(struct Action* self);
    int (*getIndex)(struct Action* self);
    enum actionDone (*getAction)(struct Action* self);
    void (*delAction)(struct Action* self);
}*pAction;
// Defining the original ActionStack class to encapsulate
typedef struct ActionStack *pActionStackOG;
// Defining the encapsulated ActionStack
typedef struct ActionStackImpl{
    pActionStackOG __actionStack;
    int (*getTopIndex)(struct ActionStackImpl* self);
    char* (*getTopData)(struct ActionStackImpl* self, char* def);
    enum actionDone (*getTopAction)(struct ActionStackImpl* self);
    void (*push)(struct ActionStackImpl* self, pAction action);
    pAction (*pop)(struct ActionStackImpl* self);
    void (*delStack)(struct ActionStackImpl* self);
} ActionStack, *pActionStack;
// Constructor for the encapsulated ActionStack
pActionStack newActionStack();
// Constructor for the encapsulated Action
pAction newAction(enum actionDone action, int index, char* data);

#endif //DATASTRUCTURESPROJECT_ACTIONUNDO_H
