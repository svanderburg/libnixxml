/*
 * Copyright (c) 2019-2022 Sander van der Burg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file
 * @brief Node module
 * @defgroup Node
 * @{
 */

#ifndef __NIXXML_NODE_H
#define __NIXXML_NODE_H
#include "nixxml-types.h"

/**
 * @brief An enumeration of possible types for Nix nodes
 */
typedef enum
{
    NIX_XML_TYPE_STRING,
    NIX_XML_TYPE_INT,
    NIX_XML_TYPE_FLOAT,
    NIX_XML_TYPE_BOOL,
    NIX_XML_TYPE_PATH,
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
 * Compares two data structures representing a list or attribute set.
 *
 * @param left Pointer to a data structure
 * @param right Pointer to a data structure
 * @return TRUE if both data structures are equal, else FALSE
 */
typedef NixXML_bool (*NixXML_CompareObjectFunc) (const void *left, const void *right);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Recursively deletes a node and all its node references.
 *
 * @param node Pointer to an NixXML_Node instance
 * @param delete_list Pointer to function that recursively deletes a list-like data structure
 * @param delete_table Pointer to a function that recursively deletes a table-like data structure
 */
void NixXML_delete_node(NixXML_Node *node, NixXML_DeleteObjectFunc delete_list, NixXML_DeleteObjectFunc delete_table);

/**
 * Recursively compares a node and alls its node references to see whether they
 * are equal.
 *
 * @param left Pointer to an NixXML_Node instance
 * @param right Pointer to an NixXML_Node instance
 * @param compare_lists Pointer to a function that compares two list-like data structures
 * @param compare_tables Pointer to a function that compares two table-like data structures
 * @return TRUE if the object structures are equal, else FALSE
 */
NixXML_bool NixXML_compare_nodes(const NixXML_Node *left, const NixXML_Node *right, NixXML_CompareObjectFunc compare_lists, NixXML_CompareObjectFunc compare_tables);

#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */
