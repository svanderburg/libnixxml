#include "nixxml-ptrarray.h"
#include <stdlib.h>
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"

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

void NixXML_print_ptr_array_elements_xml(FILE *file, const char *child_element_name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const void **array = (const void**)value;
    unsigned int i = 0;

    while(array[i] != NULL)
    {
        NixXML_print_list_element_xml(file, child_element_name, array[i], indent_level, userdata, print_value);
        i++;
    }
}

void NixXML_print_ptr_array_xml(FILE *file, const void **array, const char *child_element_name, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    NixXML_print_list_xml(file, array, child_element_name, indent_level, userdata, NixXML_print_ptr_array_elements_xml, print_value);
}

void *NixXML_parse_ptr_array(xmlNodePtr element, const char *child_element_name, void *userdata, NixXML_ParseObjectFunc parse_object)
{
    return NixXML_parse_list(element, child_element_name, userdata, NixXML_create_ptr_array, NixXML_add_value_to_ptr_array, parse_object, NixXML_finalize_ptr_array);
}
