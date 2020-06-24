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

#ifndef __DIMENSIONS_H
#define __DIMENSIONS_H
#include <libxml/parser.h>
#include <gd.h>
#include "nixxml-types.h"

typedef struct
{
    int width;
    int height;
}
Dimensions;

void *parse_dimensions(xmlNodePtr element, void *userdata);

void delete_dimensions(Dimensions *dimensions);

NixXML_bool check_dimensions(const Dimensions *dimensions);

NixXML_bool compare_dimensions(const Dimensions *left, const Dimensions *right);

void print_dimensions_nix(FILE *file, const Dimensions *dimensions, const int indent_level, void *userdata);

void print_dimensions_xml(FILE *file, const Dimensions *dimensions, const int indent_level, const char *type_property_name, void *userdata);

gdImagePtr allocate_image_with_dimensions(const Dimensions *dimensions);

#endif
