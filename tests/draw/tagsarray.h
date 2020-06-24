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

#ifndef __TAGSARRAY_H
#define __TAGSARRAY_H
#include <libxml/parser.h>
#include "nixxml-types.h"

void *parse_tags_array(xmlNodePtr element, void *userdata);

void delete_tags_array(xmlChar **tags_array);

NixXML_bool compare_tags_arrays(xmlChar **left, xmlChar **right);

void print_tags_array_nix(FILE *file, const xmlChar **tags_array, const int indent_level, void *userdata);

void print_tags_array_xml(FILE *file, const xmlChar **tags_array, const int indent_level, const char *type_property_name, void *userdata);

#endif
