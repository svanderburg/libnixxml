#ifndef __NIXXML_PTRARRAY_H
#define __NIXXML_PTRARRAY_H
#include <libxml/parser.h>
#include "nixxml-parse.h"
#include "nixxml-print.h"

typedef void (*NixXML_DeletePtrArrayElementFunc) (void *element);

void *NixXML_create_ptr_array(xmlNodePtr element, void *userdata);

void NixXML_add_value_to_ptr_array(void *list, void *value, void *userdata);

void *NixXML_finalize_ptr_array(void *list, void *userdata);

void NixXML_delete_ptr_array(void **array, NixXML_DeletePtrArrayElementFunc delete_element);

void NixXML_delete_values_array(void **array);

void NixXML_print_ptr_array_elements_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_ptr_array_nix(FILE *file, const void **array, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_ptr_array_elements_xml(FILE *file, const char *child_element_name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_ptr_array_xml(FILE *file, const void **array, const char *child_element_name, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void *NixXML_parse_ptr_array(xmlNodePtr element, const char *child_element_name, void *userdata, NixXML_ParseObjectFunc parse_object);

#endif
