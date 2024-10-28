#ifndef TTK_DLINKEDLIST_H
#define TTK_DLINKEDLIST_H

#include "toolkit.h"
#include "handle.h"

/*
    === INFO ===
    1. Doubly linked list.
    2. Does not own data, as all values are stored as handles.
    3. Head/tail push/pop operations are O(1), exposed for stacks and queues.
    4. Index get/set operations are O(n/2). This is exposed for lists. Use an array instead.
*/

typedef struct dlinkedlist_entry_t dlinkedlist_entry_t;
typedef struct dlinkedlist_entry_t {
    dlinkedlist_entry_t* next;
    dlinkedlist_entry_t* prev;
    handle_t value;
} dlinkedlist_entry_t;

typedef struct dlinkedlist_t {
    dlinkedlist_entry_t* head;
    dlinkedlist_entry_t* tail;
    size_t entries_n;
} dlinkedlist_t;

extern dlinkedlist_t*   dlinkedlist_init(void);
extern void             dlinkedlist_free(dlinkedlist_t* dlinkedlist, bool free_contents);
extern void             dlinkedlist_push_head(dlinkedlist_t* dlinkedlist, handle_t value);
extern void             dlinkedlist_push_tail(dlinkedlist_t* dlinkedlist, handle_t value);
extern handle_t         dlinkedlist_pop_head(dlinkedlist_t* dlinkedlist);
extern handle_t         dlinkedlist_pop_tail(dlinkedlist_t* dlinkedlist);
extern handle_t         dlinkedlist_get(dlinkedlist_t* dlinkedlist, size_t index);
extern void             dlinkedlist_set(dlinkedlist_t* dlinkedlist, size_t index, handle_t value, bool free_old);

#endif