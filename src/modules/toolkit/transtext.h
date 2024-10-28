#ifndef TTK_TRANSTEXT_H
#define TTK_TRANSTEXT_H

#include "toolkit.h"

/*
    === INFO ===
    1. Text structs which can change their contents on the fly (by selected language).
    2. All strings are expected to be null-terminated (such as string literals).
    3. The idea is to load a scene with all translations of all text you need, then swap on demand.
*/

/* Maximum number of supported languages. */
#define TTK_TRANSTEXT_LANG_MAXLEN 30

/* Maximum length of a language name. */
#define TTK_TRANSTEXT_LANG_MAXCNT 50

typedef struct transtext_t {
    char* _translations[TTK_TRANSTEXT_LANG_MAXCNT]; /* Internal. Do not touch. */
} transtext_t;

extern void             transtext_reflangs_clearall(void);
extern void             transtext_reflangs_add(const char* lang);
extern bool             transtext_reflang_select(const char* lang);
extern bool             transtext_translation_add(transtext_t* transtext, const char* lang, const char* text);
extern char*            transtext_translation_get(transtext_t* transtext);
extern transtext_t*     transtext_init(void);
extern void             transtext_free(transtext_t* transtext);

#endif 