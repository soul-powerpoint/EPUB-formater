#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

xmlChar *rootfile_parse(xmlNode *currrent) {
    for (xmlNode *child = currrent->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE && xmlStrcmp(child->name, (const xmlChar *) "rootfile") == 0) {
            xmlChar *full_path = xmlGetProp(child, (const xmlChar *) "full-path");
            if (full_path) return full_path;
        }
    }

    return NULL;
}

xmlChar *container_parse(char *xml_str) {
    xmlInitParser();
    xmlDocPtr xml_doc = xmlReadMemory(xml_str, (int)strlen(xml_str), NULL, NULL, 0);
    xmlChar *full_path = NULL;

    if (xml_doc == NULL) {
        fprintf(stderr, "Error in container_parse\n");
        return NULL;
    }

    xmlNode *root = xmlDocGetRootElement(xml_doc);
    for (xmlNode *current = root->children; current; current = current->next) {
        if (current->type == XML_ELEMENT_NODE && xmlStrcmp(current->name, (const xmlChar *)"rootfiles") == 0) {
            full_path = rootfile_parse(current);
        }
    }

    xmlFreeDoc(xml_doc);
    xmlCleanupParser();
    return full_path;
}
