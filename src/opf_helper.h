#ifndef OPF_LANGUAGE
#define OPF_LANGUAGE

#include <stdio.h>
#include <zip.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

int opf_parse(zip_t *za, char *xml_str, char *full_path);

#endif
