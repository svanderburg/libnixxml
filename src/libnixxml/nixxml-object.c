#include "nixxml-object.h"
#include <libxml/parser.h>
#include <stdlib.h>

void NixXML_delete_object(NixXML_Object *obj, NixXML_DeleteObjectFunc delete_list, NixXML_DeleteObjectFunc delete_attrset)
{
    if(obj != NULL)
    {
        switch(obj->type)
        {
            case NIX_XML_TYPE_STRING:
            case NIX_XML_TYPE_INT:
            case NIX_XML_TYPE_FLOAT:
            case NIX_XML_TYPE_BOOL:
                xmlFree(obj->value);
                break;
            case NIX_XML_TYPE_LIST:
                delete_list(obj->value);
                break;
            case NIX_XML_TYPE_ATTRSET:
                delete_attrset(obj->value);
                break;
        }

        free(obj);
    }
}
