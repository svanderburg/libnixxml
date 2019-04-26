#ifndef __NIXXML_PRINT_XML_H
#define __NIXXML_PRINT_XML_H
#include <stdio.h>
#include "nixxml-print.h"

typedef void (*NixXML_PrintXMLMembersFunc) (FILE *file, const char *child_element_name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);
typedef void (*NixXML_PrintVerboseXMLMembersFunc) (FILE *file, const void *value, const char *child_element_name, const char *name_property_name, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_open_root_element(FILE *file, const char *root_element_name);

void NixXML_print_close_root_element(FILE *file, const char *root_element_name);

void NixXML_print_string_xml(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_value_xml(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_int_xml(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_list_element_xml(FILE *file, const char *child_element_name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_list_xml(FILE *file, const void *list, const char *child_element_name, const int indent_level, void *userdata, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintValueFunc print_value);

void NixXML_print_simple_attribute_xml(FILE *file, const char *name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_verbose_attribute_xml(FILE *file, const char *child_element_name, const char *name_property_name, const char *name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_simple_attrset_xml(FILE *file, const void *table, const int indent_level, void *userdata, NixXML_PrintMembersFunc print_attributes, NixXML_PrintValueFunc print_value);

void NixXML_print_verbose_attrset_xml(FILE *file, const void *table, const char *child_element_name, const char *name_property_name, const int indent_level, void *userdata, NixXML_PrintVerboseXMLMembersFunc print_attributes, NixXML_PrintValueFunc print_value);

#endif
