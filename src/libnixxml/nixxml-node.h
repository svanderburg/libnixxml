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
    /** Indicates the type of node */
    NixXML_Type type;
    /** Actual data structure that represents the node of the provided type */
    void *value;
}
NixXML_Node;

/**
 * Deletes an object from heap memory.
 *
 * @param data Pointer to object to delete
 */
typedef void (*NixXML_DeleteObjectFunc) (void *data);

/**
 * Recursively deletes a node and all its node references.
 *
 * @param node Pointer to an NixXML_Node instance
 * @param delete_list Pointer to function that recursively deletes a list-like data structure
 * @param delete_table Pointer to a function that recursively deletes a table-like data structure
 */
void NixXML_delete_node(NixXML_Node *node, NixXML_DeleteObjectFunc delete_list, NixXML_DeleteObjectFunc delete_table);

#endif
