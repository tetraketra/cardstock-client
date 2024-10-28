#include "toolkit/dlinkedlist.h"

/*
    Initializes a doubly linked list entry on the heap with data.

    @param `value`: The handle to init this entry's value with.

    @returns Pointer to the new `dlinkedlist_entry_t` {`.size>0`}.

    @internal
*/
static inline dlinkedlist_entry_t* dlinkedlist_entry_init(handle_t value) {
    RUNTIME_ASSERT(value.size > 0);

    dlinkedlist_entry_t* entry = calloc(1, sizeof(dlinkedlist_entry_t));
    entry->value = value;

    return entry;
}

/*
    Initializes a doubly linked list on the heap.

    @returns Pointer to the new `dlinkedlist_t`.
*/
dlinkedlist_t* dlinkedlist_init(void) {
    dlinkedlist_t* dlinkedlist = malloc(sizeof(dlinkedlist_t));

    dlinkedlist->head = NULL;
    dlinkedlist->tail = NULL;
    dlinkedlist->entries_n = 0;

    return dlinkedlist;
}

/*
    Frees a doubly linked list.

    @param `dlinkedlist`: The doubly linked list to free {`!NULL`}.
    @param `free_contents`: Whether to free the contained handles' data.
*/
void dlinkedlist_free(dlinkedlist_t* dlinkedlist, bool free_contents) {
    RUNTIME_ASSERT(dlinkedlist != NULL);
    
    /* Traverse forward, freeing as you go. */
    dlinkedlist_entry_t* head = dlinkedlist->head;

    while (head) {
        dlinkedlist_entry_t* next = head->next;
        if (free_contents) {
            FREE_HANDLE_SAFELY_WITH_FALLBACK(head->value);
        }
        FREE(head);
        head = next;
    }

    /* Free the container itself. */
    FREE(dlinkedlist);
}

/*
    Pushes a new data entry to the head of a doubly linked list.

    @param `dlinkedlist`: The doubly linked list {`!NULL`}.
    @param `value`: The handle to init this entry's value with {`.size>0`}.
*/
void dlinkedlist_push_head(dlinkedlist_t* dlinkedlist, handle_t value) {
    RUNTIME_ASSERT(dlinkedlist != NULL);
    RUNTIME_ASSERT(value.size > 0);

    /* Push special if list is empty, otherwise normal. */
    if (dlinkedlist->entries_n == 0) {
        dlinkedlist_entry_t* entry = dlinkedlist_entry_init(value);

        dlinkedlist->head = entry;
        dlinkedlist->tail = entry;
        dlinkedlist->entries_n++;
    } else {
        dlinkedlist_entry_t* entry = dlinkedlist_entry_init(value);

        entry->next = dlinkedlist->head;
        dlinkedlist->head->prev = entry;
        dlinkedlist->head = entry;
        dlinkedlist->entries_n++;
    }
}

/*
    Pushes a new data entry to the tail of a doubly linked list.

    @param `dlinkedlist`: The doubly linked list {`!NULL`}.
    @param `value`: The handle to init this entry's value with {`.size>0`}.
*/
void dlinkedlist_push_tail(dlinkedlist_t* dlinkedlist, handle_t value) {
    RUNTIME_ASSERT(dlinkedlist != NULL);
    RUNTIME_ASSERT(value.size > 0);

    /* Push special if list is empty, otherwise normal. */
    if (dlinkedlist->entries_n == 0) {
        dlinkedlist_entry_t* entry = dlinkedlist_entry_init(value);

        dlinkedlist->head = entry;
        dlinkedlist->tail = entry;
        dlinkedlist->entries_n++;
    } else {
        dlinkedlist_entry_t* entry = dlinkedlist_entry_init(value);

        entry->prev = dlinkedlist->tail;
        dlinkedlist->tail->next = entry;
        dlinkedlist->tail = entry;
        dlinkedlist->entries_n++;
    }
}

/*
    Gets the value at a specific index in a doubly linked list.

    @param `dlinkedlist`: The doubly linked list {`!NULL`}.
    @param `index`: The index of the value to get {`<dlinkedlist->entries_n`}.

    @returns The value at the index as a `handle_t`. Returns `(handle_t){0}` if the index is out of bounds.
*/
handle_t dlinkedlist_get(dlinkedlist_t* dlinkedlist, size_t index) {
    RUNTIME_ASSERT(dlinkedlist != NULL);
    RUNTIME_ASSERT(index < dlinkedlist->entries_n);

    /* Empty list guard. */
    if (!dlinkedlist->entries_n) {
        return (handle_t){0}; /* done */ /* didn't find (empty list) */
    }

    /* Traverse (backwards if faster). */
    dlinkedlist_entry_t* entry;
    if (index*2 < dlinkedlist->entries_n) {
        entry = dlinkedlist->head;

        for (size_t i = 0; i < index; i++) {
            entry = entry->next;
        }
    } else {
        entry = dlinkedlist->tail;

        for (size_t i = dlinkedlist->entries_n-1; i > index; i--) {
            entry = entry->prev;
        }
    }

    /* Return the data. */
    return entry->value; /* done */ /* found */
}

/*
    Sets the value at a specific index in a doubly linked list.

    @param `dlinkedlist`: The doubly linked list {`!NULL`}.
    @param `index`: The index of the value to set {`<dlinkedlist->entries_n`}.
    @param `value`: The handle to init this entry's value with {`.size>0`}.
    @param `free_old`: Whether to free the old value-handle's data.
*/
void dlinkedlist_set(dlinkedlist_t* dlinkedlist, size_t index, handle_t value, bool free_old) {
    RUNTIME_ASSERT(dlinkedlist != NULL);
    RUNTIME_ASSERT(index < dlinkedlist->entries_n);
    RUNTIME_ASSERT(value.size > 0);

    /* Empty list guard. */
    if (!dlinkedlist->entries_n) {
        return; /* done */ /* didn't find (empty list) */
    }

    /* Traverse (backwards if faster). */
    dlinkedlist_entry_t* entry;
    if (index*2 < dlinkedlist->entries_n) {
        entry = dlinkedlist->head;

        for (size_t i = 0; i < index; i++) {
            entry = entry->next;
        }
    } else {
        entry = dlinkedlist->tail;

        for (size_t i = dlinkedlist->entries_n-1; i > index; i--) {
            entry = entry->prev;
        }
    }


    /* Overwrite. */
    if (free_old) {
        FREE_HANDLE_SAFELY_WITH_FALLBACK(entry->value);
    }
    
    entry->value = value;

    /* Return. */
    return; /* done */ /* found and set */
}

/*
    Pops a value from the head of a doubly linked list.
    Does not free data.

    @param `dlinkedlist`: The doubly linked list {`!NULL`}.

    @returns The popped value as a `handle_t`. Returns `(handle_t){0}` if the dlinkedlist is empty.
*/
handle_t dlinkedlist_pop_head(dlinkedlist_t* dlinkedlist) {
    RUNTIME_ASSERT(dlinkedlist != NULL);

    /* Empty list guard. */
    if (!dlinkedlist->entries_n) {
        return (handle_t){0}; /* done */ /* didn't find (empty list) */
    }

    /* Extract the dlinkedlist's current head. */
    dlinkedlist_entry_t* old_head = dlinkedlist->head;
    dlinkedlist->entries_n--;

    /* Remove the current head from the dlinkedlist. */
    dlinkedlist->head = dlinkedlist->head->next;
    dlinkedlist->head->prev = NULL;
    if (!dlinkedlist->entries_n) { /* If this was the only entry, the tail is affected too. */
        dlinkedlist->tail = NULL;
    }

    /* Exctract the data from the old head. */
    handle_t data = old_head->value;
    FREE(old_head);

    /* Return the data. */
    return data; /* done */ /* found */
}

/*
    Pops a value from the tail of a doubly linked list.
    Does not free data.

    @param `dlinkedlist`: The doubly linked list {`!NULL`}.

    @returns The popped value as a `handle_t`. Returns `(handle_t){0}` if the dlinkedlist is empty.
*/
handle_t dlinkedlist_pop_tail(dlinkedlist_t* dlinkedlist) {
    RUNTIME_ASSERT(dlinkedlist != NULL);

    /* Empty list guard. */
    if (!dlinkedlist->entries_n) {
        return (handle_t){0}; /* done */ /* didn't find (empty list) */
    }

    /* Extract the dlinkedlist's current head. */
    dlinkedlist_entry_t* old_tail = dlinkedlist->tail;
    dlinkedlist->entries_n--;

    /* Remove the current head from the dlinkedlist. */
    dlinkedlist->tail = dlinkedlist->tail->prev;
    dlinkedlist->tail->next = NULL;
    /* If this was the only entry, the head is affected too. */
    if (!dlinkedlist->entries_n) { 
        dlinkedlist->head = NULL;
    }

    /* Exctract the data from the old head. */
    handle_t data = old_tail->value;
    FREE(old_tail);

    /* Return the data. */
    return data; /* done */ /* found */
}