#include <stdio.h>
#include <zip.h>

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