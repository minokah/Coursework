// Assignment 5 - Brandon Luu
// sport_athlete - Linked List operations for athlete

#include "sport_athlete.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*
/* dump: Dumps contents of the athlete_tbl to specified
 *       file. Prints error message and does not
 *       terminate program upon failure. Does not store
 *       next pointer.
*/
void dump_athlete(struct athlete **athlete_tbl)
{
    FILE *fp;
    char filename[255];
    struct athlete **node = athlete_tbl;

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
        fwrite(*node, sizeof(struct athlete) - sizeof(struct athlete *), 1, fp);
        node = &(*node)->next;
    }
    fclose(fp);
    return;
}

/* restore: Restores contents from specified file. Does
 *          not terminate upon file open failure, but
 *          terminates upon malloc failure.
*/
void restore_athlete(struct athlete **athlete_tbl)
{
    FILE *fp;
    char filename[255];
    struct athlete buffer;
    struct athlete *temp;
    struct athlete **node;

    printf("Enter name of input file: ");
    fgets(filename, 255, stdin);
    filename[strlen(filename) - 1] = '\0';

    if ((fp = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr, "Error: file %s cannot be opened\n", filename);
        return;
    }

    while (*athlete_tbl)
    {
        temp = *athlete_tbl;
        *athlete_tbl = (*athlete_tbl)->next;
        free(temp);
    }

    node = athlete_tbl;

    while (fread(&buffer,sizeof(struct athlete) - sizeof(struct athlete *),1,fp) == 1)
    {
        if ((*node = malloc(sizeof(struct athlete))) == NULL)
        {
            fprintf(stderr, "Error: malloc failed in restore\n");
            exit(EXIT_FAILURE);
        }
        (*node)->code = buffer.code;
        strcpy((*node)->name, buffer.name);
        (*node)->age = buffer.age;
        (*node)->gender = buffer.gender;
        (*node)->next = NULL;
        node = &(*node)->next;
    }
    fclose(fp);
    return;
}

// Insert an athelte
void insert_athlete(struct athlete **a) {
    printf("-- Athlete Insertion --\n");
    struct athlete *newAthlete = calloc(1, sizeof(struct athlete));
    // Check if full
    if (newAthlete == NULL) {
        printf("Database is full!\n");
        free(newAthlete);
        return;
    }

    printf("Enter athlete code: ");
    scanf("%d", &newAthlete->code);
    while (getchar() != '\n');
    // traversal
    struct athlete *codeCheck = *a;
    while (codeCheck != NULL) {
        if (codeCheck->code == newAthlete->code) {
            printf("Athlete code already exists!\n");
            free(newAthlete);
            return;
        }
        codeCheck = codeCheck->next;
    }

    //  Prompt for info
    printf("Enter age of athlete (16-99): ");
    scanf("%d", &newAthlete->age);
    while (getchar() != '\n');
    if (newAthlete->age < 16 || newAthlete->age > 99) {
        printf("Invalid number of age!\n");
        free(newAthlete);
        return;
    }

    printf("Enter athlete gender (M, W, X): ");
    scanf("%c", &newAthlete->gender);
    while (getchar() != '\n');
    if (newAthlete->gender != 'M' && newAthlete->gender != 'W' && newAthlete->gender != 'X') {
        printf("Invalid gender selected!\n");
        free(newAthlete);
        return;
    }

    printf("Enter athlete name (max 48 characters): ");
    fgets(newAthlete->name, 50, stdin);
    newAthlete->name[strlen(newAthlete->name) - 1] = '\0';
    while (strlen(newAthlete->name) >= 48 && getchar() != '\n');
    
    // Traverse linked list and insert
    struct athlete *current = *a;
    if (current == NULL) *a = newAthlete;
    else {
        while (current->next != NULL) current = current->next;
        current->next = newAthlete;
    }

    printf("Athlete %d successfully inserted.\n", newAthlete->code);
}

// Search for an athlete
void search_athlete(struct athlete **a) {
    int code;
    printf("-- Athlete Search --\nEnter athlete code: ");
    scanf("%d", &code);
    while (getchar() != '\n');

    struct athlete *current = *a;
    while (current != NULL) {
        if (current->code == code) {
            printf("Athlete Code       Athlete Name                                               Age       Gender\n");
            printf("%-19d%-59s%-10d%c\n", current->code, current->name, current->age, current->gender);
            return;
        }
        current = current->next;
    }

    printf("Athlete %d not found!\n", code);
}

// Update an athlete
void update_athlete(struct athlete **a) {
    int code, age;
    char name[50], gender;

    printf("-- Athlete Update --\nEnter athlete code: ");
    scanf("%d", &code);
    while (getchar() != '\n');
    // traversal
    struct athlete *current = *a;
    int found = 0;
    while (current != NULL) {
        if (current->code == code) {
            found = 1;
            break;
        }
        current = current->next;
    }

    if (found == 0) {
        printf("Athlete %d not found!\n", code);
        return;
    }

    // Prompt for info
    printf("Enter age of athlete (16-99): ");
    scanf("%d", &age);
    while (getchar() != '\n');
    if (age < 16 || age > 99) {
        printf("Invalid number of age!\n");
        return;
    }

    printf("Enter athlete gender (M, W, X): ");
    scanf("%c", &gender);
    while (getchar() != '\n');
    if (gender != 'M' && gender != 'W' && gender != 'X') {
        printf("Invalid gender selected!\n");
        return;
    }

    printf("Enter athlete name (max 48 characters): ");
    fgets(name, 50, stdin);
    name[strlen(name) - 1] = '\0';
    while (strlen(current->name) >= 48 && getchar() != '\n');

    strcpy(current->name, name);
    current->gender = gender;
    current->age = age;

    printf("Athlete %d successfully updated.\n", code);
}

// Print out list of athletes
void print_athlete(struct athlete **a) {
    struct athlete *current = *a;
    if (current == NULL) {
        printf("No athletes in database!\n");
        return;
    }
    printf("Athlete Code       Athlete Name                                               Age       Gender\n");
    while (current != NULL) {
        printf("%-19d%-59s%-10d%c\n", current->code, current->name, current->age, current->gender);
        current = current->next;
    }
}

// Erase an athlete
void erase_athlete(struct athlete **a) {
    int code;
    printf("-- Athlete Deletion --\n");
    // Check if empty
    if (*a == NULL) {
        printf("No athletes in the database!\n");
        return;
    }
    printf("Enter athlete code: ");
    scanf("%d", &code);
    while (getchar() != '\n');

    struct athlete *current = *a;
    // first node
    if (current->next == NULL && current->code == code) {
        free(current);
        *a = NULL;
        printf("Athlete %d successfully deleted.\n", code);
        return;
    }
    // regular traversal
    else {
        while (current->next != NULL) {
            if (current->next->code == code) {
                struct athlete *next = current->next->next;
                free(current->next);
                current->next = next;
                printf("Athlete %d successfully deleted.\n", code);
                return;
            }
        }
    }
    
    printf("Athlete %d was not found!\n", code);
}