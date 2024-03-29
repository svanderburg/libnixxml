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
 * @brief Data-DS module
 * @defgroup Data-DS
 * @{
 */

#ifndef __NIXXML_DS_H
#define __NIXXML_DS_H

#include <libxml/parser.h>
#include "nixxml-node.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Recursively deletes a node and all its node references including data
 * structures managed by this sub library.
 *
 * @param node Pointer to an NixXML_Node instance
 */
void NixXML_delete_node_ds(NixXML_Node *node);

/**
 * Recursively compares two node object trees and checks whether they have equal
 * content.
 *
 * @param left Pointer to an NixXML_Node instance
 * @param right Pointer to an NixXML_Node instance
 * @return TRUE if the object structures are equal, else FALSE
 */
NixXML_bool NixXML_compare_nodes_ds(const NixXML_Node *left, const NixXML_Node *right);

/**
 * Recursively parses a type-annotated XML document (with simple notation for
 * attribute sets) using xmlHashTable structs for representing attribute sets
 * and pointer arrays for representing lists.
 *
 * @param element Root element of the XML document
 * @param type_property_name Name of the attribute that contains the type annotation
 * @param userdata Parameters propagated to all parse functions
 * @return A NixXML_Node
 */
void *NixXML_generic_parse_simple_expr_ds(xmlNodePtr element, const char *type_property_name, void *userdata);

/**
 * Recursively parses a type-annotated XML document (with verbose notation for
 * attribute sets) using xmlHashTable structs for representing attribute sets
 * and pointer arrays for representing lists.
 *
 * @param element Root element of the XML document
 * @param type_property_name Name of the attribute that contains the type annotation
 * @param name_property_name Name of the attribute that refers to the name of attribute
 * @param userdata Parameters propagated to all parse functions
 * @return A NixXML_Node
 */
void *NixXML_generic_parse_verbose_expr_ds(xmlNodePtr element, const char *type_property_name, const char *name_property_name, void *userdata);

/**
 * Generically prints a Nix representation of a data structure of XML_Node
 * objects using xmlHashTable structs for representing attribute sets and
 * pointer arrays for representing lists. It traverses the keys in the order in
 * which they appear in the table.
 *
 * @param file File descriptor to write to
 * @param node Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 */
void NixXML_print_generic_expr_ds_nix(FILE *file, const NixXML_Node *node, const int indent_level);

/**
 * Generically prints a Nix representation of a data structure of XML_Node
 * objects using xmlHashTable structs for representing attribute sets and
 * pointer arrays for representing lists. It traverses the keys in a
 * deterministic order.
 *
 * @param file File descriptor to write to
 * @param node Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 */
void NixXML_print_generic_expr_ds_ordered_nix(FILE *file, const NixXML_Node *node, const int indent_level);

/**
 * Generically prints a simple XML representation of a data structure of XML_Node
 * objects using xmlHashTable structs for representing attribute sets and
 * pointer arrays for representing lists. It traverses the keys in the order in
 * which they appear in the table.
 *
 * @param file File descriptor to write to
 * @param node Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param root_element_name Name of the root element
 * @param list_element_name Name of the list elements
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 */
void NixXML_print_generic_expr_ds_simple_xml(FILE *file, const NixXML_Node *node, const int indent_level, const char *root_element_name, const char *list_element_name, const char *type_property_name);

/**
 * Generically prints a simple XML representation of a data structure of XML_Node
 * objects using xmlHashTable structs for representing attribute sets and
 * pointer arrays for representing lists. It traverses the keys in a
 * deterministic order.
 *
 * @param file File descriptor to write to
 * @param node Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param root_element_name Name of the root element
 * @param list_element_name Name of the list elements
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 */
void NixXML_print_generic_expr_ds_simple_ordered_xml(FILE *file, const NixXML_Node *node, const int indent_level, const char *root_element_name, const char *list_element_name, const char *type_property_name);

/**
 * Generically prints a verbose XML representation of a data structure of
 * XML_Node objects using xmlHashTable structs for representing attribute sets
 * and pointer arrays for representing lists. It traverses the keys in the order
 * in which they appear in the table.
 *
 * @param file File descriptor to write to
 * @param node Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param root_element_name Name of the root element
 * @param attr_element_name Attribute element name
 * @param name_property_name Name of the name property
 * @param list_element_name Name of the list elements
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 */
void NixXML_print_generic_expr_ds_verbose_xml(FILE *file, const NixXML_Node *node, const int indent_level, const char *root_element_name, const char *attr_element_name, const char *name_property_name, const char *list_element_name, const char *type_property_name);

/**
 * Generically prints a verbose XML representation of a data structure of
 * XML_Node objects using xmlHashTable structs for representing attribute sets
 * and pointer arrays for representing lists. It traverses the keys in a
 * deterministic order.
 *
 * @param file File descriptor to write to
 * @param node Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param root_element_name Name of the root element
 * @param attr_element_name Attribute element name
 * @param name_property_name Name of the name property
 * @param list_element_name Name of the list elements
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 */
void NixXML_print_generic_expr_ds_verbose_ordered_xml(FILE *file, const NixXML_Node *node, const int indent_level, const char *root_element_name, const char *attr_element_name, const char *name_property_name, const char *list_element_name, const char *type_property_name);

/**
 * Generates a string vector of environment variable assignments from all
 * members in a given hash table of NixXML_Node values. It uses pointer arrays
 * for representing lists and xmlHashTables for representing tables.
 *
 * @param hash_table Pointer to an xmlHashTable
 */
xmlChar **NixXML_generate_env_vars_generic_ds(xmlHashTablePtr hash_table);

#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */
