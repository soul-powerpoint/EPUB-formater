#include <stdio.h>
#include <string.h>   // memcpy, strlen, strcpy, etc.
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <libxml/xpathInternals.h>
#include <zip.h>

#include "container_parse.h"
#include "opf_helper.h"
#include "defines.h"
#include "language_change.h"
#include "zip_helper.h"

int language_change(zip_t *zip_archive, const char *opf_full_path, xmlDocPtr doc) {
    if (!zip_archive || !opf_full_path || !doc) return -1;

    xmlChar *dump_file = NULL;
    int out_len = 0;
    const char *encoding = (doc->encoding) ? (const char *) doc->encoding : "UTF-8";

    xmlDocDumpMemoryEnc(doc, &dump_file, &out_len, encoding);
    if (!dump_file || out_len <= 0) return -1;

    // copy to malloc buffer so libzip can free() it safely
    char *owned = (char *)malloc((size_t)out_len);
    if (!owned) {
        xmlFree(dump_file);
        return -1;
    }
    memcpy(owned, dump_file, (size_t)out_len);
    xmlFree(dump_file);

    zip_int64_t index = zip_name_locate(zip_archive, opf_full_path, ZIP_FL_ENC_GUESS | ZIP_FL_NOCASE);
    zip_source_t *source = zip_source_buffer(zip_archive, owned, (zip_uint64_t)out_len, 1);
    if (!source) {
        // because source creation failed, libzip won't free it
        free(owned);
        return -1;
    }

    int flag = 0;
    if (index >= 0) {
        if (zip_file_replace(zip_archive, (zip_uint64_t)index, source, 0) != 0) flag = -1;
    } else {
        if (zip_file_add(zip_archive, opf_full_path, source, ZIP_FL_OVERWRITE) < 0) flag = -1;
    }

    if (flag != 0) {
        // frees owned (because freep=1)
        zip_source_free(source);
        return -1;
    }

    return 0;
}

void language_change_main(char *path, const char *language) {
    int err = 0;
    zip_t *zip_file_archive = zip_open(path, 0, &err);
    if (!zip_file_archive) {
        fprintf(stderr, "zip_open failed (err=%d)\n", err);
        return;
    }

    zip_uint64_t container_size = 0;
    zip_uint64_t opf_size = 0;

    char *full_path = NULL;
    char *opf_file = get_opf_str(zip_file_archive, &full_path, &container_size, &opf_size);
    if (!opf_file || !full_path) {
        fprintf(stderr, "error in language_change_main (opf/container read failed)\n");
        free(opf_file);
        free(full_path);
        zip_close(zip_file_archive);
        return;
    }

    if (opf_parse(zip_file_archive, opf_file, full_path, language) != 0) {
        fprintf(stderr, "opf_parse failed\n");
        free(opf_file);
        free(full_path);
        zip_discard(zip_file_archive);
        return;
    }

    free(opf_file);
    free(full_path);

    if (zip_close(zip_file_archive) != 0) {
        fprintf(stderr, "zip_close failed: %s\n", zip_strerror(zip_file_archive));
        zip_discard(zip_file_archive);
        return;
    }
}
