#include "transtext.h"

static char languages[TTK_TRANSTEXT_LANG_MAXCNT][TTK_TRANSTEXT_LANG_MAXLEN];
static size_t lang_n; /* Number of languages that actually exist. */
static size_t lang_i; /* Currently-selected langauge (by index). */

/*
    Clear transtext's internal language tracker.
*/
void transtext_reflangs_clearall(void) {
    for (size_t i = 0; i < TTK_TRANSTEXT_LANG_MAXCNT; i++) {
        memset(languages[i], '\0', TTK_TRANSTEXT_LANG_MAXLEN);
    }
}

/*
    Registers a langauge with transtext for use with translated text.
    Expects all `transtext_t` to support this language.

    @param lang null-terminated langauge to register,
    e.g. "English" literal or "English\0" non-literal.
*/
void transtext_reflangs_add(const char* lang) {
    strncpy(languages[lang_n], lang, TTK_TRANSTEXT_LANG_MAXLEN);
    languages[lang_n++][TTK_TRANSTEXT_LANG_MAXLEN - 1] = '\0'; /* C-strings make me paranoid T>T. */
}

/*
    Selects a language for all `transtext_t` structs to use with `transtext_get`.

    @param lang null-terminated language to select.
    
    @returns `true` if language selected, `false` if not able to.
*/
bool transtext_reflang_select(const char* lang) {
    if (strncmp(lang, languages[lang_i], TTK_TRANSTEXT_LANG_MAXLEN) == 0) { /* Check if we're already on that language. */
        return true; /* done */ /* already selected */ 
    }

    for (size_t i = 0; i < lang_n; i++) { /* Simple linear search to find the chosen language. */
        if (strncmp(lang, languages[i], TTK_TRANSTEXT_LANG_MAXLEN) == 0) {
            lang_i = i;
            return true; /* done */ /* found and selected*/
        }
    }

    return false; /* done */ /* not found */
}

/*
    Get the index in the static `languages` object of a given `lang`.

    @internal

    @returns The index, or `SIZE_MAX` if failed.
*/
static size_t _transtext_get_lang_i(const char* lang) {
    for (size_t i = 0; i < lang_n; i++) {
        if (strncmp(lang, languages[i], TTK_TRANSTEXT_LANG_MAXLEN) == 0) {
            return i;
        }
    }

    return SIZE_MAX;
}

/*
    Adds a translation to a `transtext_t` struct. When you `transtext_get` 
    this same struct, the correct translation will be selected according 
    to the last use of `transtext_reflang_select`.

    @param transtext the `transtext` struct to add a new translation to.
    @param lang the language to add a translation for.
    @param text the text to add as a translation.

    @returns `true` ideally, but `false` if the `lang` key was not recognized.
*/
bool transtext_translation_add(transtext_t* transtext, const char* lang, const char* text) {
    size_t index = _transtext_get_lang_i(lang);

    if (index == SIZE_MAX) {
        return false; /* done */ /* lang key not found */
    }

    size_t text_len = strlen(text);
    transtext->_translations[index] = calloc(1, text_len + 1);
    strncpy(transtext->_translations[index], text, text_len + 1);
    transtext->_translations[index][text_len] = '\0';

    return true; /* done */ /* translation added */
}

/*
    Gets the correct translation inside the transtext as 
    determined by the last call to `transtext_reflang_select`. 

    @param transtext The transtext to fetch the current translation from.

    @returns The translated text as determined by the last call to `transtext_reflang_select`.
*/
char* transtext_translation_get(transtext_t* transtext) {
    return transtext->_translations[lang_i];
}

/*
    Initializes a transtext on the heap.

    Use `transtext_reflangs_add` >>before<< any instances of this.
    Use `transtext_translation_add` to add translations to this transtext.

    @returns Pointer to the new transtext.
*/
transtext_t* transtext_init(void) {
    transtext_t* transtext = calloc(1, sizeof(transtext_t));

    return transtext;
}

/*
    Frees a transtext and all its internal translations.

    @param transtext The transtext to free.
*/
void transtext_free(transtext_t* transtext) {
    for (size_t i = 0; i < lang_n; i++) {
        if (transtext->_translations[i] != NULL) {
            FREE(transtext->_translations[i]);
        }
    }

    free(transtext);
}
