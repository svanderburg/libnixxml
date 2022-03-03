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

#include "drawspec.h"
#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include "nixxml-parse.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"
#include "nixxml-ptrarray.h"
#include "nixxml-xmlhashtable.h"
#include "figurestable.h"
#include "drawarray.h"
#include "metatable.h"
#include "tagsarray.h"

#define TRUE 1
#define FALSE 0

static void *create_drawspec(xmlNodePtr element, void *userdata)
{
    return calloc(1, sizeof(DrawSpec));
}

static void parse_and_insert_drawspec_attributes(xmlNodePtr element, void *table, const xmlChar *key, void *userdata)
{
    DrawSpec *drawSpec = (DrawSpec*)table;

    if(xmlStrcmp(element->name, (xmlChar*) "dimensions") == 0)
        drawSpec->dimensions = (Dimensions*)parse_dimensions(element, userdata);
    else if(xmlStrcmp(element->name, (xmlChar*) "figures") == 0)
        drawSpec->figures_table = (xmlHashTablePtr)parse_figures_table(element, userdata);
    else if(xmlStrcmp(key, (xmlChar*) "draw") == 0)
        drawSpec->draw_array = (DrawCommand**)parse_draw_array(element, userdata);
    else if(xmlStrcmp(key, (xmlChar*) "meta") == 0)
        drawSpec->meta_table = (xmlHashTablePtr)parse_meta_table(element, userdata);
    else if(xmlStrcmp(key, (xmlChar*) "tags") == 0)
        drawSpec->tags_array = (xmlChar**)parse_tags_array(element, userdata);
}

static void *parse_drawspec(xmlNodePtr element, void *userdata)
{
    DrawSpec *drawSpec = (DrawSpec*)NixXML_parse_simple_heterogeneous_attrset(element, userdata, create_drawspec, parse_and_insert_drawspec_attributes);

    /* Set default values */
    if(drawSpec != NULL)
    {
        if(drawSpec->figures_table == NULL)
            drawSpec->figures_table = xmlHashCreate(0);
        if(drawSpec->draw_array == NULL)
            drawSpec->draw_array = (DrawCommand**)NixXML_create_ptr_array(0);
        if(drawSpec->meta_table == NULL)
            drawSpec->meta_table = xmlHashCreate(0);
        if(drawSpec->tags_array == NULL)
            drawSpec->tags_array = (xmlChar**)NixXML_create_ptr_array(0);
    }

    return drawSpec;
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
    drawSpec = (DrawSpec*)parse_drawspec(node_root, NULL);

    /* Cleanup */
    xmlFreeDoc(doc);
    xmlCleanupCharEncodingHandlers();
    xmlCleanupParser();

    /* Return draw specification */
    return drawSpec;
}

void delete_drawspec(DrawSpec *drawSpec)
{
    if(drawSpec != NULL)
    {
        delete_dimensions(drawSpec->dimensions);
        delete_figures_table(drawSpec->figures_table);
        delete_draw_array(drawSpec->draw_array);
        delete_meta_table(drawSpec->meta_table);
        delete_tags_array(drawSpec->tags_array);
        free(drawSpec);
    }
}

/* Check functionality */

NixXML_bool check_drawspec(const DrawSpec *drawSpec)
{
    NixXML_bool status = TRUE;

    if(drawSpec->dimensions == NULL)
    {
        fprintf(stderr, "No dimensions specified!\n");
        status = FALSE;
    }

    if(!check_dimensions(drawSpec->dimensions))
        status = FALSE;

    if(!check_figures_table(drawSpec->figures_table))
        status = FALSE;

    if(!check_draw_array(drawSpec->draw_array, drawSpec->figures_table))
        status = FALSE;

    return status;
}

/* Compare functionality */

NixXML_bool compare_drawspecs(const DrawSpec *left, const DrawSpec *right)
{
    return(compare_dimensions(left->dimensions, right->dimensions)
      && compare_figures_tables(left->figures_table, right->figures_table)
      && compare_draw_arrays(left->draw_array, right->draw_array)
      && compare_meta_tables(left->meta_table, right->meta_table)
      && compare_tags_arrays(left->tags_array, right->tags_array));
}

/* Print Nix functionality */

static void print_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    DrawSpec *drawSpec = (DrawSpec*)value;
    NixXML_print_attribute_nix(file, "dimensions", drawSpec->dimensions, indent_level, userdata, (NixXML_PrintValueFunc)print_dimensions_nix);
    NixXML_print_attribute_nix(file, "figures", drawSpec->figures_table, indent_level, userdata, (NixXML_PrintValueFunc)print_figures_table_nix);
    NixXML_print_attribute_nix(file, "draw", drawSpec->draw_array, indent_level, userdata, (NixXML_PrintValueFunc)print_draw_array_nix);
    NixXML_print_attribute_nix(file, "meta", drawSpec->meta_table, indent_level, userdata, (NixXML_PrintValueFunc)print_meta_table_nix);
    NixXML_print_attribute_nix(file, "tags", drawSpec->tags_array, indent_level, userdata, (NixXML_PrintValueFunc)print_tags_array_nix);
}

void print_drawspec_nix(FILE *file, const DrawSpec *drawSpec, const int indent_level, void *userdata)
{
    NixXML_print_attrset_nix(file, drawSpec, indent_level, userdata, print_attributes_nix, NULL);
}

/* Print XML functionality */

static void print_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    DrawSpec *drawSpec = (DrawSpec*)value;
    NixXML_print_simple_attribute_xml(file, "dimensions", drawSpec->dimensions, indent_level, type_property_name, userdata, (NixXML_PrintXMLValueFunc)print_dimensions_xml);
    NixXML_print_simple_attribute_xml(file, "figures", drawSpec->figures_table, indent_level, type_property_name, userdata, (NixXML_PrintXMLValueFunc)print_figures_table_xml);
    NixXML_print_simple_attribute_xml(file, "draw", drawSpec->draw_array, indent_level, type_property_name, userdata, (NixXML_PrintXMLValueFunc)print_draw_array_xml);
    NixXML_print_simple_attribute_xml(file, "meta", drawSpec->meta_table, indent_level, type_property_name, userdata, (NixXML_PrintXMLValueFunc)print_meta_table_xml);
    NixXML_print_simple_attribute_xml(file, "tags", drawSpec->tags_array, indent_level, type_property_name, userdata, (NixXML_PrintXMLValueFunc)print_tags_array_xml);
}

void print_drawspec_xml(FILE *file, const DrawSpec *drawSpec, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_open_root_tag(file, "drawSpec");
    NixXML_print_simple_attrset_xml(file, drawSpec, indent_level, type_property_name, userdata, print_attributes_xml, NULL);
    NixXML_print_close_root_tag(file, "drawSpec");
}

/* Draw functionality */

void draw_image_from_drawspec(FILE *file, const DrawSpec *drawSpec)
{
    gdImagePtr image = allocate_image_with_dimensions(drawSpec->dimensions);

    /* Execute draw commands */
    unsigned int i = 0;

    while(drawSpec->draw_array[i] != NULL)
    {
        execute_draw_command(drawSpec->draw_array[i], drawSpec->figures_table, image);
        i++;
    }

    /* Write PNG image and destroy */
    gdImagePng(image, file);

    gdImageDestroy(image);
}
