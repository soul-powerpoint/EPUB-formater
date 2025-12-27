#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


xmlChar *rootfile_parse(xmlNode *currrent) {
    for (xmlNode *child = currrent->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE && xmlStrcmp(child->name, (const xmlChar *) "rootfile") == 0) {
            xmlChar *full_path = xmlGetProp(child, (const xmlChar *) "full-path");
            if (full_path) return full_path;
        }
    }

    return NULL;
}

// xmlChar *container_parse(char *xml_str) {
//     xmlInitParser();
//     xmlDocPtr xml_doc = xmlReadMemory(xml_str, (int)strlen(xml_str), NULL, NULL, 0);
//     xmlChar *full_path = NULL;

//     if (xml_doc == NULL) {
//         fprintf(stderr, "Error in container_parse\n");
//         return NULL;
//     }

//     xmlNode *root = xmlDocGetRootElement(xml_doc);
//     for (xmlNode *current = root->children; current; current = current->next) {
//         if (current->type == XML_ELEMENT_NODE && xmlStrcmp(current->name, (const xmlChar *) "rootfiles") == 0) {
//             full_path = rootfile_parse(current);
//         }
//     }

//     xmlFreeDoc(xml_doc);
//     xmlCleanupParser();
//     return full_path;
// }

xmlChar *container_parse(const char *container_xml) {
    xmlDocPtr doc = xmlReadMemory(container_xml, (int)strlen(container_xml),
                                  "container.xml", NULL,
                                  XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
    if (!doc) return NULL;

    xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
    if (!ctx) { xmlFreeDoc(doc); return NULL; }

    // Register the default namespace with a prefix, e.g. "c"
    xmlXPathRegisterNs(ctx, BAD_CAST "c",
                       BAD_CAST "urn:oasis:names:tc:opendocument:xmlns:container");

    xmlXPathObjectPtr obj =
        xmlXPathEvalExpression(
            BAD_CAST "string(/c:container/c:rootfiles/c:rootfile/@full-path)",
            ctx);

    xmlChar *res = NULL;
    if (obj && obj->stringval && obj->stringval[0] != '\0') {
        res = xmlStrdup(obj->stringval); // duplicate because obj will be freed
    }

    if (obj) xmlXPathFreeObject(obj);
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
    return res;  // caller must xmlFree()
}

