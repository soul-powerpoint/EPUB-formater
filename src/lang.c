#include "lang.h"
#include <string.h>
#include <ctype.h>

const char *const kLangCode[LANG_NULL] = {
    "en","zh","ja","ko","fr","de"
};

static int eq_code_ci(const char *a, const char *b) {
    // simple case-insensitive compare; treats '_' == '-'
    for (;;) {
        unsigned char ca = (unsigned char)*a++;
        unsigned char cb = (unsigned char)*b++;
        if (ca == '_') ca = '-';
        if (cb == '_') cb = '-';
        ca = (unsigned char)tolower(ca);
        cb = (unsigned char)tolower(cb);
        if (ca != cb) return 0;
        if (ca == '\0') return 1;
    }
}

language_type language_from_code(const char *code) {
    if (!code) return LANG_NULL;
    for (int i = 0; i < (int)LANG_NULL; ++i) {
        if (eq_code_ci(code, kLangCode[i])) return (language_type)i;
    }

    return LANG_NULL;
}
