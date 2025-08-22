#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <libxml/parser.h>

static void format_epub(char *path) {

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
