#include <stdio.h>
#include <string.h>
#include <zip.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include "defines.h"

int language_change(zip_t *zip_archive, const char *opf_full_path, xmlDocPtr doc) {
    if (!zip_archive || !opf_full_path || !doc) return -1;

    xmlChar *dump_file = NULL;
    int out_len = 0;
    const char *encoding = (doc->encoding) ? (const char *) doc->encoding : "UTF-8";

    // xmlDocDumpFormatMemoryEnc
    xmlDocDumpMemoryEnc(doc, &dump_file, &out_len, encoding);
    if (dump_file == NULL) return -1;

    zip_int64_t index = zip_name_locate(zip_archive, opf_full_path, 0);
    struct zip_source *source = zip_source_buffer(zip_archive, dump_file, (zip_uint64_t) out_len, 1);
    if (!source) {
        xmlFree(dump_file);
        return -1;
    }

    int flag = 0;
    if (index >= 0) {
        if (zip_file_replace(zip_archive, (zip_uint64_t) index, source, 0) != 0) flag = -1;
    } else {
        if (zip_file_add(zip_archive, opf_full_path, source, ZIP_FL_OVERWRITE) < 0) flag = -1;
    }

    if (flag != 0) {
        zip_source_free(source);
        return -1;
    }

    return 0;
}

int child_node_process(xmlNode *currrent, const char *language) {
    for (xmlNode *child = currrent->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE && xmlStrcmp(child->name, (const xmlChar *) "language") == 0) {
            // if (xmlNodeSetContent(child, (xmlChar *) "en") != 0) return -1;
            xmlNodeSetContent(child, (xmlChar *) language);
        }
    }

    return 0;
}

int opf_parse(zip_t *zip_archive, char *xml_str, char *full_path, const char *language) {
    if (!zip_archive || !full_path || !xml_str) return -1;
    xmlInitParser();
    xmlDocPtr xml_doc = xmlReadMemory(xml_str, (int) strlen(xml_str), NULL, NULL, 0);

    if (xml_doc == NULL) {
        fprintf(stderr, "Error in opf_parse\n");
        return -1;
    }

    xmlNode *root = xmlDocGetRootElement(xml_doc);
    for (xmlNode *current = root->children; current; current = current->next) {
        if (current->type == XML_ELEMENT_NODE && xmlStrcmp(current->name, (const xmlChar *) "metadata") == 0) {
            if (child_node_process(current, language) != 0) return -1;
        }
    }

    if (language_change(zip_archive, full_path, xml_doc) != 0) return -1;

    xmlFreeDoc(xml_doc);
    xmlCleanupParser();
    return 0;
}
