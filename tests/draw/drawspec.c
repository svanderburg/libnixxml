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

#include "drawspec.h"
#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include "nixxml-parse.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"
#include "nixxml-ptrarray.h"
#include "nixxml-xmlhashtable.h"
#include "figure.h"

#define TRUE 1
#define FALSE 0

static void *create_drawspec(xmlNodePtr element, void *userdata)
{
    return calloc(1, sizeof(DrawSpec));
}

static void parse_and_insert_drawspec_attribute(xmlNodePtr element, void *table, const xmlChar *key, void *userdata)
{
    DrawSpec *drawSpec = (DrawSpec*)table;

    if(xmlStrcmp(element->name, (xmlChar*) "dimensions") == 0)
        drawSpec->dimensions = parse_dimensions(element, userdata);
    else if(xmlStrcmp(element->name, (xmlChar*) "figures") == 0)
        drawSpec->figures = NixXML_parse_xml_hash_table(element, userdata, parse_figure);
    else if(xmlStrcmp(key, (xmlChar*) "draw") == 0)
        drawSpec->draw = NixXML_parse_ptr_array(element, "elem", userdata, parse_draw_command);
    else if(xmlStrcmp(key, (xmlChar*) "meta") == 0)
        drawSpec->meta = NixXML_parse_xml_hash_table(element, userdata, NixXML_parse_value);
    else if(xmlStrcmp(key, (xmlChar*) "tags") == 0)
        drawSpec->tags = NixXML_parse_ptr_array(element, "elem", userdata, NixXML_parse_value);
}

static void *parse_drawspec(xmlNodePtr element, void *userdata)
{
    return NixXML_parse_simple_heterogeneous_attrset(element, userdata, create_drawspec, parse_and_insert_drawspec_attribute);
}

DrawSpec *open_drawspec(const char *filename)
{
    xmlDocPtr doc;
    xmlNodePtr node_root;
    DrawSpec *drawSpec;

    /* Parse the XML document */

    if((doc = xmlParseFile(filename)) == NULL)
    {
        fprintf(stderr, "Error with parsing the XML file!\n");
        xmlCleanupCharEncodingHandlers();
        xmlCleanupParser();
        return NULL;
    }

    /* Retrieve root element */
    node_root = xmlDocGetRootElement(doc);

    if(node_root == NULL)
    {
        fprintf(stderr, "The XML file is empty!\n");
        xmlFreeDoc(doc);
        xmlCleanupCharEncodingHandlers();
        xmlCleanupParser();
        return NULL;
    }

    /* Parse manifest */
    drawSpec = parse_drawspec(node_root, NULL);

    /* Cleanup */
    xmlFreeDoc(doc);
    xmlCleanupCharEncodingHandlers();
    xmlCleanupParser();

    /* Return draw specification */
    return drawSpec;
}

static void figure_deallocator(void *payload, const xmlChar *name)
{
    delete_figure(payload);
}

static void meta_deallocator(void *payload, const xmlChar *name)
{
    xmlFree(payload);
}

void delete_drawspec(DrawSpec *drawSpec)
{
    if(drawSpec != NULL)
    {
        delete_dimensions(drawSpec->dimensions);
        xmlHashFree(drawSpec->figures, figure_deallocator);
        NixXML_delete_ptr_array((void**)drawSpec->draw, (NixXML_DeletePtrArrayElementFunc)delete_draw_command);
        xmlHashFree(drawSpec->meta, meta_deallocator);
        NixXML_delete_values_array((void**)drawSpec->tags);

        free(drawSpec);
    }
}

/* Check functionality */

static int check_draw_commands(DrawCommand **draw, xmlHashTablePtr figures_table)
{
    unsigned int i = 0;

    while(draw[i] != NULL)
    {
        if(!check_draw_command(draw[i], figures_table))
            return FALSE;

        i++;
    }

    return TRUE;
}

static void scanner_check_figure(void *payload, void *data, const xmlChar *name)
{
    int *status = (int*)data;

    if(!check_figure(payload))
        *status = FALSE;
}

static int check_figures(xmlHashTablePtr figures_table)
{
    int status = TRUE;
    xmlHashScan(figures_table, scanner_check_figure, &status);
    return status;
}

int check_drawspec(const DrawSpec *drawSpec)
{
    if(drawSpec->dimensions == NULL)
    {
        fprintf(stderr, "No dimensions specified!\n");
        return FALSE;
    }

    if(!check_dimensions(drawSpec->dimensions))
        return FALSE;

    if(drawSpec->figures != NULL && !check_figures(drawSpec->figures))
        return FALSE;

    if(!check_draw_commands(drawSpec->draw, drawSpec->figures))
        return FALSE;

    return TRUE;
}

/* Print Nix functionality */

static void print_figure_table_nix(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_print_xml_hash_table_nix(file, (xmlHashTablePtr)value, indent_level, userdata, print_figure_nix);
}

static void print_draw_list_nix(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_print_ptr_array_nix(file, (const void **)value, indent_level, userdata, print_draw_command_nix);
}

static void print_meta_table_nix(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_print_xml_hash_table_nix(file, (xmlHashTablePtr)value, indent_level, userdata, NixXML_print_string_nix);
}

static void print_tags_list_nix(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_print_ptr_array_nix(file, (const void **)value, indent_level, userdata, NixXML_print_string_nix);
}

static void print_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    DrawSpec *drawSpec = (DrawSpec*)value;
    NixXML_print_attribute_nix(file, "dimensions", drawSpec->dimensions, indent_level, userdata, print_dimensions_nix);
    NixXML_print_attribute_nix(file, "figures", drawSpec->figures, indent_level, userdata, print_figure_table_nix);
    NixXML_print_attribute_nix(file, "draw", drawSpec->draw, indent_level, userdata, print_draw_list_nix);
    NixXML_print_attribute_nix(file, "meta", drawSpec->meta, indent_level, userdata, print_meta_table_nix);
    NixXML_print_attribute_nix(file, "tags", drawSpec->tags, indent_level, userdata, print_tags_list_nix);
}

void print_drawspec_nix(FILE *file, const DrawSpec *drawSpec)
{
    NixXML_print_attrset_nix(file, drawSpec, 0, NULL, print_attributes_nix, NULL);
}

/* Print XML functionality */

static void print_figure_table_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_xml_hash_table_simple_xml(file, (xmlHashTablePtr)value, indent_level, type_property_name, userdata, print_figure_xml);
}

static void print_draw_list_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_ptr_array_xml(file, (const void **)value, "elem", indent_level, type_property_name, userdata, print_draw_command_xml);
}

static void print_meta_table_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_xml_hash_table_simple_xml(file, (xmlHashTablePtr)value, indent_level, type_property_name, userdata, NixXML_print_string_xml);
}

static void print_tags_list_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_ptr_array_xml(file, (const void **)value, "elem", indent_level, type_property_name, userdata, NixXML_print_string_xml);
}

static void print_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    DrawSpec *drawSpec = (DrawSpec*)value;
    NixXML_print_simple_attribute_xml(file, "dimensions", drawSpec->dimensions, indent_level, type_property_name, userdata, print_dimensions_xml);
    NixXML_print_simple_attribute_xml(file, "figures", drawSpec->figures, indent_level, type_property_name, userdata, print_figure_table_xml);
    NixXML_print_simple_attribute_xml(file, "draw", drawSpec->draw, indent_level, type_property_name, userdata, print_draw_list_xml);
    NixXML_print_simple_attribute_xml(file, "meta", drawSpec->meta, indent_level, type_property_name, userdata, print_meta_table_xml);
    NixXML_print_simple_attribute_xml(file, "tags", drawSpec->tags, indent_level, type_property_name, userdata, print_tags_list_xml);
}

void print_drawspec_xml(FILE *file, const DrawSpec *drawSpec)
{
    NixXML_print_open_root_tag(file, "drawSpec");
    NixXML_print_simple_attrset_xml(file, drawSpec, 0, NULL, NULL, print_attributes_xml, NULL);
    NixXML_print_close_root_tag(file, "drawSpec");
}

/* Draw functionality */

void draw_image_from_drawspec(FILE *file, const DrawSpec *drawSpec)
{
    gdImagePtr image = allocate_image_with_dimensions(drawSpec->dimensions);

    /* Execute draw commands */
    unsigned int i = 0;

    while(drawSpec->draw[i] != NULL)
    {
        execute_draw_command(drawSpec->draw[i], drawSpec->figures, image);
        i++;
    }

    /* Write PNG image and destroy */
    gdImagePng(image, file);

    gdImageDestroy(image);
}
