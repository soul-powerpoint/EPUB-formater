#ifndef LANGUAGE_CHANGE
#define LANGUAGE_CHANGE

#include <stdio.h>
#include <libxml/parser.h>

int language_change(zip_t *zip_archive, const char *opf_full_path, xmlDocPtr doc);

void language_change_main(char *path, const char *language);

#endif
