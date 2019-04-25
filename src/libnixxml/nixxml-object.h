#ifndef __NIXXML_OBJECT_H
#define __NIXXML_OBJECT_H

typedef enum
{
    NIX_XML_TYPE_STRING,
    NIX_XML_TYPE_INT,
    NIX_XML_TYPE_FLOAT,
    NIX_XML_TYPE_BOOL,
    NIX_XML_TYPE_LIST,
    NIX_XML_TYPE_ATTRSET
}
NixXML_Type;

typedef struct
{
    NixXML_Type type;
    void *value;
}
NixXML_Object;

typedef void (*NixXML_DeleteObjectFunc) (void *data);

void NixXML_delete_object(NixXML_Object *obj, NixXML_DeleteObjectFunc delete_list, NixXML_DeleteObjectFunc delete_attrset);

#endif
