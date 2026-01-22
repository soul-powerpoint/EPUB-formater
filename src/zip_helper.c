#include <stdio.h>
#include <string.h>
#include <zip.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <ctype.h>
#include <string.h>

#include "container_parse.h"
#include "opf_helper.h"
#include "defines.h"
#include "language_change.h"
#include "zip_helper.h"

static int str_ieq(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == *b;
}

static int str_iendswith(const char *s, const char *suffix) {
    size_t ls = strlen(s);
    size_t lf = strlen(suffix);
    if (lf > ls) return 0;
    return str_ieq(s + (ls - lf), suffix);
}

static zip_int64_t locate_entry(zip_t *za, const char *name) {
    zip_flags_t flags = ZIP_FL_ENC_GUESS | ZIP_FL_NOCASE;
    zip_int64_t idx = zip_name_locate(za, name, flags);
    if (idx >= 0) return idx;

    zip_int64_t n = zip_get_num_entries(za, 0);
    for (zip_int64_t i = 0; i < n; i++) {
        const char *en = zip_get_name(za, (zip_uint64_t)i, ZIP_FL_ENC_GUESS);
        if (!en) continue;
        if (str_ieq(en, name)) return i;
    }

    if (str_iendswith(name, "container.xml")) {
        for (zip_int64_t i = 0; i < n; i++) {
            const char *en = zip_get_name(za, (zip_uint64_t)i, ZIP_FL_ENC_GUESS);
            if (!en) continue;
            if (str_iendswith(en, "container.xml")) return i;
        }
    }

    return -1;
}

char *zip_read(zip_t *zip_file_archive, const char *path, zip_uint64_t *size) {
    if (size) *size = 0;
    if (!zip_file_archive || !path) return NULL;

    zip_int64_t idx = locate_entry(zip_file_archive, path);
    if (idx < 0) {
        fprintf(stderr, "zip_read: entry not found: %s\n", path);
        return NULL;
    }

    zip_stat_t st;
    zip_stat_init(&st);
    if (zip_stat_index(zip_file_archive, (zip_uint64_t)idx, 0, &st) != 0) {
        fprintf(stderr, "zip_read: zip_stat_index failed for %s\n", path);
        return NULL;
    }

    zip_file_t *zf = zip_fopen_index(zip_file_archive, (zip_uint64_t)idx, 0);
    if (!zf) {
        fprintf(stderr, "zip_read: zip_fopen_index failed for %s\n", path);
        return NULL;
    }

    char *buf = (char *)malloc((size_t)st.size + 1);
    if (!buf) {
        zip_fclose(zf);
        return NULL;
    }

    zip_uint64_t off = 0;
    while (off < st.size) {
        zip_int64_t n = zip_fread(zf, buf + off, st.size - off);
        if (n < 0) {
            fprintf(stderr, "zip_read: zip_fread failed for %s\n", path);
            free(buf);
            zip_fclose(zf);
            return NULL;
        }
        if (n == 0) break;
        off += (zip_uint64_t)n;
    }

    buf[off] = '\0';
    if (size) *size = off;
    zip_fclose(zf);
    return buf;
}

/**
 * Get the content in opf file
 */
char *get_opf_str(zip_t *zip_archive, char **full_path, zip_uint64_t *container_size, zip_uint64_t *opf_size) {
    char *container = zip_read(zip_archive, "META-INF/container.xml", container_size);
    printf("container_size=%llu\n\n", (unsigned long long)*container_size);
    fwrite(container, 1, *container_size, stdout);
    printf("\n");

    xmlChar *full_path_src = container_parse(container);
    if (!full_path_src) {
        fprintf(stderr, "Error in container_parse: full-path not found\n");
        free(container);
        return NULL;
    }

    char *opf_str = zip_read(zip_archive, (char *) full_path_src, opf_size);
    printf("full_path_src = \n%s\n\n", full_path_src);

    *full_path = (char *) malloc(sizeof(char) * (strlen(full_path_src)) + 1);

    strcpy(*full_path, full_path_src);
    xmlFree(full_path_src);
    free(container);
    return opf_str;
}
