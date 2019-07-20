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

#ifndef __NIXXML_PTRARRAY_H
#define __NIXXML_PTRARRAY_H
#include <libxml/parser.h>
#include "nixxml-parse.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"
#include "nixxml-generate-env.h"

typedef void (*NixXML_DeletePtrArrayElementFunc) (void *element);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new pointer array by parsing the sub elements of a provided
 * XML element.
 *
 * @param element XML element to parse.
 * @param userdata Arbitrary user data that is propagated to all parse functions
 * @return A data structure that encapsulates a pointer array. It must be finalized to become a usable pointer array.
 */
void *NixXML_create_ptr_array(xmlNodePtr element, void *userdata);

/**
 * Adds a value to a non-finalized pointer array.
 *
 * @param list A non-finalized pointer array struct
 * @param value Pointer to a value to add
 * @param userdata Arbitrary user data that is propagated to all parse functions
 */
void NixXML_add_value_to_ptr_array(void *list, void *value, void *userdata);

/**
 * Finalizes the pointer array (by discarding obsolete resources) and makes it
 * usable to be consumed by the application.
 *
 * @param list A non-finalized pointer array struct
 * @param userdata Arbitrary user data that is propagated to all parse functions
 * @return A pointer array (void**)
 */
void *NixXML_finalize_ptr_array(void *list, void *userdata);

/**
 * Deletes a pointer array including its contents
 *
 * @param array Pointer array to delete
 * @param delete_element Pointer to a function that deletes each element
 */
void NixXML_delete_ptr_array(void **array, NixXML_DeletePtrArrayElementFunc delete_element);

/**
 * Deletes a pointer array and the values it refers to.by executing free()
 *
 * @param array Pointer array to delete
 */
void NixXML_delete_values_array(void **array);

/**
 * Prints a Nix representation of all elements in the array.
 *
 * @param file File descriptor to write to
 * @param value Pointer array
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_ptr_array_elements_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

/**
 * Prints a Nix representation of a list from a pointer array.
 *
 * @param file File descriptor to write to
 * @param array Pointer array
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_ptr_array_nix(FILE *file, const void **array, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

/**
 * Prints an XML representation of all elements in the array.
 *
 * @param file File descriptor to write to
 * @param child_element_name Name of each list child element
 * @param value Pointer array
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_ptr_array_elements_xml(FILE *file, const char *child_element_name, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Prints an XML representation of a list from a pointer array.
 *
 * @param file File descriptor to write to
 * @param array Pointer array
 * @param child_element_name Name of each list child element
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param print_value Pointer to a function that prints an element value
 */
void NixXML_print_ptr_array_xml(FILE *file, const void **array, const char *child_element_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value);

/**
 * Parses a pointer array from the sub elements of a provided XML element.
 *
 * @param element XML element to parse.
 * @param child_element_name Name of each list child element
 * @param userdata Arbitrary user data that gets propagated to all print functions
 * @param parse_object Pointer to a function that parses the value
 * @return A pointer array (void**)
 */
void *NixXML_parse_ptr_array(xmlNodePtr element, const char *child_element_name, void *userdata, NixXML_ParseObjectFunc parse_object);

/**
 * Generates a white space separated environment variable representation of a
 * pointer array.
 *
 * @param value A pointer array
 * @param userdata Arbitrary user data that is propagated to all generate functions
 * @param generate_value A pointer to a function that generates an environment variable representation of a provided value
 */
xmlChar *NixXML_generate_env_value_from_ptr_array(const void *value, void *userdata, NixXML_GenerateEnvValueFunc generate_value);

#ifdef __cplusplus
}
#endif

#endif
