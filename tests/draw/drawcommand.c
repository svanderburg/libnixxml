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

#include "drawcommand.h"
#include <stdlib.h>
#include "nixxml-parse.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"
#include "figure.h"

#define TRUE 1
#define FALSE 0

static void *create_draw_command(xmlNodePtr element, void *userdata)
{
    return calloc(1, sizeof(DrawCommand));
}

static void insert_draw_command_attribute(void *table, const xmlChar *key, void *value, void *userdata)
{
    DrawCommand *drawCommand = (DrawCommand*)table;

    if(xmlStrcmp(key, (xmlChar*) "figure") == 0)
        drawCommand->figure = (xmlChar*)value;
    else if(xmlStrcmp(key, (xmlChar*) "x") == 0)
    {
        drawCommand->x = atoi((char*)value);
        xmlFree(value);
    }
    else if(xmlStrcmp(key, (xmlChar*) "y") == 0)
    {
        drawCommand->y = atoi((char*)value);
        xmlFree(value);
    }
    else
        xmlFree(value);
}

void *parse_draw_command(xmlNodePtr element, void *userdata)
{
    return NixXML_parse_simple_attrset(element, userdata, create_draw_command, NixXML_parse_value, insert_draw_command_attribute);
}

void delete_draw_command(DrawCommand *drawCommand)
{
    if(drawCommand != NULL)
    {
        xmlFree(drawCommand->figure);
        free(drawCommand);
    }
}

int check_draw_command(const DrawCommand *drawCommand, xmlHashTablePtr figures_table)
{
    int status = TRUE;

    if(drawCommand->x < 0)
    {
        fprintf(stderr, "The draw command x coordinator should be 0 or greater\n");
        status = FALSE;
    }

    if(drawCommand->y < 0)
    {
        fprintf(stderr, "The draw command y coordinator should be 0 or greater\n");
        status = FALSE;
    }

    if(xmlHashLookup(figures_table, drawCommand->figure) == NULL)
    {
        fprintf(stderr, "Dangling reference from draw command to figure: %s\n", drawCommand->figure);
        status = FALSE;
    }

    return status;
}

int compare_draw_commands(const DrawCommand *left, const DrawCommand *right)
{
    return((xmlStrcmp(left->figure, right->figure) == 0)
      && (left->x == right->x)
      && (left->y == right->y));
}

/* Nix print functionality */

static void print_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const DrawCommand *drawCommand = (const DrawCommand*)value;
    NixXML_print_attribute_nix(file, "figure", drawCommand->figure, indent_level, userdata, NixXML_print_string_nix);
    NixXML_print_attribute_nix(file, "x", &drawCommand->x, indent_level, userdata, NixXML_print_int_nix);
    NixXML_print_attribute_nix(file, "y", &drawCommand->y, indent_level, userdata, NixXML_print_int_nix);
}

void print_draw_command_nix(FILE *file, const DrawCommand *drawCommand, const int indent_level, void *userdata)
{
    NixXML_print_attrset_nix(file, drawCommand, indent_level, userdata, print_attributes_nix, NULL);
}

/* XML print functionality */

static void print_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    const DrawCommand *drawCommand = (const DrawCommand*)value;
    NixXML_print_simple_attribute_xml(file, "figure", drawCommand->figure, indent_level, type_property_name, userdata, NixXML_print_string_xml);
    NixXML_print_simple_attribute_xml(file, "x", &drawCommand->x, indent_level, type_property_name, userdata, NixXML_print_int_xml);
    NixXML_print_simple_attribute_xml(file, "y", &drawCommand->y, indent_level, type_property_name, userdata, NixXML_print_int_xml);
}

void print_draw_command_xml(FILE *file, const DrawCommand *drawCommand, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_simple_attrset_xml(file, drawCommand, indent_level, type_property_name, userdata, print_attributes_xml, NULL);
}

void execute_draw_command(const DrawCommand *drawCommand, xmlHashTablePtr figures_table, gdImagePtr image)
{
    Figure *figure = (Figure*)xmlHashLookup(figures_table, drawCommand->figure);
    draw_figure(figure, image, drawCommand->x, drawCommand->y);
}
