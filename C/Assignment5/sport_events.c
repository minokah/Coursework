// Assignment 5 - Brandon Luu
// sport_events - Linked List operations for event

#include "sport_events.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* dump: Dumps contents of the event_tbl to specified
 *       file. Prints error message and does not
 *       terminate program upon failure. Does not store
 *       next pointer.
*/
void dump_event(struct event **event_tbl)
{
    FILE *fp;
    char filename[255];
    struct event **node = event_tbl;

    printf("Enter name of output file: ");
    fgets(filename, 255, stdin);
    filename[strlen(filename) - 1] = '\0';

    if ((fp = fopen(filename, "wb")) == NULL)
    {
        fprintf(stderr, "File %s could not be written\n", filename);
        return;
    }

    while (*node)
    {
        fwrite(*node, sizeof(struct event) - sizeof(struct event *), 1, fp);
        node = &(*node)->next;
    }
    fclose(fp);
    return;
}

/* restore: Restores contents from specified file. Does
 *          not terminate upon file open failure, but
 *          terminates upon malloc failure.
*/
void restore_event(struct event **event_tbl)
{
    FILE *fp;
    char filename[255];
    struct event buffer;
    struct event *temp;
    struct event **node;

    printf("Enter name of input file: ");
    fgets(filename, 255, stdin);
    filename[strlen(filename) - 1] = '\0';

    if ((fp = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr, "Error: file %s cannot be opened\n", filename);
        return;
    }

    while (*event_tbl)
    {
        temp = *event_tbl;
        *event_tbl = (*event_tbl)->next;
        free(temp);
    }

    node = event_tbl;

    while (fread(&buffer,sizeof(struct event) - sizeof(struct event *),1,fp) == 1)
    {
        if ((*node = malloc(sizeof(struct event))) == NULL)
        {
            fprintf(stderr, "Error: malloc failed in restore\n");
            exit(EXIT_FAILURE);
        }
        (*node)->code = buffer.code;
        strcpy((*node)->name, buffer.name);
        (*node)->competitors = buffer.competitors;
        (*node)->gender = buffer.gender;
        (*node)->next = NULL;
        node = &(*node)->next;
    }
    fclose(fp);
    return;
}

// Event insertion
void insert_event(struct event **a) {
    printf("-- Event Insertion --\n");
    struct event *newEvent = calloc(1, sizeof(struct event));
    // Database full
    if (newEvent == NULL) {
        printf("Database is full!\n");
        free(newEvent);
        return;
    }

    printf("Enter event code: ");
    scanf("%d", &newEvent->code);
    while (getchar() != '\n');

    // traversal
    struct event *codeCheck = *a;
    while (codeCheck != NULL) {
        if (codeCheck->code == newEvent->code) {
            printf("Event code already exists!\n");
            free(newEvent);
            return;
        }
        codeCheck = codeCheck->next;
    }

    // Prompt for info
    printf("Enter number of competitors (10-99): ");
    scanf("%d", &newEvent->competitors);
    while (getchar() != '\n');
    if (newEvent->competitors < 10 || newEvent->competitors > 99) {
        printf("Invalid number of competitors!\n");
        free(newEvent);
        return;
    }

    printf("Enter event gender (M, W, X): ");
    scanf("%c", &newEvent->gender);
    while (getchar() != '\n');
    if (newEvent->gender != 'M' && newEvent->gender != 'W' && newEvent->gender != 'X') {
        printf("Invalid gender selected!\n");
        free(newEvent);
        return;
    }

    printf("Enter event name (max 48 characters): ");
    fgets(newEvent->name, 50, stdin);
    newEvent->name[strlen(newEvent->name) - 1] = '\0';
    while (strlen(newEvent->name) >= 48 && getchar() != '\n');

    // traverse linkedlist (if needed) and add it
    struct event *current = *a;
    if (current == NULL) *a = newEvent;
    else {
        while (current->next != NULL) current = current->next;
        current->next = newEvent;
    }

    printf("Event %d successfully inserted.\n", newEvent->code);
}

// Search for an event
void search_event(struct event **a) {
    int code;
    printf("-- Event Search --\nEnter event code: ");
    scanf("%d", &code);
    while (getchar() != '\n');

    struct event *current = *a;
    while (current != NULL) {
        if (current->code == code) {
            printf("Event Code       Event Name                                               Competitors       Gender\n");
            printf("%-17d%-57s%-18d%c\n", current->code, current->name, current->competitors, current->gender);
            return;
        }
        current = current->next;
    }

    printf("Event %d not found!\n", code);
}

// Update an event
void update_event(struct event **a) {
    int code, competitors;
    char name[50], gender;

    printf("-- Event Update --\nEnter event code: ");
    scanf("%d", &code);
    while (getchar() != '\n');
    // traversal
    struct event *current = *a;
    int found = 0;

    // Check if found
    while (current != NULL) {
        if (current->code == code) {
            found = 1;
            break;
        }
        current = current->next;
    }

    if (found == 0) {
        printf("Event %d not found!\n", code);
        return;
    }

    printf("Enter number of competitors (10-99): ");
    scanf("%d", &competitors);
    while (getchar() != '\n');
    if (competitors < 10 || competitors > 99) {
        printf("Invalid number of competitors!\n");
        return;
    }

    printf("Enter event gender (M, W, X): ");
    scanf("%c", &gender);
    while (getchar() != '\n');
    if (gender != 'M' && gender != 'W' && gender != 'X') {
        printf("Invalid gender selected!\n");
        return;
    }

    printf("Enter event name (max 48 characters): ");
    fgets(name, 50, stdin);
    name[strlen(name) - 1] = '\0';
    while (strlen(current->name) >= 48 && getchar() != '\n');

    // Update it
    strcpy(current->name, name);
    current->gender = gender;
    current->competitors = competitors;

    printf("Event %d successfully updated.\n", code);
}

// Print list of events
void print_event(struct event **a) {
    struct event *current = *a;
    if (current == NULL) {
        printf("No events in database!\n");
        return;
    }
    printf("\nEvent Code       Event Name                                               Competitors       Gender\n");
    while (current != NULL) {
        printf("%-17d%-57s%-18d%c\n", current->code, current->name, current->competitors, current->gender);
        current = current->next;
    }
}

// Erase an event
void erase_event(struct event **a) {
    int code;
    printf("-- Event Deletion --\n");
    // Check if empty
    if (*a == NULL) {
        printf("No events in the database!\n");
        return;
    }
    printf("Enter event code: ");
    scanf("%d", &code);
    while (getchar() != '\n');

    struct event *current = *a;
    // first node
    if (current->next == NULL && current->code == code) {
        free(current);
        *a = NULL;
        printf("Event %d successfully deleted.\n", code);
        return;
    }
    // regular traversal
    else {
        while (current->next != NULL) {
            if (current->next->code == code) {
                struct event *next = current->next->next;
                free(current->next);
                current->next = next;
                printf("Event %d successfully deleted.\n", code);
                return;
            }
        }
    }
    
    printf("Event %d was not found!\n", code);
}