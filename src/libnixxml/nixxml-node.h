#ifndef __NIXXML_NODE_H
#define __NIXXML_NODE_H

/**
 * @brief An enumeration of possible types for Nix nodes
 */
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

/**
 * @brief Captures the properties of an AST node
 */
typedef struct
{
    NixXML_Type type;
    void *value;
}
NixXML_Node;

typedef void (*NixXML_DeleteObjectFunc) (void *data);

void NixXML_delete_node(NixXML_Node *node, NixXML_DeleteObjectFunc delete_list, NixXML_DeleteObjectFunc delete_attrset);

#endif
