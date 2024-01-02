#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-reserved-identifier"
//
// Created by sdds on 12/20/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

// New Nodes constructor
pQueueEntry __newQueueEntry(char* data){
    DEBUG_INFO("Entering __newQueueEntry function\n");
    pQueueEntry newEntry = (pQueueEntry)malloc(sizeof(*newEntry));
    if(newEntry == NULL){
        DEBUG_ERROR("Failed to allocate %lld for the new node\n", sizeof(*newEntry));
        DEBUG_INFO("Exiting __newQueueEntry function...\n");
        return newEntry;
    }
    DEBUG_OKAY("Allocated %lld bytes for the new node at 0x%p\n", sizeof(*newEntry), newEntry);
    newEntry->data = (char*)malloc(strlen(data)+1);
    if(newEntry->data == NULL){
        DEBUG_ERROR("Failed to allocate %lld bytes for the data in the new node\n", strlen(data)+1);
        free((void*)newEntry);
        DEBUG_INFO("Exiting __newQueueEntry function...\n");
        return newEntry;
    }
    DEBUG_OKAY("Allocated %lld bytes for the new node data at 0x%p\n", strlen(data)+1, newEntry->data);

    strcpy(newEntry->data, data);
    newEntry->index = 0;
    newEntry->__next = NULL;
    newEntry->__prev = NULL;
    DEBUG_OKAY("Set up the new node at 0x%p\n", newEntry);
    DEBUG_INFO("Exiting __newQueueEntry function...\n");
    return newEntry;
}

// Go to the next queue entry - For the iterator
pQueueEntry __QueueIterNext(pQueueIter self) {
    DEBUG_INFO("Entering __QueueIterNext Function\n");

    // Making a pointer to the queue head which is pointed to in the iterator
    pQueueEntry retVal = self->__current;

    DEBUG_INFO("Retrieved the queue head for the iterator at 0x%p\n", retVal);

    // Checking if the current is NULL to avoid a NULL pointer
    if (self->__current != NULL) {
        // Point to the next entry in the queue (Following it down from the head)
        self->__current = self->__current->__next;
    }

    DEBUG_INFO("Exiting __QueueIterNext Function...\n");
    // Return the new pointer
    return retVal;
}

// Delete the iterator aka free it from the memory
void __QueueIterDel(pQueueIter self) {
    DEBUG_INFO("Entring __QueueIterDel Function\n");

    // Free the iterator from the memory
    DEBUG_INFO("Freeing the Iterator at 0x%p\n\t\\___ Size: %lld\n", self, sizeof(*self));
    free((void*)self);
    DEBUG_OKAY("Freed %lld bytes from the Iterator at 0x%p\n", sizeof(*self), self);

    DEBUG_INFO("Exiting __QueueIterDel function...\n");
}

// Constructor for the queue iterator
pQueueIter __newQueueIter(pQueue Queue) {
    DEBUG_INFO("Entered __newQueueIter Function\n");
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
    DEBUG_INFO("Entered __QueueFind Function\n");

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
    DEBUG_INFO("Entered __QueueDump Function\n");
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

// Add a node to the head of the queue - Time Complexity O(n)
int __putHead(pQueue self, char* data, bool inc){
    DEBUG_INFO("Entered __putHead function\n");
    // Initialize the new node
    pQueueEntry newEntry = __newQueueEntry(data);
    self->Queue->__count++;
    // If the Queue is empty, link it directly
    if(self->Queue->__head == NULL){
        self->Queue->__head = newEntry;
        self->Queue->__tail = newEntry;
        return self->Queue->__head->index;
    }

    // Increment the index of the OG head and the following nodes
    for(pQueueEntry cur = self->Queue->__head; cur != NULL && inc; cur = cur->__next){
        cur->index++;
    }
    // Link the new Node to the head
    self->Queue->__head->__prev = newEntry;
    newEntry->__next = self->Queue->__head;
    self->Queue->__head = newEntry;
    DEBUG_OKAY("Linked the new Node\n");
    DEBUG_INFO("Exiting __putHead function...\n");
    return self->Queue->__head->index;
}

// Add a node to the end of the queue - Time complexity O(1)
int __putTail(pQueue self, char* data){
    DEBUG_INFO("Entered __putTail function\n");
    // Initialize the new node
    pQueueEntry newEntry = __newQueueEntry(data);

    self->Queue->__count++;
    // If the queue is empty
    if(self->Queue->__head == NULL){
        newEntry->index = 0;
        self->Queue->__head = newEntry;
        self->Queue->__tail = newEntry;
        DEBUG_OKAY("The Queue is empty\n\t\\___ Linked the head and tail to the new node\n");
        return newEntry->index;
    }
    // If the queue is not empty
    newEntry->index = self->Queue->__tail->index + 1;

    self->Queue->__tail->__next = newEntry;
    newEntry->__prev = self->Queue->__tail;
    self->Queue->__tail = newEntry;
    DEBUG_OKAY("Linked the new node to the tail\n");
    DEBUG_INFO("Exiting the __putTail function...\n");
    return self->Queue->__tail->index;
}

// helper function for __updateData
int __updateDataOrdered(pQueue self, int index, char* data, bool inc) {
    DEBUG_INFO("Entered __updateDataOrdered function\n");
    DEBUG_INFO("Queue: 0x%p\n\t\\___ Index: %d\n\t\\___ Data: %s\n",self ,index, data);
    // Check if anything needed is null
    if (!self || !data) return -1;
    // Initialize the new Node
    pQueueEntry newNode = __newQueueEntry(data);
    self->Queue->__count++;
    DEBUG_INFO("Incremented the queue count\n");
    // Check if the index is the head
    if(index == 0){
        __putHead(self, data, inc);
        DEBUG_OKAY("Added the new Node to the head\n");
        return index;
    }
    // Set the index for the new node
    newNode->index = index;
    // Handle if the queue is empty3
    if(self->Queue->__head == NULL){
        self->Queue->__head = newNode;
        self->Queue->__tail = newNode;
        return index;
    }
    // Search for the right place to insert the new Node
    DEBUG_INFO("Finding the right place it insert the node...\n");
    pQueueEntry cur = self->Queue->__head;
    while (cur->__next != NULL && cur->__next->index < index) {
        cur = cur->__next;
    }
    DEBUG_INFO("Found the right place!!\n\t\\___ cur: 0x%p\n\t\\___ cur->next: 0x%p\n\t\\___ newNode: 0x%p\n\t\\___ head: 0x%p\n\t\\___ Tail: 0x%p\n",
               cur, cur->__next, newNode, self->Queue->__head, self->Queue->__tail
               );
    // Once the right node is found, chain them together
    // Linking the newNode next pointer to the node after the current one
    DEBUG_INFO("Setting the newNode->next to  the current->next...\n");
    newNode->__next = cur->__next;
    DEBUG_INFO("Values of\n\t\\___ newNode->next: 0x%p\n\t\\___ current->next: 0x%p\n", newNode->__next, cur->__next);
    // check if it is the last node (Avoiding a NULL pointer)
    if (cur->__next != NULL) {
        DEBUG_INFO("Linking the node in front of the current one..\n");
        // link the previous pointer of the node after the current one to our new node (place it in the middle)
        cur->__next->__prev = newNode;

    } else {
        DEBUG_INFO("No node in front of the current one\n\t\\___ Linking the tail\n");
        // If it is the last node, point the tail to it
        self->Queue->__tail = newNode;
        DEBUG_INFO("Linked the tail\n");
    }
    // Chain our new Node previous pointer to the current one (Placing it in front of the current one)
    newNode->__prev = cur;
    DEBUG_INFO("Linked the newNode previous pointer to the current node\n");
    // Finally, link the current node to the new node (placing it in  front of it)
    cur->__next = newNode;
    DEBUG_INFO("Linked the current node next pointer to the newNode\n");
    DEBUG_OKAY("Linked the new node in it's right place\n\t\\___ Current Node: 0x%p\n\t\\___ Next Node: 0x%p\n\t\\___ Previous Node: 0x%p\n",
               newNode, newNode->__next, newNode->__prev);
    DEBUG_INFO("Exiting __updateDataOrdered function...\n");
    return newNode->index;
}
// Update the node at the given index, if not found create one and insert at the right index - Time Complexity O(n)
int __updateData(pQueue self, int index, char* data, bool inc) {
    DEBUG_INFO("Entered __updateData function\n");
    DEBUG_INFO("Queue: 0x%p\n\t\\___ Index: %d\n\t\\___ Data: %s\n",self ,index, data);
    if (!self || !data) return -1;
    pQueueEntry newNode = __QueueFind(self, index);
    size_t newDataSize = strlen(data);
    if (newNode != NULL) {
        DEBUG_INFO("Found the needed node\n");
        free((void *) newNode->data);
        newNode->data = (char *) malloc(newDataSize + 1);
        strcpy(newNode->data, data);
        DEBUG_INFO("Exiting __updateData function...\n");
        return newNode->index;
    }
    DEBUG_INFO("Calling __updateDataOrdered...\n");
    int newIndex = __updateDataOrdered(self, index, data, inc);
    DEBUG_INFO("Exiting __updateData function...\n");
    return newIndex;
}
// Get the data at the given index - TC O(n)
char* __getQueue(pQueue self, int index, char* def){
    DEBUG_INFO("Entered __getQueue function\n");
    pQueueEntry toFind = __QueueFind(self, index);
    if(toFind == NULL) {
        DEBUG_ERROR("Didn't find the required node\n");
        DEBUG_INFO("Exiting __getQueue function...\n");
        return def;
    }
    DEBUG_OKAY("Found the required node\n");
    DEBUG_INFO("Exiting __getQueue function...\n");
    return toFind->data;
}
// Get the data in the head of the queue - TC O(1)
char* __getHead(pQueue self, char* def){
    DEBUG_INFO("Entered __getHead function\n");
    if(self->Queue->__head == NULL) {
        DEBUG_ERROR("Queue head is NULL\n");
        DEBUG_INFO("Exiting __getHead function...\n");
        return def;
    }
    DEBUG_OKAY("Retrieving data inside the head...\n");
    DEBUG_INFO("Exiting __getHead function...\n");
    return self->Queue->__head->data;
}
// Get the Head's index
int __getHeadIndex(pQueue self){
    if(self->Queue->__head == NULL) return -1;
    return self->Queue->__head->index;
}
// Get the data in the tail of the queue - TC O(1)
char* __getTail(pQueue self, char* def){
    DEBUG_INFO("Entered __getTail function\n");
    if(self->Queue->__tail == NULL) {
        DEBUG_ERROR("Queue tail is NULL\n");
        DEBUG_INFO("Exiting __getTail function...");
        return def;
    }
    DEBUG_OKAY("Retrieving data inside the tail...\n");
    DEBUG_INFO("Exiting __getTail function...\n");
    return self->Queue->__tail->data;
}
// Dequeue the head of the queue - TC O(1)
char* __delHead(pQueue self, char* def){
    DEBUG_INFO("Entered __delHead function (dequeue)\n");

    if(self->Queue->__head == NULL) {
        DEBUG_ERROR("Queue is empty\n");
        return def;
    }
    // Get the data inside the head
    char* retData = __getHead(self, def);

    // Check if it is the default value
    if(strcmp(retData, def) == 0) return def;
    self->Queue->__count--;
    // if not retrieve the OG data
    pQueueEntry toBeDeleted = self->Queue->__head;
    if(self->Queue->__head->__next != NULL) {
        self->Queue->__head->__next->__prev = NULL;
        self->Queue->__head = self->Queue->__head->__next;
    } else {
        // If there's only one node, set both head and tail to NULL
        self->Queue->__head = NULL;
        self->Queue->__tail = NULL;
    }
    // Free the deleted Node from the memory
    free((void*)toBeDeleted);
    DEBUG_OKAY("retrieving the data inside the head...\n");
    DEBUG_INFO("Exiting __delHead (dequeue) function...\n");
    return retData;
}
void __delData(pQueue self, int index){
    DEBUG_INFO("Entered __delData function\n");
    pQueueEntry toBeDeleted = __QueueFind(self, index);
    if (toBeDeleted == NULL) return;
    DEBUG_OKAY("Found a node\n\t\\___ Deleting...\n");
    // Handle the head of the queue
    if (toBeDeleted == self->Queue->__head) {
        self->Queue->__head = toBeDeleted->__next;
        if (self->Queue->__head != NULL) {
            self->Queue->__head->__prev = NULL;
        }
    }

    // Handle the tail of the queue
    if (toBeDeleted == self->Queue->__tail) {
        self->Queue->__tail = toBeDeleted->__prev;
        if (self->Queue->__tail != NULL) {
            self->Queue->__tail->__next = NULL;
        }
    }

    // Handle a middle node
    if (toBeDeleted->__next != NULL) {
        toBeDeleted->__next->__prev = toBeDeleted->__prev;
    }
    if (toBeDeleted->__prev != NULL) {
        toBeDeleted->__prev->__next = toBeDeleted->__next;
    }

    // Free the node
    free((void*)toBeDeleted->data);
    free((void*)toBeDeleted);
    DEBUG_INFO("Exiting __delData function...\n");
}
// Get the size of the queue
int __sizeQueue(pQueue self){
    return self->Queue->__count;
}
// Free the queue from memory - deleting it - TC O(1)
void __delQueue(pQueue self){
    DEBUG_INFO("Entered __delQueue function\n");
    pQueueEntry current, next;
    current = self->Queue->__head;
    DEBUG_OKAY("Got a pointer to the head\n\t\\___ Deleting nodes...\n");
    while(current != NULL){
        next = current->__next;
        free((void*) current->data);
        free((void*)current);
        current = next;
    }
    DEBUG_OKAY("Deleted the nodes\n\t\\___ Deleting the queue object.\n");
    free((void*)self);
    self= NULL;
    DEBUG_INFO("Exiting __delQueue function...\n");
}
// Function to iterate through the queue - TC O(n)
void __QueueIterate(pQueue Queue) {
    DEBUG_INFO("Entered __QueueIterate function\n");
    pQueueIter iter = __newQueueIter(Queue);
    pQueueEntry entry;
    while ((entry = iter->next(iter)) != NULL) {
        printf("Index: %d, Data: %s\n", entry->index, entry->data);
    }

    iter->del(iter);
    DEBUG_OKAY("Deleted the Iterator\n");
    DEBUG_INFO("Exiting __QueueIterate function...\n");
}
// Queue constructor
pQueue newQueue(){
    DEBUG_INFO("Entered newQueue function\n");
    // Allocating enough memory for the queue skeletal object
    pQueue newQueue = (pQueue)malloc(sizeof(*newQueue));
    if(newQueue == NULL) {
        DEBUG_ERROR("Failed to allocate %lld bytes\n", sizeof(*newQueue));
        DEBUG_INFO("Exiting newQueue function...\n");
        return NULL;
    }
    DEBUG_OKAY("Allocated %lld bytes at 0x%p\n", sizeof(*newQueue), newQueue);
    // Allocating enough memory for the queue itself
    newQueue->Queue = (struct Queue*)malloc(sizeof((*newQueue->Queue)));
    if(newQueue->Queue == NULL){
        DEBUG_ERROR("Failed to allocate %lld bytes\n", sizeof(*(newQueue->Queue)));
        DEBUG_INFO("Freeing %lld bytes from 0x%p\n", sizeof(*newQueue), newQueue);
        free((void*)(newQueue));
        DEBUG_INFO("Exiting newQueue function...\n");
        return NULL;
    }
    // Initializing the default values for the queue
    newQueue->Queue->__count = 0;
    newQueue->Queue->__head = NULL;
    newQueue->Queue->__tail = NULL;
    DEBUG_OKAY("Initialized the queue with default values\n");

    // Linking the functions to the function pointers
    newQueue->updateData = &__updateData;
    newQueue->sizeQueue = &__sizeQueue;
    newQueue->getQueue = &__getQueue;

    newQueue->getHeadIndex = &__getHeadIndex;
    newQueue->getHead = &__getHead;
    newQueue->putHead = &__putHead;
    newQueue->getTail = &__getTail;

    newQueue->dequeueData = &__delHead;
    newQueue->enqueueData = &__putTail;

    newQueue->deleteData = &__delData;

    newQueue->iterQueue = &__QueueIterate;
    newQueue->dumpQueue = &__QueueDump;
    newQueue->delQueue = &__delQueue;
    DEBUG_OKAY("Linked the functions to the Queue\n");

    DEBUG_INFO("Exiting newQueue function...\n");
    return newQueue;
}
#pragma clang diagnostic pop