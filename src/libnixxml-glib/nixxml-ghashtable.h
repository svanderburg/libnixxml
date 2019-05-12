#ifndef __NIXXML_GHASHTABLE_H
#define __NIXXML_GHASHTABLE_H
#include <glib.h>
#include <libxml/parser.h>
#include "nixxml-parse.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"

/**
 * Creates a new GHashTable by parsing the sub elements of a provided XML
 * element.
 *
 * @param element XML element to parse.
 * @param userdata Arbitrary user data that is propagated to all parse functions
 * @return A pointer to a GHashTable
 */
void *NixXML_create_g_hash_table(xmlNodePtr element, void *userdata);

/**
 * Inserts a value into a GHashTable
 *
 * @param table Pointer to a GHashTable
 * @param key Attribute name to use as a key for the hash table
 * @param value Pointer to a value to insert
 * @param userdata Arbitrary user data that is propagated to all parse functions
 */
void NixXML_insert_into_g_hash_table(void *table, const xmlChar *key, void *value, void *userdata);

/**
 * Prints a Nix representation of all members in the table.
 *
 * @param file File descriptor to write to
 * @param value A GHashTable instance
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_g_hash_table_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

/**
 * Prints a Nix representation of an attribute set from a GHashTable.
 *
 * @param file File descriptor to write to
 * @param hash_table Pointer to an GHashTable
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_g_hash_table_nix(FILE *file, GHashTable *hash_table, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

/**
 * Prints a simple XML representation of all members in the table.
 *
 * @param file File descriptor to write to
 * @param value A GHashTable instance
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_g_hash_table_simple_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a simple XML representation of an attribute set from a GHashTable.
 *
 * @param file File descriptor to write to
 * @param hash_table An GHashTable instance
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_g_hash_table_simple_xml(FILE *file, GHashTable *hash_table, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a verbose XML representation of all members in the table.
 *
 * @param file File descriptor to write to
 * @param value A GHashTable instance
 * @param child_element_name Name of each attribute child element
 * @param name_property_name Name of the name property
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_g_hash_table_verbose_attributes_xml(FILE *file, const void *value, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints a verbose XML representation of an attribute set from an GHashTable.
 *
 * @param file File descriptor to write to
 * @param hash_table A GHashTable instance
 * @param child_element_name Name of each attribute child element
 * @param name_property_name Name of the name property
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_g_hash_table_verbose_xml(FILE *file, GHashTable *hash_table, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Parses a GHashTable from the sub elements of a provided XML element.
 *
 * @param element XML element to parse.
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param parse_object Pointer to a function that parses the value
 * @return A pointer to a GHashTable
 */
void *NixXML_parse_g_hash_table(xmlNodePtr element, void *userdata, NixXML_ParseObjectFunc parse_object);

#endif
