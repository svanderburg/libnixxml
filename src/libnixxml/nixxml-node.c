#include "nixxml-node.h"
#include <libxml/parser.h>
#include <stdlib.h>

void NixXML_delete_node(NixXML_Node *node, NixXML_DeleteObjectFunc delete_list, NixXML_DeleteObjectFunc delete_attrset)
{
    if(node != NULL)
    {
        switch(node->type)
        {
            case NIX_XML_TYPE_STRING:
            case NIX_XML_TYPE_INT:
            case NIX_XML_TYPE_FLOAT:
            case NIX_XML_TYPE_BOOL:
                xmlFree(node->value);
                break;
            case NIX_XML_TYPE_LIST:
                delete_list(node->value);
                break;
            case NIX_XML_TYPE_ATTRSET:
                delete_attrset(node->value);
                break;
        }

        free(node);
    }
}
