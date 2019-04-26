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

static void print_attributes_xml(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const Dimensions *dimensions = (const Dimensions*)value;
    NixXML_print_simple_attribute_xml(file, "width", &dimensions->width, indent_level, userdata, NixXML_print_int_xml);
    NixXML_print_simple_attribute_xml(file, "height", &dimensions->height, indent_level, userdata, NixXML_print_int_xml);
}

void print_dimensions_xml(FILE *file, const void *figure, const int indent_level, void *userdata)
{
    NixXML_print_simple_attrset_xml(file, figure, indent_level, userdata, print_attributes_xml, NULL);
}

gdImagePtr allocate_image_with_dimensions(const Dimensions *dimensions)
{
    gdImagePtr image = gdImageCreate(dimensions->width, dimensions->height);
    gdImageColorAllocate(image, 0, 0, 0); // Set background color to black
    return image;
}
