#ifndef __NIXXML_PARSE_GENERIC_H
#define __NIXXML_PARSE_GENERIC_H

#include "nixxml-parse.h"
#include "nixxml-node.h"

typedef struct
{
    const char *type_property_name;
    const char *name_property_name;
    NixXML_CreateObjectFunc create_list;
    NixXML_CreateObjectFunc create_table;
    NixXML_AddElementFunc add_element;
    NixXML_InsertObjectFunc insert_object;
    NixXML_FinalizeListFunc finalize_list;
}
NixXML_ParseExprParams;

void *NixXML_parse_generic_string(xmlNodePtr element, void *userdata);

void *NixXML_parse_generic_int(xmlNodePtr element, void *userdata);

void *NixXML_parse_generic_float(xmlNodePtr element, void *userdata);

void *NixXML_parse_generic_bool(xmlNodePtr element, void *userdata);

void *NixXML_create_generic_list(xmlNodePtr element, void *userdata);

void *NixXML_create_generic_attrset(xmlNodePtr element, void *userdata);

void NixXML_add_element_to_generic_list(void *list, void *value, void *userdata);

void NixXML_insert_object_into_generic_attrset(void *table, const xmlChar *key, void *value, void *userdata);

void *NixXML_finalize_generic_list(void *list, void *userdata);

void *NixXML_parse_expr(xmlNodePtr element, void *userdata);

/**
 * Recursively parses a type-annotated XML document.
 *
 * @param element Root element of the XML document
 * @param type_property_name Name of the attribute that contains the type annotation
 * @param name_property_name Name of the attribute that refers to the name of attribute
 * @param create_list Function that constructs a list-like data structure
 * @param create_table Function that constructs a table-like data structure
 * @param add_element Function that adds an element to the list-like data structure
 * @param insert_object Function that inserts an object into the table-like data structure
 * @param finalize_list Finalizes the list-like data structure by carrying out additional house keeping tasks
 * @return A list-like or table-like data structure, string or NULL if the data cannot be parsed.
 */
NixXML_Node *NixXML_generic_parse_expr(xmlNodePtr element, const char *type_property_name, const char *name_property_name, NixXML_CreateObjectFunc create_list, NixXML_CreateObjectFunc create_table, NixXML_AddElementFunc add_element, NixXML_InsertObjectFunc insert_object, NixXML_FinalizeListFunc finalize_list);

#endif
