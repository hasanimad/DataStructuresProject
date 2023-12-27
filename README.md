# Data Structures and Algorithms Project

This project is a comprehensive implementation of various data structures, including queues, stacks, and more. It provides a set of functionalities to manipulate data in a queue, maintain an action stack for undo operations, and execute various commands related to data management.

## Features

- **Queue Management:** Enqueue, dequeue, and manipulate data in a queue structure.
- **Undo Functionality:** Implement an action stack to undo recent changes in the queue.
- **Command Execution:** Perform various operations based on text commands.

## Getting Started

### Prerequisites

- C compiler (e.g., GCC, Clang)
- CMake (optional for build automation)

### Installation

1. Clone the repository:
```bash
git clone https://github.com/hasanimad/DataStructuresProject.git
```
2. Navigate to the project directory:
```sh
cd DataStructuresProject/
```
3. Compile the project:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
## Usage

Run the compiled program:
```bash
./DataStructuresProject
```
The program accepts various commands for queue manipulation and other operations. Here is a list of available commands:

    insert: Enqueue data into the queue.
    remove: Dequeue data from the queue.
    modify: Modify existing data in the queue.
    fetch: Retrieve specific data from the queue.
    PrintData: Print all data in the queue.
    PeekEnd: Peek at the tail of the queue.
    PeekHead: Peek at the head of the queue.
    InsertFront: Insert data at the front of the queue.
    Undo: Undo the last action.
    CountElements: Count the number of elements in the queue.
    PrintMenu: Display available commands.
    PrintDetails: Print diagnostic data of the queue.
    RemoveAll: Remove all elements from the queue.


