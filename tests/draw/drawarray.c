/*
 * Copyright (c) 2019-2020 Sander van der Burg
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

#include "drawarray.h"
#include <nixxml-ptrarray.h>

#define TRUE 1
#define FALSE 0

void *parse_draw_array(xmlNodePtr element, void *userdata)
{
    return NixXML_parse_ptr_array(element, "elem", userdata, parse_draw_command);
}

void delete_draw_array(DrawCommand **draw_array)
{
    NixXML_delete_ptr_array((void**)draw_array, (NixXML_DeletePtrArrayElementFunc)delete_draw_command);
}

NixXML_bool check_draw_array(DrawCommand **draw_array, xmlHashTablePtr figures_table)
{
    unsigned int i = 0;

    while(draw_array[i] != NULL)
    {
        if(!check_draw_command(draw_array[i], figures_table))
            return FALSE;

        i++;
    }

    return TRUE;
}

NixXML_bool compare_draw_arrays(DrawCommand **left, DrawCommand **right)
{
    return NixXML_compare_ptr_arrays((const void **)left, (const void **)right, (NixXML_ComparePtrArrayElementFunc)compare_draw_commands);
}

void print_draw_array_nix(FILE *file, const DrawCommand **draw_array, const int indent_level, void *userdata)
{
    NixXML_print_ptr_array_nix(file, (const void **)draw_array, indent_level, userdata, (NixXML_PrintValueFunc)print_draw_command_nix);
}

void print_draw_array_xml(FILE *file, const DrawCommand **draw_array, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_ptr_array_xml(file, (const void **)draw_array, "elem", indent_level, type_property_name, userdata, (NixXML_PrintXMLValueFunc)print_draw_command_xml);
}
