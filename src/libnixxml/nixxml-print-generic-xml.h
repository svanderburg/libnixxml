#ifndef __NIXXML_PRINT_GENERIC_XML_H
#define __NIXXML_PRINT_GENERIC_XML_H

#include <stdio.h>
#include "nixxml-object.h"
#include "nixxml-print-xml.h"

void NixXML_print_expr_xml(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_generic_expr_xml(FILE *file, const NixXML_Object *value, const int indent_level, const char *root_element_name, const char *child_element_name, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintMembersFunc print_attributes);

#endif
