#include <stdio.h>
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

    // xmlDocDumpFormatMemoryEnc
    xmlDocDumpMemoryEnc(doc, &dump_file, &out_len, encoding);
    if (dump_file == NULL) return -1;

    zip_int64_t index = zip_name_locate(zip_archive, opf_full_path, 0);
    struct zip_source *source = zip_source_buffer(zip_archive, dump_file, (zip_uint64_t) out_len, 1);
    if (!source) {
        xmlFree(dump_file);
        return -1;
    }

    int flag = 0;
    if (index >= 0) {
        if (zip_file_replace(zip_archive, (zip_uint64_t) index, source, 0) != 0) flag = -1;
    } else {
        if (zip_file_add(zip_archive, opf_full_path, source, ZIP_FL_OVERWRITE) < 0) flag = -1;
    }

    if (flag != 0) {
        zip_source_free(source);
        return -1;
    }

    return 0;
}

void language_change_main(char *path, const char *language) {
    xmlInitParser();

    int err;
    zip_t *zip_file_archive = zip_open(path, 0, &err);

    zip_uint64_t container_size;
    zip_uint64_t opf_size;

    char *full_path = NULL;
    char *opf_file = get_opf_str(zip_file_archive, &full_path, &container_size, &opf_size);

    if (!opf_file) {
        printf("error in language_change_main\n");
    }

    printf("full_path = \n%s\n\n", full_path);
    printf("opf = \n%s\n\n", opf_file);

    free(opf_file);
    free(full_path);
    zip_close(zip_file_archive);

    xmlCleanupParser();
}
