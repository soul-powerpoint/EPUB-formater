#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// int write_css_file(const char *class) {
//     FILE *file = fopen("override.css", "w");
//     if (file == NULL) {
//         fprintf(stderr, "Error: Could not open file for writing\n");
//         return 1;
//     }

//     fprintf(file, "/* auto-generated CSS file */\n\n");
//     for (int i = 0; i < rule_count; i++) {
//         fprintf(file, "%s {\n", rules[i].selector);
//         fprintf(file, "    %s: %s;\n", rules[i].property, rules[i].value);
//         fprintf(file, "}\n\n");
//     }

//     fclose(file);
//     printf("CSS file generated successfully.\n");
//     return 0;
// }
