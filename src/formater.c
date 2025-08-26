#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <libxml/parser.h>

static char *zip_read(zip_t *zip_file_archive, const char *path, zip_uint64_t *size) {
    struct zip_stat zip_st;
    zip_stat_init(&zip_st);
    zip_stat(zip_file_archive, path, 0, &zip_st);

    zip_file_t *zip_file = zip_fopen(zip_file_archive, path, 0);
    char *buf = malloc((zip_st.size + 1) * sizeof(char));
    zip_fread(zip_file, buf, zip_st.size);
    buf[zip_st.size] = '\0';
    zip_fclose(zip_file);

    *size = zip_st.size;
    return buf;
}

static void format_epub(char *path) {
    int err;
    zip_t *zip_file_archive = zip_open(path, 0, &err);

    zip_uint64_t container_size;
    char *container = zip_read(zip_file_archive, "META-INF/container.xml", &container_size);
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

