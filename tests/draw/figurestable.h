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

#ifndef __FIGURESTABLE_H
#define __FIGURESTABLE_H
#include "figure.h"
#include <libxml/parser.h>
#include "nixxml-types.h"

void *parse_figures_table(xmlNodePtr element, void *userdata);

void delete_figures_table(xmlHashTablePtr figures_table);

NixXML_bool check_figures_table(xmlHashTablePtr figures_table);

NixXML_bool compare_figures_tables(xmlHashTablePtr left, xmlHashTablePtr right);

void print_figures_table_nix(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata);

void print_figures_table_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, const char *type_property_name, void *userdata);

#endif
