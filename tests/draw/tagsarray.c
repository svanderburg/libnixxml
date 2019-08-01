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

#include "tagsarray.h"
#include <nixxml-ptrarray.h>

void *parse_tags_array(xmlNodePtr element, void *userdata)
{
    return NixXML_parse_ptr_array(element, "elem", userdata, NixXML_parse_value);
}

void delete_tags_array(xmlChar **tags_array)
{
    NixXML_delete_values_array((void**)tags_array);
}

void print_tags_array_nix(FILE *file, const void **array, const int indent_level, void *userdata)
{
    NixXML_print_ptr_array_nix(file, array, indent_level, userdata, NixXML_print_string_nix);
}

void print_tags_array_xml(FILE *file, const void **array, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_ptr_array_xml(file, array, "elem", indent_level, type_property_name, userdata, NixXML_print_string_xml);
}
