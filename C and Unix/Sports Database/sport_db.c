// Assignment 4, Brandon Luu
// Sports Database using structures and arrays

#include <stdio.h>
#include <string.h>

// function definitions
void readline(char array[], int n);
void purge();

int main() {
    // event structure, hold 100 of them.
    struct event {
        int code, competitors;
        char gender, name[50];
    } events[100];
    int set[100] = {0}; // the struct array and this array are hashtables basically

    int counter = 0; // element counter

    printf("2211 Winter Olympics!\n");

    while (1) {
        char choice;
        
        // get input
        printf("\nEnter operation code: ");
        scanf("%c", &choice);
        purge();

        // print events
        if (choice == 'p') {
            if (counter == 0) {
                printf("       The database is empty.");
                continue;
            }
            printf("Event Code       Event Name                                               Competitors       Gender\n");
            for (int i = 0; i != 100; i++) {
                if (set[i] == 1) printf("%-17d%-57s%-18d%c\n", events[i].code, events[i].name, events[i].competitors, events[i].gender);
            }
        }
        // new event
        else if (choice == 'i') {
            // check if database is full
            if (counter >= 100) {
                printf("       The database is full.");
                continue;
            }
            
            int code, competitors;
            char name[50], gender;

            printf("       Enter event code (0-99): ");
            scanf("%d", &code);
            purge();
            if (code < 0 || code > 99) {
                printf("       Invalid event code range.");
                continue;
            }
            if (set[code] == 1) {
                printf("       That event code is already in the database.");
                continue;
            }
            printf("       Event event name (maximum 49 characters): ");
            readline(name, 50);
            printf("       Enter number of competitors (10-99): ");
            scanf("%d", &competitors);
            purge();
            if (competitors < 10 || competitors > 99) {
                printf("       Invalid number of competitors.");
                continue;
            }
            printf("       Enter the gender (M, W, X): ");
            scanf("%c", &gender);
            purge();
            if (gender != 'M' && gender != 'W' && gender != 'X') {
                printf("       Invalid gender selected.");
                continue;
            }

            // copy into hashtables
            set[code] = 1;
            events[code].code = code;
            strcpy(events[code].name, name);
            events[code].gender = gender;
            events[code].competitors = competitors;
            counter++;

            printf("Event #%d saved!", code);
        }
        // search for an event in database
        else if (choice == 's') {
            int code;

            printf("       Enter event code to search (0-99): ");
            scanf("%d", &code);
            purge();
            if (code < 0 || code > 99) {
                printf("       Invalid event code range.");
                continue;
            }
            if (set[code] == 0) {
                printf("       That event code does not exist in the database.");
                continue;
            }

            printf("Event Code       Event Name                                               Competitors       Gender\n");
            printf("%-17d%-57s%-18d%c\n", events[code].code, events[code].name, events[code].competitors, events[code].gender);
        }
        // update something in the database
        else if (choice == 'u') {
            int code, competitors;
            char name[50], gender;

            printf("       Enter event code to update (0-99): ");
            scanf("%d", &code);
            purge();
            if (code < 0 || code > 99) {
                printf("       Invalid event code range.");
                continue;
            }
            if (set[code] == 0) {
                printf("       That event code does not exist in the database.");
                continue;
            }
            printf("       Event event name (maximum 49 characters): ");
            readline(name, 50);
            printf("       Enter number of competitors (10-99): ");
            scanf("%d", &competitors);
            purge();
            if (competitors < 10 || competitors > 99) {
                printf("       Invalid event code range.");
                continue;
            }
            printf("       Enter the gender (M, W, X): ");
            scanf("%c", &gender);
            purge();
            if (gender != 'M' && gender != 'W' && gender != 'X') {
                printf("       Invalid gender selected.");
                continue;
            }
            set[code] = 1;
            events[code].code = code;
            strcpy(events[code].name, name);
            events[code].gender = gender;
            events[code].competitors = competitors;

            printf("Event #%d updated!", code);
        }
        // quit
        else if (choice == 'q') {
            printf("Quitting.\n\n");
            return 0;
        }
    }
}

// read up to n characters in a line, then add in the null character
void readline(char array[], int n) {
    int i = 0;
    int eat = 1;
    while (i != n-1) { // limit to n-2 characters
        char c = getchar();
        if (c != '\n') { // stop if we reached newline
            array[i] = c;
            i++;
        }
        else {
            eat = 0;
            break;
        }
    }
    array[i] = '\0'; // ith character will be null

    if (eat == 1) { // purge input if input string is larger than n-1
        purge();
    }
}

// purge input.
void purge() {
    while (getchar() != '\n');
}