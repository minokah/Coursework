//To be implemented
struct event {
    int code, competitors;
    char name[50], gender;
    struct event *next;
};
void insert_event(struct event **a);
void search_event(struct event **a);
void update_event(struct event **a);
void print_event(struct event **a);
void dump_event(struct event **a);
void restore_event(struct event **a);
void erase_event(struct event **a);