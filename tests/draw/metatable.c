/*
 * Copyright (c) 2019-2022 Sander van der Burg
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

#include "metatable.h"
#include <nixxml-xmlhashtable.h>

void *parse_meta_table(xmlNodePtr element, void *userdata)
{
    return NixXML_parse_xml_hash_table_simple(element, userdata, NixXML_parse_value);
}

static void meta_deallocator(void *payload, const xmlChar *name)
{
    xmlFree(payload);
}

void delete_meta_table(xmlHashTablePtr meta_table)
{
    xmlHashFree(meta_table, meta_deallocator);
}

NixXML_bool compare_meta_tables(xmlHashTablePtr left, xmlHashTablePtr right)
{
    return NixXML_compare_xml_property_tables(left, right);
}

void print_meta_table_nix(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata)
{
    NixXML_print_xml_hash_table_nix(file, hash_table, indent_level, userdata, NixXML_print_string_nix);
}

void print_meta_table_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_xml_hash_table_simple_xml(file, hash_table, indent_level, type_property_name, userdata, NixXML_print_string_xml);
}
