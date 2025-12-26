#include <stdio.h>
#include <string.h>
#include <zip.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include "container_parse.h"
#include "opf_helper.h"
#include "defines.h"
#include "language_change.h"
#include "zip_helper.h"

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
