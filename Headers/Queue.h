//
// Created by sdds on 12/20/2023.
//

#ifndef DATASTRUCTURESPROJECT_QUEUE_H
#define DATASTRUCTURESPROJECT_QUEUE_H
#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-reserved-identifier"
#include <stdbool.h>




typedef struct Queue *pOGQueue;
typedef struct QueueEntry QueueEntry, *pQueueEntry;
// Public Class to Encapsulate the original one - Interface
typedef struct QueueImplementation{
    pOGQueue Queue;

    // Dump the queue for debugging purposes
    void (*dumpQueue)(struct QueueImplementation* self);
    // Free the queue from the memory
    void (*delQueue)(struct QueueImplementation* self);
    // Iterate the queue printing out all the elements in FIFO order
    void (*iterQueue)(struct QueueImplementation* self);
    // Update data at the given index in the queue
    int (*updateData)(struct QueueImplementation* self, int index, char* data, bool inc);
    // Get the data at the given index
    char* (*getQueue)(struct QueueImplementation* self, int index, char* def);
    // Get the data at the front of the queue
    char* (*getHead)(struct QueueImplementation* self, char* def);
    // Get the Head's index
    int (*getHeadIndex)(struct QueueImplementation* self);
    // Insert data at the beginning of the queue
    int (*putHead)(struct QueueImplementation* self, char* data, bool inc);
    // Get the data at the end of the queue
    char* (*getTail)(struct QueueImplementation* self, char* def);
    // Add data to the queue
    int (*enqueueData)(struct QueueImplementation* self, char* data);
    // Get the data at the end of the queue then free it (Don't forget to free the dequeued data)
    char* (*dequeueData)(struct QueueImplementation* self, char* def);
    // Delete data at a given index
    void (*deleteData)(struct QueueImplementation* self, int index);
    // Get the size of the queue aka the number of nodes
    int (*sizeQueue)(struct QueueImplementation* self);
}*pQueue, *pQueueImpl;

// Initialize the queue
pQueue newQueue();

#pragma clang diagnostic pop
#endif //DATASTRUCTURESPROJECT_QUEUE_H
