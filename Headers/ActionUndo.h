//
// Created by sdds on 12/25/2023.
//

#ifndef DATASTRUCTURESPROJECT_ACTIONUNDO_H
#define DATASTRUCTURESPROJECT_ACTIONUNDO_H

typedef enum actionDone {INSERT, MODIFY, DELETE} ActionDone, *pActionDone;
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
    int (*getTopIndex)(struct ActionStackImpl* self);
    char* (*getTopData)(struct ActionStackImpl* self, char* def);
    enum actionDone (*getTopAction)(struct ActionStackImpl* self);
    void (*push)(struct ActionStackImpl* self, pAction action);
    pAction (*pop)(struct ActionStackImpl* self);
    void (*delStack)(struct ActionStackImpl* self);
} ActionStack, *pActionStack;

pActionStack newActionStack();
pAction newAction(enum actionDone action, int index, char* data);

#endif //DATASTRUCTURESPROJECT_ACTIONUNDO_H
