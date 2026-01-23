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
#include "language_change.h"
#include "zip_helper.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <file.epub> -l <lang>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[2], "-l") != 0) {
        fprintf(stderr, "usage: %s <file.epub> -l <lang>\n", argv[0]);
        return 1;
    }

    xmlInitParser();
    language_change_main(argv[1], argv[3]);
    xmlCleanupParser();
    return 0;
}
