#ifndef __NIXXML_PRINT_GENERIC_XML_H
#define __NIXXML_PRINT_GENERIC_XML_H

#include <stdio.h>
#include "nixxml-node.h"
#include "nixxml-print-xml.h"

void NixXML_print_expr_simple_xml(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_generic_expr_simple_xml(FILE *file, const NixXML_Node *value, const int indent_level, const char *root_element_name, const char *child_element_name, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintMembersFunc print_attributes);

void NixXML_print_expr_verbose_xml(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_generic_expr_verbose_xml(FILE *file, const NixXML_Node *value, const int indent_level, const char *root_element_name, const char *list_element_name, const char *attr_element_name, const char *name_property_name, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintVerboseXMLMembersFunc print_attributes);

#endif
