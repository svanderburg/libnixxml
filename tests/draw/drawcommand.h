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

#ifndef __DRAWCOMMAND_H
#define __DRAWCOMMAND_H
#include <libxml/parser.h>
#include <gd.h>

typedef struct
{
    xmlChar *figure;

    int x;
    int y;
}
DrawCommand;

void *parse_draw_command(xmlNodePtr element, void *userdata);

void delete_draw_command(DrawCommand *drawCommand);

int check_draw_command(const DrawCommand *drawCommand, xmlHashTablePtr figures_table);

void print_draw_command_nix(FILE *file, const DrawCommand *drawCommand, const int indent_level, void *userdata);

void print_draw_command_xml(FILE *file, const DrawCommand *drawCommand, const int indent_level, const char *type_property_name, void *userdata);

void execute_draw_command(const DrawCommand *drawCommand, xmlHashTablePtr figures_table, gdImagePtr image);

#endif
