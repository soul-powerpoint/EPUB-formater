#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

void rootfile_parse(xmlNode *currrent) {
    for (xmlNode *child = currrent->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE && xmlStrcmp(child->name, (const xmlChar *) "rootfile") == 0) {
            xmlChar *fullpath = xmlGetProp(child, (const xmlChar *) "full-path");
            if (fullpath) {
                printf("full-path = %s\n", fullpath);
                xmlFree(fullpath);
            }
        }
    }
}

int container_parse(char *xml_str) {
    xmlInitParser();
    xmlDocPtr xml_doc = xmlReadMemory(xml_str, (int)strlen(xml_str), NULL, NULL, 0);
    if (xml_doc == NULL) {
        fprintf(stderr, "Error in container_parse\n");
        return 1;
    }

    xmlNode *root = xmlDocGetRootElement(xml_doc);
    for (xmlNode *current = root->children; current; current = current->next) {
        if (current->type == XML_ELEMENT_NODE && xmlStrcmp(current->name, (const xmlChar *)"rootfiles") == 0) {
            rootfile_parse(current);
        }
    }

    xmlFreeDoc(xml_doc);
    xmlCleanupParser();
    return 0;
}
