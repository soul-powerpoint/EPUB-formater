#include <stdio.h>
#include <zip.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

char *zip_read(zip_t *zip_file_archive, char *path, zip_uint64_t *size) {
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

char *get_opf_str(zip_t *zip_archive, char **full_path, zip_uint64_t *container_size, zip_uint64_t *opf_size) {
    char *container = zip_read(zip_archive, "META-INF/container.xml", container_size);
    xmlChar *full_path_src = container_parse(container);
    char *opf_str = zip_read(zip_archive, (char *) full_path_src, opf_size);
    printf("full_path_src = \n%s\n\n", full_path_src);

    *full_path = (char *) malloc(sizeof(char) * (strlen(full_path_src)) + 1);

    strcpy(*full_path, full_path_src);
    xmlFree(full_path_src);
    free(container);
    return opf_str;
}
