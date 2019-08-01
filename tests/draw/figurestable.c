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

#include "figurestable.h"
#include <nixxml-xmlhashtable.h>

#define TRUE 1
#define FALSE 0

void *parse_figures_table(xmlNodePtr element, void *userdata)
{
    return NixXML_parse_xml_hash_table_simple(element, userdata, parse_figure);
}

static void figure_deallocator(void *payload, const xmlChar *name)
{
    delete_figure((Figure*)payload);
}

void delete_figures_table(xmlHashTablePtr figures_table)
{
    xmlHashFree(figures_table, figure_deallocator);
}

static void scanner_check_figure(void *payload, void *data, const xmlChar *name)
{
    int *status = (int*)data;

    if(!check_figure((Figure*)payload))
        *status = FALSE;
}

int check_figures_table(xmlHashTablePtr figures_table)
{
    int status = TRUE;
    xmlHashScan(figures_table, scanner_check_figure, &status);
    return status;
}

void print_figures_table_nix(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata)
{
    NixXML_print_xml_hash_table_nix(file, hash_table, indent_level, userdata, (NixXML_PrintValueFunc)print_figure_nix);
}

void print_figures_table_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_xml_hash_table_simple_xml(file, hash_table, indent_level, type_property_name, userdata, (NixXML_PrintXMLValueFunc)print_figure_xml);
}
