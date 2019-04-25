#ifndef __NIXXML_PARSE_H
#define __NIXXML_PARSE_H

#include <libxml/parser.h>

typedef void *(*NixXML_ParseObjectFunc) (xmlNodePtr element, void *userdata);
typedef void *(*NixXML_CreateObjectFunc) (xmlNodePtr element, void *userdata);
typedef void (*NixXML_AddElementFunc) (void *list, void *value, void *userdata);
typedef void (*NixXML_InsertObjectFunc) (void *table, const xmlChar *key, void *value, void *userdata);
typedef void *(*NixXML_FinalizeListFunc) (void *list, void *userdata);
typedef void (*NixXML_ParseAndInsertObjectFunc) (xmlNodePtr element, void *table, const xmlChar *key, void *userdata);

/**
 * Searches for a element property with a provided name.
 *
 * @param element XML element to parse.
 * @param property_name Name of the property to search for
 * @return The property value or NULL if the property cannot be found
 */
xmlChar *NixXML_find_property(xmlNodePtr element, const char *property_name);

/**
 * Parses a value, e.g. a string, float, int or bool.
 *
 * @param element XML element to parse.
 * @return A string representation of the value or NULL if it cannot be parsed. The caller needs to be free up the memory with xmlFree()
 */
void *NixXML_parse_value(xmlNodePtr element, void *userdata);

/**
 * Parses a list-like data structure.
 *
 * @param element XML element to parse.
 * @param child_element_name Name of the child XML elements in the list or NULL to accept any sub element as a list element.
 * @param add_element Function that adds an element to the list-like data structure
 * @param parse_object Function that parses the value of the list element
 * @param finalize_list Finalizes the list-like data structure by carrying out additional house keeping tasks
 * @return A list-like data structure
 */
void *NixXML_parse_list(xmlNodePtr element, const char *child_element_name, void *userdata, NixXML_CreateObjectFunc create_list, NixXML_AddElementFunc add_element, NixXML_ParseObjectFunc parse_object, NixXML_FinalizeListFunc finalize_list);

void *NixXML_parse_simple_heterogeneous_attrset(xmlNodePtr element, void *userdata, NixXML_CreateObjectFunc create_table, NixXML_ParseAndInsertObjectFunc parse_and_insert_object);

/**
 * Parses a table-like data structure using a simple XML notation in which every
 * sub element name represents a key and every content text the value.
 *
 * @param element XML element to parse.
 * @param create_table Function that constructs a table-like data structure
 * @param parse_object Function that parses the value of the list element
 * @return A table-like data structure
 */
void *NixXML_parse_simple_attrset(xmlNodePtr element, void *userdata, NixXML_CreateObjectFunc create_table, NixXML_ParseObjectFunc parse_object, NixXML_InsertObjectFunc insert_object);

void *NixXML_parse_verbose_heterogeneous_attrset(xmlNodePtr element, const char *child_element_name, const char *name_property_name, void *userdata, NixXML_CreateObjectFunc create_table, NixXML_ParseAndInsertObjectFunc parse_and_insert_object);

/**
 * Parses a table-like data structure using a verbose XML notation in which every
 * sub element is annotated with a name attribute and every contect text the
 * value.
 *
 * @param element XML element to parse.
 * @param child_element_name Name of the child XML elements in the attribute set or NULL to accept any sub element as an entry.
 * @param name_property_name Name of the attribute that refers to the name of attribute
 * @param create_table Function that constructs a table-like data structure
 * @param parse_object Function that parses the value of the list element
 * @param insert_object Function that inserts an object into the table-like data structure
 * @return A table-like data structure
 */
void *NixXML_parse_verbose_attrset(xmlNodePtr element, const char *child_element_name, const char *name_property_name, void *userdata, NixXML_CreateObjectFunc create_table, NixXML_ParseObjectFunc parse_object, NixXML_InsertObjectFunc insert_object);

#endif
