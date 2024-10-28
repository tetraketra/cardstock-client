#include "toolkit/hashtable.h"

/*
    Internal FNV-1a hash function.

    @param `key`: The key to hash {`!NULL`}.
    @param `key_sb`: The size of the key in bytes {`>0`}.

    @returns Hashed `uint64_t`.
*/
static uint64_t hash_fnv1a(void* key, size_t key_sb) {
    RUNTIME_ASSERT(key != NULL);
    RUNTIME_ASSERT(key_sb > 0);

    uint64_t hash = 0xcbf29ce484222325;

    for (size_t byte = 0; byte < key_sb; byte++) {
        hash = (hash ^ ((uint8_t*)key)[byte]) * 0x100000001b3;
    }

    return hash;
}

/*
    Internal default key equality function.

    @param `key1`: The *handle* of the first key to compare.
    @param `key2`: The *handle* of the second key to compare.

    @returns `true` if the handles' data are equal, `false` otherwise.

    @note Required Behavior:
    @note If only one handle's data is null, false.
    @note If both are null but they're the same size, true.
    @note If their sizes are different, false.
    @note If the handles' data is the same, true, otherwise false.
    @note *see `KEY_EQ_REQUIRED_BEHAVIOR` for first three steps

*/
static bool key_eq_default(handle_t key1, handle_t key2) {
    RUNTIME_ASSERT(key1.size != 0);
    RUNTIME_ASSERT(key2.size != 0);

    KEY_EQ_REQUIRED_BEHAVIOR(key1, key2);
    return memcmp(key1.data, key2.data, key1.size) == 0;
}

/*
    Initializes a hash table on the heap.
    Round Robin hashing (open addressing).
    Keys and values may be different sizes.
    Thanks to handles, sizing up (see `max_load`) does not cause stale external references.

    @param `initial_capacity`: The initial size of hash table. Doubles when `max_load` is exceeded.
    @param `key_eq`: Nullable alternate key equality function. See `key_eq_default` for expected behavior.
    @param `max_load`: The maximum load factor allowed before the hash table *automatically* rehashes and doubles in size.

    @returns Pointer to the new `hashtable_t`.

    @note https://thenumb.at/Hashtables/
    @note https://programming.guide/robin-hood-hashing.html
*/
hashtable_t* hashtable_init(size_t initial_capacity, bool (*key_eq)(handle_t, handle_t), float max_load) {
    RUNTIME_ASSERT(initial_capacity != 0);
    RUNTIME_ASSERT(max_load != 1.0f);

    hashtable_t* htable = malloc(sizeof(hashtable_t));
    *htable = (hashtable_t){0};

    htable->buckets = calloc(initial_capacity, sizeof(key_value_pair_t));
    htable->capacity = initial_capacity;
    htable->max_load = max_load;
    htable->key_eq = key_eq != NULL ? key_eq : key_eq_default;
    htable->count = 0;

    return htable;
}

/*
    Frees a hash table.

    @param `htable`: The hash table to free.
    @param `free_values`: Whether to free table's stored handles' data. If true, stales all external references.
*/
void hashtable_free(hashtable_t* htable, bool free_values) {
    RUNTIME_ASSERT(htable != NULL);

    for (size_t i = 0; i < htable->capacity; i++) {
        FREE_HANDLE_SAFELY_WITH_FALLBACK(htable->buckets[i].key);

        if (free_values) {
            FREE_HANDLE_SAFELY_WITH_FALLBACK(htable->buckets[i].value);
        }
    }

    free(htable->buckets);
    free(htable);
}

/*
    Inserts an entry into a hash table.
    Thanks to handles, this does not copy or move the data associated with the entry and does not stale any references.

    @param `htable`: The hash table to insert into.
    @param `key`: Any *handle* of the key you wish to insert.
    @param `value`: Any *handle* of the value you wish to insert.
    @param `free_old_kv_if_overwritten`: Whether to free the data of an existing key-value-pair's handles if overwritten. 
*/
void hashtable_insert(hashtable_t* htable, handle_t key, handle_t value, bool free_old_kv_if_overwritten) {
    RUNTIME_ASSERT(htable != NULL);
    RUNTIME_ASSERT(key.data != NULL);
    RUNTIME_ASSERT(key.size != 0);

    uint64_t ideal_index = hash_fnv1a(key.data, key.size)%htable->capacity;
    size_t index = ideal_index;

    for (size_t i = 0; i < htable->capacity; i++) { /* insert via Robin Hood method */
        key_value_pair_t* kv = &htable->buckets[index];

        if (kv->key.data == NULL) { /* empty, insert new, done */
            kv->key = key;
            kv->value = value;
            kv->ideal_index = ideal_index;
            htable->count++;
            break;
        }
        
        if (htable->key_eq(kv->key, key)) { /* same key, insert over, done */
            if (free_old_kv_if_overwritten) {
                FREE_HANDLE_SAFELY_WITH_FALLBACK(kv->key);
                FREE_HANDLE_SAFELY_WITH_FALLBACK(kv->value);
            }

            kv->key = key;
            kv->value = value;
            kv->ideal_index = ideal_index;
            break;

        } else { /* diff key, Robin Hood, check PSL */
            size_t kv_psl = UDISTANCE(index, kv->ideal_index); /* psl of existing kv */
            size_t key_psl = UDISTANCE(index, ideal_index); /* psl of new kv if inserted here */

            if (key_psl > kv_psl) { /* give spot to "poor" (far away) new key, swap, continue */
                key_value_pair_t tmp_kv = *kv;

                kv->key = key;
                kv->value = value;
                kv->ideal_index = ideal_index;

                key = tmp_kv.key;
                value = tmp_kv.value;
                ideal_index = tmp_kv.ideal_index;
            }
        }

        index = (index + 1) % htable->capacity; /* linear probe */
    }

    if (htable->count/(float)htable->capacity > 0.8) { /* load too high, double and rehash, also spooky magic number */
        DEBUG("Rehash triggered in hashtable %p. Capacity doubling from %zu to %zu. Load %zu/%zu = %f.\n",
            htable, 
            htable->capacity, htable->capacity*2, 
            htable->count, htable->capacity, htable->count/(float)htable->capacity
        );

        hashtable_t* tmp_htable = hashtable_init(htable->capacity*2, htable->key_eq, htable->max_load);

        for (size_t i = 0; i < htable->capacity; i++) {
            key_value_pair_t* kv = &htable->buckets[i];

            if (kv->key.data != NULL) { /* rehash into temp hashtable */
                hashtable_insert(tmp_htable, kv->key, kv->value, false);
            }
        }

        free(htable->buckets);
        *htable = *tmp_htable; /* overwrite with rehashed */
        free(tmp_htable);

        handle_t maybe_missing = hashtable_lookup(htable, key); 
        if (maybe_missing.data == NULL) { /* held item is only sometimes in the table after rehash */
            hashtable_insert(htable, key, value, free_old_kv_if_overwritten);
        }
    }
}

/*
    Fetches an entry from a hash table.

    @param `htable`: The hash table to fetch from.
    @param `key`: Any *handle* of the key you wish to free.

    @returns A handle to the value associated with that key, or a `NULL` handle if not found. 
*/
handle_t hashtable_lookup(hashtable_t* htable, handle_t key) {
    RUNTIME_ASSERT(htable != NULL);
    RUNTIME_ASSERT(key.data != NULL);
    RUNTIME_ASSERT(key.size != 0);

    size_t position = hash_fnv1a(key.data, key.size)%htable->capacity;

    for (size_t i = 0; i < htable->capacity; i++) { /* find value */
        key_value_pair_t* kv = &htable->buckets[position];

        if (kv->key.size == 0) { /* empty, not found, done */
            return (handle_t){0};
        }

        if (htable->key_eq(kv->key, key)) { /* same key, done */
            return kv->value;
        }

        position = (position + 1) % htable->capacity; /* linear probe */
    }

    return (handle_t){0}; /* exhausted the table, done */
}

/*
    Removes an entry from a hash table.

    @param `htable`: The hash table to remove from.
    @param `key`: Any *handle* of the key you wish to remove.
    @param `free_value`: Whether to free the value associated with the key. If true, stales all external references. 
    @param `free_passed_key_if_removed`: Whether to free the passed-in key if it is successfully removed. 
     If you delete a value using a different-loc-in-memory-but-equivalent key, this saves you the `free(dlimbe_key)` call.
*/
void hashtable_remove(hashtable_t* htable, handle_t key, bool free_value, bool free_passed_key_if_removed) {
    RUNTIME_ASSERT(htable != NULL);
    RUNTIME_ASSERT(key.data != NULL);
    RUNTIME_ASSERT(key.size != 0);

    uint64_t original_index = hash_fnv1a(key.data, key.size)%htable->capacity;
    size_t index = original_index;

    for (size_t i = 0; i < htable->capacity; i++) { /* find value to remove */
        key_value_pair_t* kv = &htable->buckets[index];

        if (kv->key.size == 0) { /* empty, not found, done */
            return;
        }

        if (htable->key_eq(kv->key, key)) { /* same key, free, begin backshift */
            if (free_passed_key_if_removed) {
                FREE_HANDLE_SAFELY_WITH_FALLBACK(key);
            }

            if (free_value) {
                FREE_HANDLE_SAFELY_WITH_FALLBACK(kv->value);
            }

            FREE_HANDLE_SAFELY_WITH_FALLBACK(kv->key);

            *kv = (key_value_pair_t){0};
            htable->count--;

            break; /* begin backshift */
        }

        index = (index + 1) % htable->capacity; /* linear probe */
    }

    index = (index + 1) % htable->capacity; /* move forward first, so we're looking at a movable value */
    for (size_t i = 0; i < htable->capacity; i++) { /* backshift */
        key_value_pair_t* kv = &htable->buckets[index];

        if (kv->key.data == NULL) { /* empty, current chain done shifting, done */
            return;
        }

        if (UDISTANCE(index, kv->ideal_index) > 0) { /* psl > 0, shift back */
            htable->buckets[(index-1)%htable->capacity] = *kv;
            *kv = (key_value_pair_t){0};
        } else { /* psl == 0, new chain start, current chain done shifting, done */
            return;
        }

        index = (index + 1) % htable->capacity;
    }

    ERROR("Impossible execution point reached in `hashtable_remove` (did you mess up the logic?).\n");
    return; /* default, !should never happen! */
}

/*
    Checks if an entry exists in a hash table.

    @param `htable`: The hash table to check.
    @param `key`: The key to check for.

    @returns `true` if the key is associated with a value, `false` otherwise.
*/
inline bool hashtable_contains(hashtable_t* htable, handle_t key) {
    return hashtable_lookup(htable, key).size != 0;
}