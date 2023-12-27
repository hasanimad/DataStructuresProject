//
// Created by sdds on 12/27/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/MainWrapper.h"
#include "../Headers/ActionUndoLink.h"

typedef void (*CommandFunc)(char**, pQueueImpl, pActionStack);
typedef struct {
    char* command;
    CommandFunc func;
} __CommandMap;

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
            "3 / modify\n\t\\___ Usage: (3/modify) <INDEX> <STRING>\n\t\\___ Help: Modify existing data or add new one\n"
            "4 / fetch\n\t\\___ Usage: (4/fetch) <INDEX> <DEFAULT VALUE TO DISPLAY IN CASE NOT FOUND>\n\t\\___ Help: Display data from the program.\n"
            "5 / PrintData\n\t\\___ Usage: (5/PrintData)\n\t\\___ Help: Display the data inside the program\n"
            "6 / peekEnd\n\t\\___ Usage: (6/peekEnd) <DEFAULT VALUE TO DISPLAY IN CASE NOT FOUND>\n\t\\___ Help: Display data at the end of the stored data.\n"
            "7 / peekHead\n\t\\___ Usage: (7/peekHead) <DEFAULT VALUE TO DISPLAY IN CASE NOT FOUND>\n\t\\___ Help: Display the data at the beginning of the stored data.\n"
            "8 / insertFront\n\t\\___ Usage: (8/InsertFront) <STRING>\n\t\\___ Help: Insert into the beginning of the data.\n"
            "9 / Undo\n\t\\___ Usage (9/Undo)\n\t\\___ Undo the last insert or delete.\n"
            "10 / CountElements\n\t\\___ Usage: (10/CountElements)\n\t\\___ Help: Display the number of the inserted data.\n"
            "11 /printMenu\n\t\\___ Usage: (11/printMenu)\n\t\\___ Help: Prints the menu.\n"
            "12 / PrintDetails\n\t\\___ Usage: (12/PrintDetails)\n\t\\___ Help: Display Diagnostic data of the program.\n"
            "13 / RemoveAll\n\t\\___ Usage: (13/RemoveAll)\n\t\\___ Help: DELETE EVERYTHING and EXIT.\n"
            "14 / Exit\n\t\\___ Usage: (14/Exit)\n\t\\___ Help: Exit the application\n";

    printf("%s", ascii_art);
    printf("%s", mainMenu);
}

void __toLowerString(char *string) {
    for (char *p = string; *p; ++p) *p = *p > 0x40 && *p < 0x5b ? *p | 0x60 : *p;
}

char **__inputSplitter(char *userInput) {
    char **userInputSplit = malloc(3 * sizeof(char *));
    if (!userInputSplit) return NULL;

    // Initialize to NULL to handle early returns
    for (int i = 0; i < 3; i++) {
        userInputSplit[i] = NULL;
    }
    char *tempInput = (char *) malloc(strlen(userInput));
    if (!tempInput) {
        free(userInputSplit);
        return NULL;
    }
    strcpy(tempInput, userInput);

    char *token = strtok(tempInput, " ");
    for (int i = 0; i < 3 && token != NULL; i++) {

        userInputSplit[i] = malloc(strlen(token) + 1);

        if (!userInputSplit[i]) {
            for (int j = 0; j < i; j++) free(userInputSplit[j]);
            free(userInputSplit);
            free(tempInput);
            return NULL;
        }

        strcpy(userInputSplit[i], token);
        token = (i < 1) ? strtok(NULL, " ") : strtok(NULL, "\n");
    }

    free(tempInput);
    return userInputSplit;
}

void __freeStrings(char **strings, int length) {
    if (!strings) return;
    for (int i = 0; i < length; i++) free((void *) strings[i]);
    free((void *) strings);
}


void __numbersMenu(char** args, pQueueImpl storedData, pActionStack storedActionStack){
    int choice = strtol(args[0], (char**)NULL, 10);

    switch(choice){
        case 1:
            enQueueData(args, storedData, storedActionStack);
            break;

        case 2:
            // Implement a wrapper function to get the data then free it
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