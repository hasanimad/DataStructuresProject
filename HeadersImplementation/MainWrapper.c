//
// Created by sdds on 12/27/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/ActionUndoLink.h"
#include "../Headers/MainWrapper.h"
#include "../Headers/strux.h"

// Function pointer
typedef void (*CommandFunc)(char**, pQueueImpl, pActionStack);
// Map Class {command, functionToCall}
typedef struct {
    char* command;
    CommandFunc func;
} __CommandMap;

// Map itself, containing all the keys
__CommandMap __commandMap[] = {
        {"insert", &enQueueData},
        { "remove", &deQueueData},
        { "modify", &modifyData},
        {"fetch", (CommandFunc) &getQueueData},
        {"printdata", (CommandFunc) &iterateQueue},
        {"peekend", (CommandFunc) &peekEnd},
        {"peekhead", (CommandFunc) &peekHead},
        { "insertfront", &insertFront },
        {"undo", (CommandFunc) &undoAction},
        {"countelements", (CommandFunc) &countElements},
        { "printmenu", &printMenu },
        {"printdetails", (CommandFunc) &printDiagnosticData},
        {"removeall", (CommandFunc) &removeAll},
        {NULL, NULL} // Sentinel to mark end of the array
};

// Print the main menu (Banner included)
void printMenu() {
    unsigned char ascii_art[] =
            ".___________. _______ .______      .___  ___.  __  .__   __.      ___       __         .__   __.   ______   .___________. _______ .______      ___       _______  \n"
            "|           ||   ____||   _  \\     |   \\/   | |  | |  \\ |  |     /   \\     |  |        |  \\ |  |  /  __  \\  |           ||   ____||   _  \\    /   \\     |       \\ \n"
            "`---|  |----`|  |__   |  |_)  |    |  \\  /  | |  | |   \\|  |    /  ^  \\    |  |        |   \\|  | |  |  |  | `---|  |----`|  |__   |  |_)  |  /  ^  \\    |  .--.  |\n"
            "    |  |     |   __|  |      /     |  |\\/|  | |  | |  . `  |   /  /_\\  \\   |  |        |  . `  | |  |  |  |     |  |     |   __|  |   ___/  /  /_\\  \\   |  |  |  |\n"
            "    |  |     |  |____ |  |\\  \\----.|  |  |  | |  | |  |\\   |  /  _____  \\  |  `----.   |  |\\   | |  `--'  |     |  |     |  |____ |  |     /  _____  \\  |  '--'  |\n"
            "    |__|     |_______|| _| `._____||__|  |__| |__| |__| \\__| /__/     \\__\\ |_______|   |__| \\__|  \\______/      |__|     |_______|| _|    /__/     \\__\\ |_______/ \n"
            "                                                                                                                                                                  \n";
    unsigned char mainMenu[] =
            "1 / Insert\n\t\\___ Usage: (1/Insert) <STRING>\n\t\\___ Help: Insert data into the program.\n"
            "2 / Remove\n\t\\___ Usage: (2/Remove) <DEFAULT VALUE TO DISPLAY IN CASE NOT FOUND>\n\t\\___ Help: Display data then Remove it from the program.\n"
            "3 / Modify\n\t\\___ Usage: (3/modify) <INDEX> <STRING>\n\t\\___ Help: Modify existing data or add new one.\n"
            "4 / Fetch\n\t\\___ Usage: (4/fetch) <INDEX> <DEFAULT VALUE TO DISPLAY IN CASE NOT FOUND>\n\t\\___ Help: Display data from the program.\n"
            "5 / PrintData\n\t\\___ Usage: (5/PrintData)\n\t\\___ Help: Display the data inside the program.\n"
            "6 / PeekEnd\n\t\\___ Usage: (6/peekEnd) <DEFAULT VALUE TO DISPLAY IN CASE NOT FOUND>\n\t\\___ Help: Display data at the end of the stored data.\n"
            "7 / PeekHead\n\t\\___ Usage: (7/peekHead) <DEFAULT VALUE TO DISPLAY IN CASE NOT FOUND>\n\t\\___ Help: Display the data at the beginning of the stored data.\n"
            "8 / InsertFront\n\t\\___ Usage: (8/InsertFront) <STRING>\n\t\\___ Help: Insert into the beginning of the data.\n"
            "9 / Undo\n\t\\___ Usage (9/Undo)\n\t\\___ Undo the last insert or delete.\n"
            "10 / CountElements\n\t\\___ Usage: (10/CountElements)\n\t\\___ Help: Display the number of the inserted data.\n"
            "11 /PrintMenu\n\t\\___ Usage: (11/printMenu)\n\t\\___ Help: Prints the menu.\n"
            "12 / PrintDetails\n\t\\___ Usage: (12/PrintDetails)\n\t\\___ Help: Display Diagnostic data of the program.\n"
            "13 / RemoveAll\n\t\\___ Usage: (13/RemoveAll)\n\t\\___ Help: DELETE EVERYTHING and EXIT.\n"
            "14 / Exit\n\t\\___ Usage: (14/Exit)\n\t\\___ Help: Exit the application.\n";

    printf("%s", ascii_art);
    printf("%s", mainMenu);
}
// Convert a string to lowerCase
void __toLowerString(char *string) {
    DEBUG_INFO("Entered __toLowerString function\n");
    for (unsigned char *p = (unsigned char*)string; *p; ++p) *p = *p > 0x40 && *p < 0x5b ? *p | 0x60 : *p;
    DEBUG_OKAY("Exiting __toLowerString function...\n");
}
// Helper function to split the user input into 3 parts
char **__inputSplitter(char *userInput) {
    DEBUG_INFO("Entered __inputSplitter function\n");
    DEBUG_INFO("Allocating memory for the split user input...\n");
    char **userInputSplit = malloc(3 * sizeof(char *));
    if (!userInputSplit) return NULL;
    DEBUG_OKAY("Allocated %lld bytes at 0x%p\n", sizeof(userInputSplit), userInputSplit);
    // Initialize to NULL to handle early returns
    for (int i = 0; i < 3; i++) {
        userInputSplit[i] = NULL;
    }
    DEBUG_INFO("Allocating a temporary buffer for the user input...\n");
    char *tempInput = (char *) malloc(strlen(userInput)+1);
    if (!tempInput) {
        free(userInputSplit);
        return NULL;
    }
    strcpy(tempInput, userInput);
    DEBUG_OKAY("Allocated %lld bytes at 0x%p\n", strlen(userInput)+1, tempInput);
    DEBUG_INFO("Initializing the token pointer...\n");
    char *token = strtok(tempInput, " ");
    DEBUG_OKAY("Got the token pointer\n\t\\___ Location: 0x%p\n\t\\___ Data: %s\n", token, token);
    for (int i = 0; i < 3 && token != NULL; i++) {
        DEBUG_INFO("Allocating memory for the %d user input...\n", i+1);
        userInputSplit[i] = malloc(strlen(token) + 1);
        DEBUG_OKAY("Allocated %lld bytes at 0x%p for the %d user input\n", strlen(token)+1, userInputSplit[i], i+1);
        if (!userInputSplit[i]) {
            for (int j = 0; j < i; j++) free(userInputSplit[j]);
            free(userInputSplit);
            free(tempInput);
            return NULL;
        }
        DEBUG_INFO("Copying the data from the token to the user input array...\n");
        strcpy(userInputSplit[i], token);
        DEBUG_INFO("Getting the new token pointer...\n");
        token = (i < 1) ? strtok(NULL, " ") : strtok(NULL, "\n");
        DEBUG_OKAY("Got the token pointer\n\t\\___ Location: 0x%p\n\t\\___ Data: %s\n", token, token);
    }
    DEBUG_INFO("Freeing %lld from 0x%p\n", sizeof(*tempInput), tempInput);
    free(tempInput);
    DEBUG_INFO("Exiting __inputSplitter function...\n");
    return userInputSplit;
}
// Helper function to free an array of strings
void __freeStrings(char **strings, int length) {
    if (!strings) return;
    for (int i = 0; i < length; i++) free((void *) strings[i]);
    free((void *) strings);
}
// Helper function to call the right function depending on numerical user input
void __numbersMenu(char** args, pQueueImpl storedData, pActionStack storedActionStack){
    int choice = strtol(args[0], (char**)NULL, 10);

    switch(choice){
        case 1:
            enQueueData(args, storedData, storedActionStack);
            break;

        case 2:
            deQueueData(args, storedData, storedActionStack);
            break;

        case 3:
            modifyData(args, storedData, storedActionStack);
            break;

        case 4:
            getQueueData(args, storedData);
            break;

        case 5:
            iterateQueue(storedData);
            break;

        case 6:
            peekEnd(args, storedData);
            break;

        case 7:
            peekHead(args, storedData);
            break;

        case 8:
            insertFront(args, storedData,storedActionStack);

            break;

        case 9:
            undoAction(storedData, storedActionStack);
            break;

        case 10:
            countElements(storedData);
            break;
        case 11:
            printMenu();
            break;
        case 12:
            printDiagnosticData(storedData);
            break;
        case 13:
            removeAll(storedData, storedActionStack);
            break;
        default:
            printf("Invalid Input\n");
    }

}
// Helper function to call the right function depending on user input
void __textMenu(char** args, pQueueImpl storedData, pActionStack storedActionStack){
    __toLowerString(args[0]);

    for (int i = 0; __commandMap[i].command != NULL; i++) {
        if (strcmp(args[0], __commandMap[i].command) == 0) {
            __commandMap[i].func(args, storedData, storedActionStack);
            return;
        }
    }

    printf("Invalid input\n");
    }
// Encapsulates all the appropriate functions for the running of this program
int mainWrapper(char* userInput, pQueueImpl DataQueue, pActionStack ActionStack){
    char **split;
    int choice;

    choice = strtol(userInput, NULL, 10);
    split = __inputSplitter(userInput);



    if (choice != 0) {
        if(choice == 14){
            __freeStrings(split, 3);
            return 1;
        }
        __numbersMenu(split, DataQueue, ActionStack);
        return 0;
    }

    __toLowerString(split[0]);
    if(strcmp(split[0], "exit\n") == 0){
        __freeStrings(split, 3);
        return 1;
    }
    __textMenu(split, DataQueue, ActionStack);

    return 0;
}