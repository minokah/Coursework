// Assignment 5 - Brandon Luu
// sport_registration - Linked List operations for registration
// Bonus 10%!!

#include "sport_registration.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Insert a registration
void insert_registration(struct registration **r, struct event **e, struct athlete **a) {
    printf("-- Registration Insertion --\n");
    struct registration *newRegistration = calloc(1, sizeof(struct registration));
    // Check if full
    if (newRegistration == NULL) {
        printf("Database is full!\n");
        free(newRegistration);
        return;
    }

    printf("Enter registration code: ");
    scanf("%d", &newRegistration->code);
    while (getchar() != '\n');
    // traversal
    struct registration *regCheck = *r;
    while (regCheck != NULL) {
        if (regCheck->code == newRegistration->code) {
            printf("Registration code already exists!\n");
            free(newRegistration);
            return;
        }
        regCheck = regCheck->next;
    }

    // Prompt for info
    // Event prompt
    int eventFound = 0;
    printf("Enter event code: ");
    scanf("%d", &newRegistration->event);
    while (getchar() != '\n');

    struct event *evCheck = *e;
    while (evCheck != NULL) {
        if (evCheck->code == newRegistration->event) {
            eventFound = 1;
            break;
        }
        evCheck = evCheck->next;
    }

    if (eventFound == 0) {
        printf("Event %d not doesn't exist!\n", newRegistration->event);
        free(newRegistration);
        return;
    }

    // Athlete prompt
    int athleteFound = 0;
    printf("Enter athlete code: ");
    scanf("%d", &newRegistration->athlete);
    while (getchar() != '\n');

    struct athlete *athCheck = *a;
    while (athCheck != NULL) {
        if (athCheck->code == newRegistration->athlete) {
            athleteFound = 1;
            break;
        }
        athCheck = athCheck->next;
    }

    if (athleteFound == 0) {
        printf("Athlete %d not doesn't exist!\n", newRegistration->athlete);
        free(newRegistration);
        return;
    }
    
    // Traverse linked list and insert
    struct registration *current = *r;
    if (current == NULL) *r = newRegistration;
    else {
        while (current->next != NULL) current = current->next;
        current->next = newRegistration;
    }

    printf("Registration %d successfully inserted.\n", newRegistration->code);
}

// Search for a registration
void search_registration(struct registration **a) {
    int code;
    printf("-- Registration Search --\nEnter registration code: ");
    scanf("%d", &code);

    struct registration *current = *a;
    while (current != NULL) {
        if (current->code == code) {
            printf("Registration Code       Event Code       Athlete Code\n");
            printf("%-24d%-17d%d\n", current->code, current->event, current->athlete);
            return;
        }
        current = current->next;
    }

    printf("Registration %d not found!\n", code);
}

// Update a registration
void update_registration(struct registration **r, struct event **e, struct athlete **a) {
    int code, event, athlete;

    printf("-- Athlete Update --\nEnter athlete code: ");
    scanf("%d", &code);
    while (getchar() != '\n');
    // traversal
    int found = 0;
    struct registration *current = *r;
    while (current != NULL) {
        if (current->code == code) {
            found = 1;
            break;
        }
        current = current->next;
    }

    if (found == 0) {
        printf("Registration does not exist!\n");
        return;
    }

    // Prompt for info
    // Event prompt
    int eventFound = 0;
    printf("Enter event code: ");
    scanf("%d", &event);
    while (getchar() != '\n');

    struct event *evCheck = *e;
    while (evCheck != NULL) {
        if (evCheck->code == event) {
            eventFound = 1;
            break;
        }
        evCheck = evCheck->next;
    }

    if (eventFound == 0) {
        printf("Event %d not doesn't exist!\n", &event);
        return;
    }

    // Athlete prompt
    int athleteFound = 0;
    printf("Enter athlete code: ");
    scanf("%d", &athlete);
    while (getchar() != '\n');

    struct athlete *athCheck = *a;
    while (athCheck != NULL) {
        if (athCheck->code == athlete) {
            athleteFound = 1;
            break;
        }
        athCheck = athCheck->next;
    }

    if (athleteFound == 0) {
        printf("Athlete %d not doesn't exist!\n", &athlete);
        return;
    }

    current->event = event;
    current->athlete = athlete;

    printf("Registration %d successfully updated.\n", code);
}

// Print out list of athletes
void print_registration(struct registration **r) {
    struct registration *current = *r;
    if (current == NULL) {
        printf("No registrations in database!\n");
        return;
    }
    printf("\nRegistration Code       Event Code       Athlete Code\n");
    while (current != NULL) {
        printf("%-24d%-17d%d\n", current->code, current->event, current->athlete);
        current = current->next;
    }
}

// Erase an athlete
void erase_registration(struct registration **a) {
    int code;
    printf("-- Registration Deletion --\n");
    // Check if empty
    if (*a == NULL) {
        printf("No registrations in the database!\n");
        return;
    }
    printf("Enter registration code: ");
    scanf("%d", &code);
    while (getchar() != '\n');

    struct registration *current = *a;
    // first node
    if (current->next == NULL && current->code == code) {
        free(current);
        *a = NULL;
        printf("Registration %d successfully deleted.\n", code);
        return;
    }
    // regular traversal
    else {
        while (current->next != NULL) {
            if (current->next->code == code) {
                struct registration *next = current->next->next;
                free(current->next);
                current->next = next;
                printf("Registration %d successfully deleted.\n", code);
                return;
            }
        }
    }
    
    printf("Registration %d was not found!\n", code);
}