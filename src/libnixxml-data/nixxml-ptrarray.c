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

#include "nixxml-ptrarray.h"
#include <stdlib.h>

typedef struct
{
    unsigned int count;
    void **elements;
}
PtrArray;

void *NixXML_create_ptr_array(xmlNodePtr element, void *userdata)
{
    unsigned int numOfElements = xmlChildElementCount(element);
    PtrArray *ret = (PtrArray*)malloc(sizeof(PtrArray));
    ret->count = 0;

    ret->elements = (void**)malloc((numOfElements + 1) * sizeof(void*));
    ret->elements[numOfElements] = NULL;

    return ret;
}

void NixXML_add_value_to_ptr_array(void *list, void *value, void *userdata)
{
    PtrArray *array = (PtrArray*)list;
    array->elements[array->count] = value;
    array->count++;
}

void *NixXML_finalize_ptr_array(void *list, void *userdata)
{
    PtrArray *array = (PtrArray*)list;
    void **elements = array->elements;
    free(array);
    return elements;
}

void NixXML_delete_ptr_array(void **array, NixXML_DeletePtrArrayElementFunc delete_element)
{
    if(array != NULL)
    {
        unsigned int i = 0;

        while(array[i] != NULL)
        {
            delete_element(array[i]);
            i++;
        }

        free(array);
    }
}

void NixXML_delete_values_array(void **array)
{
    NixXML_delete_ptr_array(array, free);
}

void NixXML_print_ptr_array_elements_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const void **array = (const void**)value;
    unsigned int i = 0;

    while(array[i] != NULL)
    {
        NixXML_print_list_element_nix(file, array[i], indent_level, userdata, print_value);
        i++;
    }
}

void NixXML_print_ptr_array_nix(FILE *file, const void **array, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    NixXML_print_list_nix(file, array, indent_level, userdata, NixXML_print_ptr_array_elements_nix, print_value);
}

void NixXML_print_ptr_array_elements_xml(FILE *file, const char *child_element_name, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    const void **array = (const void**)value;
    unsigned int i = 0;

    while(array[i] != NULL)
    {
        NixXML_print_list_element_xml(file, child_element_name, array[i], indent_level, type_property_name, userdata, print_value);
        i++;
    }
}

void NixXML_print_ptr_array_xml(FILE *file, const void **array, const char *child_element_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    NixXML_print_list_xml(file, array, child_element_name, indent_level, type_property_name, userdata, NixXML_print_ptr_array_elements_xml, print_value);
}

void *NixXML_parse_ptr_array(xmlNodePtr element, const char *child_element_name, void *userdata, NixXML_ParseObjectFunc parse_object)
{
    return NixXML_parse_list(element, child_element_name, userdata, NixXML_create_ptr_array, NixXML_add_value_to_ptr_array, parse_object, NixXML_finalize_ptr_array);
}

xmlChar *NixXML_generate_env_value_from_ptr_array(const void *value, void *userdata, NixXML_GenerateEnvValueFunc generate_value)
{
    const void **array = (const void**)value;
    unsigned int i = 0;
    xmlChar *result = NULL;

    while(array[i] != NULL)
    {
        xmlChar *env = generate_value(array[i], userdata);
        result = NixXML_append_value_to_list_env_value(result, env);
        xmlFree(env);
        i++;
    }

    return result;
}
