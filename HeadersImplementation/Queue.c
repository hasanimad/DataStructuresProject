#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-reserved-identifier"
//
// Created by sdds on 12/20/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/Queue.h"
#include "../Headers/strux.h"



/*---{ Defining Private Structs }---*/

// Queue entries struct (Class)
struct QueueEntry{
    // The index is redundant, just there for the needed functionality
    int index;
    // A pointer to the stored data
    char* data;
    // A pointer to the next entry
    struct QueueEntry* __next;
    // A pointer to the previous entry
    struct QueueEntry* __prev;
};

// Main queue struct (Class)
struct Queue{
    // Number of entries in the queue
    int __count;
    // A pointer to the head of the queue
    pQueueEntry __head;
    // A pointer to the tail of the queue
    pQueueEntry __tail;
};

// Queue iterator (For modularity)
typedef struct QueueIter {
    // A pointer to the current entry in the queue
    pQueueEntry __current;

    // A function to go to the next entry
    pQueueEntry (*next) (struct QueueIter* self);
    // A function to delete the iterator, freeing it from the memory
    void (*del) (struct QueueIter* self);
}QueueIter, *pQueueIter;

/*---{ Private Functions }---*/
pQueueEntry __newQueueEntry(char* data){
    DEBUG_INFO("Entering __newQueueEntry function...\n");
    pQueueEntry newEntry = (pQueueEntry)malloc(sizeof(*newEntry));
    if(newEntry == NULL){
        DEBUG_ERROR("Failed to allocate %lld for the new node\n", sizeof(*newEntry));
        DEBUG_INFO("Exiting __newQueueEntry function...");
        return newEntry;
    }
    DEBUG_OKAY("Allocated %lld bytes for the new node at 0x%p\n", sizeof(*newEntry), newEntry);
    newEntry->data = (char*)malloc(strlen(data)+1);
    if(newEntry->data == NULL){
        DEBUG_ERROR("Failed to allocate %lld bytes for the data in the new node\n", strlen(data)+1);
        free((void*)newEntry);
        DEBUG_INFO("Exiting __newQueueEntry function...");
        return newEntry;
    }

    strcpy(newEntry->data, data);
    newEntry->index = 0;
    newEntry->__next = NULL;
    newEntry->__prev = NULL;
    DEBUG_OKAY("Set up the new node at 0x%p\n", newEntry);
    DEBUG_INFO("Exiting __newQueueEntry function...");
    return newEntry;
}
// Go to the next queue entry - For the iterator
pQueueEntry __QueueIterNext(pQueueIter self) {
    DEBUG_INFO("Entering __QueueIterNext Function...\n");

    // Making a pointer to the queue head which is pointed to in the iterator
    pQueueEntry retVal = self->__current;

    DEBUG_INFO("Retrieved the queue head for the iterator at 0x%p\n", retVal);

    // Checking if the current is NULL to avoid a NULL pointer
    if (retVal != NULL) {
        // Point to the next entry in the queue (Following it down from the head)
        retVal = retVal->__next;
    }

    DEBUG_INFO("Exiting __QueueIterNext Function...\n");
    // Return the new pointer
    return retVal;
}
// Delete the iterator aka free it from the memory
void __QueueIterDel(pQueueIter self) {
    DEBUG_INFO("Entring __QueueIterDel Function...\n");

    // Free the iterator from the memory
    DEBUG_INFO("Freeing the Iterator at 0x%p\n\t\\___ Size: %lld\n", self, sizeof(*self));
    free((void*)self);
    DEBUG_OKAY("Freed %lld bytes from the Iterator at 0x%p\n", sizeof(*self), self);

    DEBUG_INFO("Exiting __QueueIterDel function...\n");
}
// Constructor for the queue iterator
pQueueIter __newQueueIter(pQueue Queue) {
    DEBUG_INFO("Entered __newQueueIter Function...\n");
    // Allocate enough memory for the iterator - 24 bytes
    pQueueIter newIter = (pQueueIter)malloc(sizeof(*newIter));

    // Check if the allocation failed
    if (newIter == NULL) {
        DEBUG_ERROR("Allocating %lld bytes for the iterator failed at 0x%p\n", sizeof(*newIter), newIter);
        return NULL;
    }
    DEBUG_OKAY("New Iterator at 0x%p\n\t\\___ Size: %lld\n", newIter, sizeof(*newIter));
    // Link the iterator to the passed queue head
    newIter->__current = Queue->Queue->__head;
    DEBUG_OKAY("Linked the Iterator to the queue head\n\t\\___ Iterator: 0x%p\n\t\\___ Queue: Head 0x%p\n", newIter, Queue->Queue->__head);
    // Link the next function to the function pointer in the iterator
    newIter->next = &__QueueIterNext;
    DEBUG_OKAY("Linked the next function to the iterator\n\t\\___ Iterator: 0x%p\n\t\\___ Function: 0x%p\n", newIter, &__QueueIterNext);
    // Link the deletion function to the function pointer in the iterator
    newIter->del = &__QueueIterDel;
    DEBUG_OKAY("Linked the delete function to the iterator\n\t\\___ Iterator: 0x%p\n\t\\___ Function: 0x%p\n", newIter, &__QueueIterDel);
    // Return the new object
    DEBUG_INFO("Exiting __newQueueIter Function...\n");
    return newIter;
}

// A helper function to find an entry with a given index - Time Complexity: O(n)
pQueueEntry __QueueFind(pQueue self, int index){
    DEBUG_INFO("Entered __QueueFind Function...\n");

    // Start searching from the queue head
    pQueueEntry current = self->Queue->__head;
    DEBUG_OKAY("Got a link to the Queue Head at 0x%p\n", current);
    while(current != NULL){
        // Traverse through the queue, if found return a pointer to the entry
        if(current->index == index) return current;
        current = current->__next;
    }

    DEBUG_INFO("Exiting __QueueFind Function...\n");
    // If not found, return NULL
    return NULL;
}

// Diagnostic dump of the queue
void __QueueDump(pQueue self){
    DEBUG_INFO("Entered __QueueDump Function...\n");
    if(self == NULL){
        DEBUG_ERROR("Passed in a NULL pointer\n");
        DEBUG_INFO("Exiting __QueueDump...\n");
        return;
    }

    printf("Nodes count: %d\n", self->Queue->__count);
    for(pQueueEntry cur = self->Queue->__head; cur != NULL; cur = cur->__next){
        printf("Node 0x%p\n\t\\___ Index: %d\n\t\\___ Data: %s\n", cur, cur->index, cur->data);
    }
    DEBUG_INFO("Exiting __QueueDump...\n");
}
void __putHead(pQueue self, char* data){
    DEBUG_INFO("Entered __putHead function...\n");
    // Allocate enough memory for the inserted data
    pQueueEntry newEntry = (pQueueEntry)malloc(sizeof(*newEntry));
    if(newEntry == NULL){
        DEBUG_ERROR("Failed to allocate %lld for the new node\n", sizeof(*newEntry));
        DEBUG_INFO("Exiting __putHead function...");
        return;
    }
    DEBUG_OKAY("Allocated %lld bytes for the new node at 0x%p\n", sizeof(*newEntry), newEntry);
    newEntry->data = (char*)malloc(strlen(data)+1);
    if(newEntry->data == NULL){
        DEBUG_ERROR("Failed to allocate %lld bytes for the data in the new node\n", strlen(data)+1);
        free((void*)newEntry);
        DEBUG_INFO("Exiting __putHead function...");
        return;
    }

    strcpy(newEntry->data, data);
    newEntry->index = 0;
    newEntry->__next = NULL;
    newEntry->__prev = NULL;
    DEBUG_OKAY("Set up the new node at 0x%p\n", newEntry);
    self->Queue->__count++;
    // If the Queue is empty, link it directly
    if(self->Queue->__head == NULL){
        self->Queue->__head = newEntry;
        self->Queue->__tail = newEntry;
        return;
    }

    // Increment the index of the OG head and the following nodes
    for(pQueueEntry cur = self->Queue->__head; cur != NULL; cur = cur->__next){
        cur->index++;
    }
    // Link the new Node to the head
    self->Queue->__head->__prev = newEntry;
    newEntry->__next = self->Queue->__head;
    self->Queue->__head = newEntry;
    DEBUG_OKAY("Linked the new Node\n");
    DEBUG_INFO("Exiting __putHead function...\n");
}
void __putTail(pQueue self, char* data){
    DEBUG_INFO("Entered __putTail function...\n");
    pQueueEntry newEntry = (pQueueEntry)malloc(sizeof(*newEntry));
    if(newEntry == NULL){
        DEBUG_ERROR("Failed to allocate %lld for the new node\n", sizeof(*newEntry));
        DEBUG_INFO("Exiting __putHead function...");
        return;
    }
    newEntry->data = (char*)malloc(strlen(data)+1);
    strcpy(newEntry->data, data);
    newEntry->__next = NULL;
    newEntry->__prev = NULL;

    self->Queue->__count++;
    if(self->Queue->__head == NULL){
        newEntry->index = 0;
        self->Queue->__head = newEntry;
        self->Queue->__tail = newEntry;
        return;
    }
    newEntry->index = self->Queue->__tail->index + 1;

    self->Queue->__tail->__next = newEntry;
    newEntry->__prev = self->Queue->__tail;
    self->Queue->__tail = newEntry;
}
void __updateDataOrdered(pQueue self, int index, char* data) {
    // Check if anything needed is null
    if (!self || !data) return;
    // Initialize the needed vars
    pQueueEntry newNode;
    size_t newDataSize = strlen(data);
    // Make the new Node
    newNode = (pQueueEntry) malloc(sizeof(*newNode));
    if(newNode==NULL)return;
    newNode->data = (char*)malloc(newDataSize+1);
    if(newNode->data == NULL){
        free((void*)newNode);
        return;
    }
    // Copy the new data to the node
    newNode->index = index;
    strcpy_s(newNode->data, newDataSize + 1, data);
    // Set the default next and previous values
    newNode->__next = NULL;
    newNode->__prev = NULL;
    self->Queue->__count++;
    // Check if the index is the head
    if(index == 0){
        newNode->__next = self->Queue->__head;
        self->Queue->__head->__prev = newNode;
        self->Queue->__head = newNode;
        return;
    }
    // Search for the right place to insert the new Node
    pQueueEntry cur = self->Queue->__head;
    while (cur->__next != NULL && cur->__next->index < index) {
        cur = cur->__next;
    }
    // Once the right node is found, chain them together
    // Linking the newNode next pointer to the node after the current one
    newNode->__next = cur->__next;
    // check if it is the last node (Avoiding a NULL pointer)
    if (cur->__next != NULL) {
        // link the previous pointer of the node after the current one to our new node (place it in the middle)
        cur->__next->__prev = newNode;
    } else {
        // If it is the last node, point the tail to it
        self->Queue->__tail = newNode;
    }
    // Chain our new Node previous pointer to the current one (Placing it in front of the current one)
    newNode->__prev = cur;
    // Finally, link the current node to the new node (placing it in  front of it)
    cur->__next = newNode;
    // Increment the number of nodes in the queue
    self->Queue->__count++;
}
void __updateData(pQueue self, int index, char* data) {
    if (!self || !data) return;
    pQueueEntry newNode = __QueueFind(self, index);
    size_t newDataSize = strlen(data);
    if (newNode != NULL) {
        free((void *) newNode->data);
        newNode->data = (char *) malloc(newDataSize + 1);
        strcpy_s(newNode->data, newDataSize + 1, data);
        return;
    }
    __updateDataOrdered(self, index, data);
}
char* __getQueue(pQueue self, int index, char* def){
    pQueueEntry toFind = __QueueFind(self, index);
    if(toFind == NULL) return def;
    return toFind->data;
}
char* __getHead(pQueue self, char* def){
    if(self->Queue->__head == NULL) return def;
    return self->Queue->__head->data;
}
char* __getTail(pQueue self, char* def){
    if(self->Queue->__tail == NULL) return def;
    return self->Queue->__tail->data;
}
char* __delHead(pQueue self, char* def){
    // Get the data inside the head
    char* retData = __getHead(self, def);
    // Check if it is the default value
    if(strcmp(retData, def) == 0) return def;
    self->Queue->__count--;
    // if not retrieve the OG data
    pQueueEntry toBeDeleted = self->Queue->__head;
    self->Queue->__head->__next->__prev = NULL;
    self->Queue->__head = self->Queue->__head->__next;
    toBeDeleted->__next = NULL;
    // Free the deleted Node from the memory
    free((void*)toBeDeleted);

    return retData;
}
int __sizeQueue(pQueue self){
    return self->Queue->__count;
}
void __delQueue(pQueue self){
    pQueueEntry current, next;
    current = self->Queue->__head;
    while(current != NULL){
        next = current->__next;
        free((void*) current->data);
        free((void*)current);
        current = next;
    }
    free((void*)self);
}
void __QueueIterate(pQueue Queue) {
    pQueueIter iter = __newQueueIter(Queue);
    pQueueEntry entry;
    while ((entry = iter->next(iter)) != NULL) {
        printf("Index: %d, Data: %s\n", entry->index, entry->data);
    }

    iter->del(iter);
}
pQueue newQueue(){
    pQueue newQueue = (pQueue)malloc(sizeof(*newQueue));
    if(newQueue == NULL) return NULL;
    newQueue->Queue = (struct Queue*)malloc(sizeof((*newQueue->Queue)));
    if(newQueue->Queue == NULL){
        free((void*)(newQueue));
        return NULL;
    }

    newQueue->Queue->__count = 0;
    newQueue->Queue->__head = NULL;
    newQueue->Queue->__tail = NULL;


    newQueue->updateData = &__updateData;
    newQueue->sizeQueue = &__sizeQueue;
    newQueue->getQueue = &__getQueue;

    newQueue->getHead = &__getHead;
    newQueue->putHead = &__putHead;
    newQueue->getTail = &__getTail;

    newQueue->dequeueData = &__delHead;
    newQueue->enqueueData = &__putTail;

    newQueue->iterQueue = &__QueueIterate;
    newQueue->dumpQueue = &__QueueDump;
    newQueue->delQueue = &__delQueue;

    return newQueue;
}
#pragma clang diagnostic pop