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

#ifndef __DRAWARRAY_H
#define __DRAWARRAY_H
#include <libxml/parser.h>
#include "drawcommand.h"

void *parse_draw_array(xmlNodePtr element, void *userdata);

void delete_draw_array(DrawCommand **draw_array);

int check_draw_array(DrawCommand **draw_array, xmlHashTablePtr figures_table);

int compare_draw_arrays(DrawCommand **left, DrawCommand **right);

void print_draw_array_nix(FILE *file, const DrawCommand **draw_array, const int indent_level, void *userdata);

void print_draw_array_xml(FILE *file, const DrawCommand **draw_array, const int indent_level, const char *type_property_name, void *userdata);

#endif
