#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <zip.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "container_parse.h"
#include "opf_helper.h"
#include "defines.h"

static char *zip_read(zip_t *zip_file_archive, char *path, zip_uint64_t *size) {
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

static char *get_opf_str(zip_t *zip_archive, char *full_path, zip_uint64_t *container_size, zip_uint64_t *opf_size) {
    char *container = zip_read(zip_archive, "META-INF/container.xml", container_size);
    xmlChar *full_path_src = container_parse(container);
    char *opf_str = zip_read(zip_archive, (char *) full_path_src, opf_size);
    printf("full_path_src = \n%s\n\n", full_path_src);

    full_path = malloc(sizeof(char) * strlen(full_path_src));

    strcpy(full_path, full_path_src);
    free(container);
    return opf_str;
}

static void language_change(char *path, const char *language) {
    int err;
    zip_t *zip_file_archive = zip_open(path, 0, &err);

    zip_uint64_t container_size;
    zip_uint64_t opf_size;

    xmlChar *full_path = NULL;
    char *opf_file = get_opf_str(zip_file_archive, full_path, &container_size, &opf_size);

    printf("full_path = \n%s\n\n", full_path);
    printf("opf = \n%s\n\n", opf_file);

    free(opf_file);
    zip_close(zip_file_archive);
}

static void font_size_change(char *path) {

}

static void parse_arguments(int argc, char **argv) {
    if (strcmp(argv[0], LANGUAGE) == 0) {
        // if (argv[])
    } else if (strcmp(argv[1], SIZE) == 0) {

    }
}

static void get_opt(int argc, char *argv[]) {
    int opt;

    while((opt = getopt(argc, argv, ":l:h")) != -1) {
        switch(opt) {
            case 'l':
                printf("filename: %s\n", optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    for(; optind < argc; optind++){
        printf("extra arguments: %s\n", argv[optind]);
    }

    return;
}

int main (int argc, char **argv) {
    if (argc % 2 != 0) {
        fprintf(stderr, "usage: %s <file.epub>\n", argv[0]);
        return -1;
    }

    // parse_arguments(argc, argv[1]);
    // get_opt(argc, argv);

    if (strcmp(argv[2], LANGUAGE) == 0) {
        xmlInitParser();
        language_change(argv[1], argv[3]);
        xmlCleanupParser();
    } else {
        fprintf(stderr, "currently only support language changing\n", argv[0]);
        return -1;
    }

    return 0;
}
