//To be implemented
struct athlete {
    int code, age;
    char name[50], gender;
    struct athlete *next;
};
void insert_athlete(struct athlete **a);
void search_athlete(struct athlete **a);
void update_athlete(struct athlete **a);
void print_athlete(struct athlete **a);
void dump_athlete(struct athlete **a);
void restore_athlete(struct athlete **a);
void erase_athlete(struct athlete **a);