/*
 * Copyright (c) 2019 Sander van der Burg
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

#ifndef __NIXXML_XMLHASHTABLE_H
#define __NIXXML_XMLHASHTABLE_H
#include <libxml/parser.h>
#include "nixxml-parse.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"
#include "nixxml-generate-env.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new xmlHashTable by parsing the sub elements of a provided XML
 * element.
 *
 * @param element XML element to parse.
 * @param userdata Arbitrary user data that is propagated to all parse functions
 * @return A pointer to an xmlHashTable
 */
void *NixXML_create_xml_hash_table(xmlNodePtr element, void *userdata);

/**
 * Inserts a value into an xmlHashTable
 *
 * @param table Pointer to an xmlHashTable
 * @param key Attribute name to use as a key for the hash table
 * @param value Pointer to a value to insert
 * @param userdata Arbitrary user data that is propagated to all parse functions
 */
void NixXML_insert_into_xml_hash_table(void *table, const xmlChar *key, void *value, void *userdata);

/**
 * Prints a Nix representation of all members in the table. The keys are
 * traversed in the order in which they appear in the table.
 *
 * @param file File descriptor to write to
 * @param value An xmlHashTable instance
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

/**
 * Prints a Nix representation of an attribute set from an xmlHashTable. It
 * traverses the keys in the order in which they appear in the table.
 *
 * @param file File descriptor to write to
 * @param hash_table Pointer to an xmlHashTable
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_nix(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

/**
 * Prints a Nix representation of all members in the table. The keys are
 * traversed in a deterministic order.
 *
 * @param file File descriptor to write to
 * @param value An xmlHashTable instance
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_ordered_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

/**
 * Prints a Nix representation of an attribute set from an xmlHashTable. It
 * traverses in a deterministic order.
 *
 * @param file File descriptor to write to
 * @param hash_table Pointer to an xmlHashTable
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_ordered_nix(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

/**
 * Prints a simple XML representation of all members in the table. The keys are
 * traversed in the order in which they appear in the table.
 *
 * @param file File descriptor to write to
 * @param value An xmlHashTable instance
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_simple_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a simple XML representation of an attribute set from an xmlHashTable.
 * It traverses the keys in the order in which they appear in the table.
 *
 * @param file File descriptor to write to
 * @param hash_table An xmlHashTable instance
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_simple_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a simple XML representation of all members in the table. The keys are
 * traversed in a deterministic order.
 *
 * @param file File descriptor to write to
 * @param value An xmlHashTable instance
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_simple_ordered_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a simple XML representation of an attribute set from an xmlHashTable.
 * It traverses the keys in a deterministic order.
 *
 * @param file File descriptor to write to
 * @param hash_table An xmlHashTable instance
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_simple_ordered_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a verbose XML representation of all members in the table. The keys are
 * traversed in the order in which they appear in the table.
 *
 * @param file File descriptor to write to
 * @param value An xmlHashTable instance
 * @param child_element_name Name of each attribute child element
 * @param name_property_name Name of the name property
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_verbose_attributes_xml(FILE *file, const void *value, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a verbose XML representation of an attribute set from an xmlHashTable.
 * It traverses the keys in the order in which they appear in the table.
 *
 * @param file File descriptor to write to
 * @param hash_table An xmlHashTable instance
 * @param child_element_name Name of each attribute child element
 * @param name_property_name Name of the name property
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_verbose_xml(FILE *file, xmlHashTablePtr hash_table, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a verbose XML representation of all members in the table. The keys are
 * traversed in a deterministic order.
 *
 * @param file File descriptor to write to
 * @param value An xmlHashTable instance
 * @param child_element_name Name of each attribute child element
 * @param name_property_name Name of the name property
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_verbose_ordered_attributes_xml(FILE *file, const void *value, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a verbose XML representation of an attribute set from an xmlHashTable.
 * It traverses the keys in a deterministic order.
 *
 * @param file File descriptor to write to
 * @param hash_table An xmlHashTable instance
 * @param child_element_name Name of each attribute child element
 * @param name_property_name Name of the name property
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_xml_hash_table_verbose_ordered_xml(FILE *file, xmlHashTablePtr hash_table, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Parses an xmlHashTable from the sub elements of a provided XML element using
 * the simple notation.
 *
 * @param element XML element to parse.
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param parse_object Pointer to a function that parses the value
 * @return A pointer to an xmlHashTable
 */
void *NixXML_parse_xml_hash_table_simple(xmlNodePtr element, void *userdata, NixXML_ParseObjectFunc parse_object);

/**
 * Parses an xmlHashTable from the sub elements of a provided XML element using
 * the verbose notation.
 *
 * @param element XML element to parse.
 * @param child_element_name Name of each attribute child element
 * @param name_property_name Name of the name property
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param parse_object Pointer to a function that parses the value
 * @return A pointer to an xmlHashTable
 */
void *NixXML_parse_xml_hash_table_verbose(xmlNodePtr element, const char *child_element_name, const char *name_property_name, void *userdata, NixXML_ParseObjectFunc parse_object);

/**
 * Generates a string vector of environment variable assignments from all
 * members in a given hash table.
 *
 * @param hash_table Pointer to an xmlHashTable
 * @param userdata Arbitrary user data that is propagated to all generate functions
 * @param generate_value A pointer to a function that generates an environment variable representation of a provided value
 */
xmlChar **NixXML_generate_env_vars_from_xml_hash_table(xmlHashTablePtr hash_table, void *userdata, NixXML_GenerateEnvValueFunc generate_value);

#ifdef __cplusplus
}
#endif

#endif
