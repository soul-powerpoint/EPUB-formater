#ifndef ZIP_HELPER
#define ZIP_HELPER

#include <stdio.h>
#include <zip.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

char *zip_read(zip_t *zip_file_archive, const char *path, zip_uint64_t *size);

char *get_opf_str(zip_t *zip_archive, char **full_path, zip_uint64_t *container_size, zip_uint64_t *opf_size);

#endif
