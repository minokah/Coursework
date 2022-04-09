//To be implemented
#include "sport_events.h"
#include "sport_athlete.h"

struct registration {
    int code, event, athlete;
    struct registration *next;
};
void insert_registration(struct registration **r, struct event **e, struct athlete **a);
void search_registration(struct registration **r);
void update_registration(struct registration **r, struct event **e, struct athlete **a);
void print_registration(struct registration **r);
void erase_registration(struct registration **r);