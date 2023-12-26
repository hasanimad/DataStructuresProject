//
// Created by sdds on 12/25/2023.
//

#ifndef DATASTRUCTURESPROJECT_ACTIONUNDO_H
#define DATASTRUCTURESPROJECT_ACTIONUNDO_H

#define DEBUG
enum actionDone {INSERT, DELETE};
typedef struct ActionOG *pActionOG;

typedef struct Action{
    pActionOG action;

    char* (*getData)(struct Action* self);
    int (*getIndex)(struct Action* self);
    enum actionDone (*getAction)(struct Action* self);
    void (*delAction)(struct Action* self);
}*pAction;

typedef struct ActionStack *pActionStackOG;

typedef struct ActionStackImpl{
    pActionStackOG __actionStack;
    void (*push)(struct ActionStackImpl* self, pAction action);
    pAction (*pop)(struct ActionStackImpl* self);
    void (*delStack)(struct ActionStackImpl* self);
} ActionStack, *pActionStack;

pActionStack newActionStack();
pAction newAction(enum actionDone action, int index, char* data);

#endif //DATASTRUCTURESPROJECT_ACTIONUNDO_H
