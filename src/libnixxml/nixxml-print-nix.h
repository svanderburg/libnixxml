#ifndef __NIXXML_PRINT_NIX_H
#define __NIXXML_PRINT_NIX_H
#include <stdio.h>
#include "nixxml-print.h"

void NixXML_print_string_nix(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_value_nix(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_int_nix(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_list_element_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_list_nix(FILE *file, const void *list, const int indent_level, void *userdata, NixXML_PrintMembersFunc print_list_elements, NixXML_PrintValueFunc print_value);

void NixXML_print_attribute_nix(FILE *file, const char *name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_attrset_nix(FILE *file, const void *table, const int indent_level, void *userdata, NixXML_PrintMembersFunc print_attributes, NixXML_PrintValueFunc print_value);

#endif
