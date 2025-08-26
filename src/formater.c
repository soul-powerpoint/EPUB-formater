#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <libxml/parser.h>

static char *zip_read(zip_t *zip_file, const char *path, zip_uint64_t *size) {
    struct zip_stat zip_st;

    zip_stat_init(&zip_st);
}

static void format_epub(char *path) {
    int err;
    zip_t *zip_file = zip_open(path, 0, &err);

    zip_uint64_t container_size;
    char *container = zip_read(zip_file, "META-INF/container.xml");
}

int main (int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file1.epub>\n", argv[0]);
    }
    xmlInitParser();
    format_epub(argv[1]);
    xmlCleanupParser();

    return 0;
}

