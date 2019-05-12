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

#include "dimensions.h"
#include <stdlib.h>
#include "nixxml-parse.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"

#define TRUE 1
#define FALSE 0

static void *create_dimensions(xmlNodePtr element, void *userdata)
{
    return calloc(1, sizeof(Dimensions));
}

static void insert_dimensions_attribute(void *table, const xmlChar *key, void *value, void *userdata)
{
    Dimensions *dimensions = (Dimensions*)table;

    if(xmlStrcmp(key, (xmlChar*) "width") == 0)
        dimensions->width = atoi(value);
    else if(xmlStrcmp(key, (xmlChar*) "height") == 0)
        dimensions->height = atoi(value);

    xmlFree(value);
}

void *parse_dimensions(xmlNodePtr element, void *userdata)
{
    return NixXML_parse_simple_attrset(element, userdata, create_dimensions, NixXML_parse_value, insert_dimensions_attribute);
}

void delete_dimensions(Dimensions *dimensions)
{
    if(dimensions != NULL)
        free(dimensions);
}

int check_dimensions(const Dimensions *dimensions)
{
    if(dimensions->width <= 0)
    {
        fprintf(stderr, "ERROR: dimensions width should be greater than 0!\n");
        return FALSE;
    }

    if(dimensions->height <= 0)
    {
        fprintf(stderr, "ERROR: dimensions height should be greater than 0!\n");
        return FALSE;
    }

    return TRUE;
}

static void print_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const Dimensions *dimensions = (const Dimensions*)value;
    NixXML_print_attribute_nix(file, "width", &dimensions->width, indent_level, userdata, NixXML_print_int_nix);
    NixXML_print_attribute_nix(file, "height", &dimensions->height, indent_level, userdata, NixXML_print_int_nix);
}

void print_dimensions_nix(FILE *file, const void *figure, const int indent_level, void *userdata)
{
    NixXML_print_attrset_nix(file, figure, indent_level, userdata, print_attributes_nix, NULL);
}

static void print_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_attribute_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    const Dimensions *dimensions = (const Dimensions*)value;
    NixXML_print_simple_attribute_xml(file, "width", &dimensions->width, indent_level, type_attribute_name, userdata, NixXML_print_int_xml);
    NixXML_print_simple_attribute_xml(file, "height", &dimensions->height, indent_level, type_attribute_name, userdata, NixXML_print_int_xml);
}

void print_dimensions_xml(FILE *file, const void *figure, const int indent_level, const char *type_attribute_name, void *userdata)
{
    NixXML_print_simple_attrset_xml(file, figure, indent_level, type_attribute_name, userdata, print_attributes_xml, NULL);
}

gdImagePtr allocate_image_with_dimensions(const Dimensions *dimensions)
{
    gdImagePtr image = gdImageCreate(dimensions->width, dimensions->height);
    gdImageColorAllocate(image, 0, 0, 0); /* Set background color to black */
    return image;
}
