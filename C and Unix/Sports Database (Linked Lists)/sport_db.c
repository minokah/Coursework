// Assignment 5 - Brandon Luu
// sport_db - Sport database for events and athletes

// Bonus command registration added for the extra 10%!!

/*********************************************************
 * From C PROGRAMMING: A MODERN APPROACH, Second Edition *
 * By K. N. King                                         *
 * Copyright (c) 2008, 1996 W. W. Norton & Company, Inc. *
 * All rights reserved.                                  *
 * This program may be freely distributed for class use, *
 * provided that this copyright notice is retained.      *
 *********************************************************/

/* Adopted from inventory2.c (Chapter 17, page 434) by Will Beldman */
/* Maintains an Olympics database (linked list version) */

#include "sport_db.h"
#include <stddef.h>
#include <stdio.h>

int main(void)
{
    struct event *event_tbl = NULL;   /* points to first event */
    struct athlete *athlete_tbl = NULL;   /* points to first athlete */
    struct registration *registration_tbl = NULL; // points to first registration

    char entity, code;
    printf("****************************\n");
    printf("* 2211 Winter Olympics!    *\n");
    printf("****************************\n\n");

    for (;;) {
        printf("Enter entity type (h for help, q to quit, e for events, a for athletes, r for registration): ");
        scanf(" %c", &entity);
        while (getchar() != '\n')   /* skips to end of line */
        ;
        if (entity == 'h'){
            print_help();
        }
        else if (entity == 'e'){
            printf("Event:\n\t");
            printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\td to dump, r to restore,\n\te to erase): ");
            scanf(" %c", &code);
            while (getchar() != '\n')   /* skips to end of line */
                ;

            switch (code) {
                case 'i': insert_event(&event_tbl);
                            break;
                case 's': search_event(&event_tbl);
                            break;
                case 'u': update_event(&event_tbl);
                            break;
                case 'p': print_event(&event_tbl);
                            break;
                case 'd': dump_event(&event_tbl);
                            break;
                case 'r': restore_event(&event_tbl);
                            break;
                case 'e': erase_event(&event_tbl);
                            break;
                default:  printf("Illegal code\n");
            }
        }
        else if (entity == 'a'){
            printf("Athlete:\n\t");
            printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\td to dump, r to restore,\n\te to erase): ");
            scanf(" %c", &code);
            while (getchar() != '\n')   /* skips to end of line */
                ;

            switch (code) {
                case 'i':  insert_athlete(&athlete_tbl);
                            break;
                case 's': search_athlete(&athlete_tbl);
                            break;
                case 'u': update_athlete(&athlete_tbl);
                            break;
                case 'p': print_athlete(&athlete_tbl);
                            break;
                case 'd': dump_athlete(&athlete_tbl);
                            break;
                case 'r': restore_athlete(&athlete_tbl);
                            break;
                case 'e': erase_athlete(&athlete_tbl);
                            break;
                default:  printf("Illegal code\n");
            }
        }
        else if (entity == 'r'){
            printf("Registration:\n\t");
            printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\te to erase): ");
            scanf(" %c", &code);
            while (getchar() != '\n')   /* skips to end of line */
                ;

            switch (code) {
                case 'i':  insert_registration(&registration_tbl, &event_tbl, &athlete_tbl);
                            break;
                case 's': search_registration(&registration_tbl);
                            break;
                case 'u': update_registration(&registration_tbl, &event_tbl, &athlete_tbl);
                            break;
                case 'p': print_registration(&registration_tbl);
                            break;
                case 'e': erase_registration(&registration_tbl);
                            break;
                default:  printf("Illegal code\n");
            }
        }
        else if (entity == 'q'){
            return 0;
        }
        else{
            printf("Illegal entity\n");
        }
        printf("\n");
    }
}

void print_help(void){
    printf("\nEnter e for event, a for athlete, then\n");
    printf("Enter operation code\n\t(i to insert, s to search,\n\tu to update, p to print,\n\td to dump, r to restore,\n\te to erase)\n\n ");
}
