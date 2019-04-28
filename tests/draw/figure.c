#include "figure.h"
#include <stdlib.h>
#include "nixxml-parse.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"

#define TRUE 1
#define FALSE 0

static void *create_figure(xmlNodePtr element, void *userdata)
{
    return calloc(1, sizeof(Figure));
}

static void insert_figure_attribute(void *table, const xmlChar *key, void *value, void *userdata)
{
    Figure *figure = (Figure*)table;

    if(xmlStrcmp(key, (xmlChar*) "type") == 0)
        figure->type = value;
    else if(xmlStrcmp(key, (xmlChar*) "width") == 0)
    {
        figure->width = atoi(value);
        xmlFree(value);
    }
    else if(xmlStrcmp(key, (xmlChar*) "height") == 0)
    {
        figure->height = atoi(value);
        xmlFree(value);
    }
    else if(xmlStrcmp(key, (xmlChar*) "radius") == 0)
    {
        figure->radius = atoi(value);
        xmlFree(value);
    }
    else
        xmlFree(value);
}

void *parse_figure(xmlNodePtr element, void *userdata)
{
    return NixXML_parse_simple_attrset(element, userdata, create_figure, NixXML_parse_value, insert_figure_attribute);
}

void delete_figure(Figure *figure)
{
    if(figure != NULL)
    {
        xmlFree(figure->type);
        free(figure);
    }
}

int check_figure(const Figure *figure)
{
    if(xmlStrcmp(figure->type, (xmlChar*) "rectangle") != 0 && xmlStrcmp(figure->type, (xmlChar *) "circle") != 0)
    {
        fprintf(stderr, "Unknown figure type: %s\n", figure->type);
        return FALSE;
    }

    if(figure->width < 0)
    {
        fprintf(stderr, "The figure width should be 0 or greater\n");
        return FALSE;
    }

    if(figure->height < 0)
    {
        fprintf(stderr, "The figure height should be 0 or greater\n");
        return FALSE;
    }

    return TRUE;
}

static void print_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const Figure *figure = (const Figure*)value;
    NixXML_print_attribute_nix(file, "type", figure->type, indent_level, userdata, NixXML_print_string_nix);
    NixXML_print_attribute_nix(file, "width", &figure->width, indent_level, userdata, NixXML_print_int_nix);
    NixXML_print_attribute_nix(file, "height", &figure->height, indent_level, userdata, NixXML_print_int_nix);
    NixXML_print_attribute_nix(file, "radius", &figure->radius, indent_level, userdata, NixXML_print_int_nix);
}

void print_figure_nix(FILE *file, const void *figure, const int indent_level, void *userdata)
{
    NixXML_print_attrset_nix(file, figure, indent_level, userdata, print_attributes_nix, NULL);
}

static void print_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_attribute_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    const Figure *figure = (const Figure*)value;
    NixXML_print_simple_attribute_xml(file, "type", figure->type, indent_level, type_attribute_name, userdata, NixXML_print_string_xml);
    NixXML_print_simple_attribute_xml(file, "width", &figure->width, indent_level, type_attribute_name, userdata, NixXML_print_int_xml);
    NixXML_print_simple_attribute_xml(file, "height", &figure->height, indent_level, type_attribute_name, userdata, NixXML_print_int_xml);
    NixXML_print_simple_attribute_xml(file, "radius", &figure->radius, indent_level, type_attribute_name, userdata, NixXML_print_int_xml);
}

void print_figure_xml(FILE *file, const void *figure, const int indent_level, const char *type_property_name, void *userdata)
{
    NixXML_print_simple_attrset_xml(file, figure, indent_level, type_property_name, userdata, print_attributes_xml, NULL);
}

void draw_figure(const Figure *figure, gdImagePtr image, int x, int y)
{
    int white = gdImageColorAllocate(image, 255, 255, 255);

    if(xmlStrcmp(figure->type, (xmlChar*) "rectangle") == 0)
        gdImageRectangle(image, x, y, x + figure->width, y + figure->height, white);
    else if(xmlStrcmp(figure->type, (xmlChar*) "circle") == 0)
        gdImageEllipse(image, x, y, figure->radius, figure->radius, white);
}
