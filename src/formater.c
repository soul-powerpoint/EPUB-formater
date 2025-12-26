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

// static void parse_arguments(int argc, char **argv) {
//     if (strcmp(argv[0], LANGUAGE) == 0) {
//         // if (argv[])
//     } else if (strcmp(argv[1], SIZE) == 0) {

//     }
// }

// static void get_opt(int argc, char *argv[]) {
//     int opt;

//     while((opt = getopt(argc, argv, ":l:h")) != -1) {
//         switch(opt) {
//             case 'l':
//                 printf("filename: %s\n", optarg);
//                 break;
//             case ':':
//                 printf("option needs a value\n");
//                 break;
//             case '?':
//                 printf("unknown option: %c\n", optopt);
//                 break;
//         }
//     }

//     for(; optind < argc; optind++){
//         printf("extra arguments: %s\n", argv[optind]);
//     }

//     return;
// }

int main (int argc, char **argv) {
    if (argc % 2 != 0) {
        fprintf(stderr, "usage: %s <file.epub>\n", argv[0]);
        return -1;
    }

    // parse_arguments(argc, argv[1]);
    // get_opt(argc, argv);

    if (strcmp(argv[2], LANGUAGE) == 0) {
        language_change_main(argv[1], argv[3]);
    } else {
        fprintf(stderr, "currently only support language changing\n");
        return -1;
    }

    return 0;
}
