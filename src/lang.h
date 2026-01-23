#pragma once
#include <stddef.h>

typedef enum {
    LANG_EN,
    LANG_ZH,
    LANG_JA,
    LANG_KO,
    LANG_FR,
    LANG_DE,
    LANG_NULL
} language_type;

extern const char *const kLangCode[LANG_NULL];

static inline const char *language_code(language_type id) {
    return (id >= 0 && id < LANG_NULL) ? kLangCode[id] : NULL;
}

language_type language_from_code(const char *code);
