#ifndef __NIXXML_XMLHASHTABLE_H
#define __NIXXML_XMLHASHTABLE_H
#include <libxml/parser.h>
#include "nixxml-parse.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"

void *NixXML_create_xml_hash_table(xmlNodePtr element, void *userdata);

void NixXML_insert_into_xml_hash_table(void *table, const xmlChar *key, void *value, void *userdata);

void NixXML_print_xml_hash_table_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_xml_hash_table_nix(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_xml_hash_table_simple_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

void NixXML_print_xml_hash_table_simple_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

void NixXML_print_xml_hash_table_verbose_attributes_xml(FILE *file, const void *value, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

void NixXML_print_xml_hash_table_verbose_xml(FILE *file, xmlHashTablePtr hash_table, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

void *NixXML_parse_xml_hash_table(xmlNodePtr element, void *userdata, NixXML_ParseObjectFunc parse_object);

#endif
