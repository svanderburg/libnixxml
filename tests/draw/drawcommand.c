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
        drawCommand->figure = value;
    else if(xmlStrcmp(key, (xmlChar*) "x") == 0)
    {
        drawCommand->x = atoi(value);
        xmlFree(value);
    }
    else if(xmlStrcmp(key, (xmlChar*) "y") == 0)
    {
        drawCommand->y = atoi(value);
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
    if(drawCommand->x < 0)
    {
        fprintf(stderr, "The draw command x coordinator should be 0 or greater\n");
        return FALSE;
    }

    if(drawCommand->y < 0)
    {
        fprintf(stderr, "The draw command y coordinator should be 0 or greater\n");
        return FALSE;
    }

    if(xmlHashLookup(figures_table, drawCommand->figure) == NULL)
    {
        fprintf(stderr, "Dangling reference from draw command to figure: %s\n", drawCommand->figure);
        return FALSE;
    }

    return TRUE;
}

/* Nix print functionality */

static void print_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const DrawCommand *drawCommand = (const DrawCommand*)value;
    NixXML_print_attribute_nix(file, "figure", drawCommand->figure, indent_level, userdata, NixXML_print_string_nix);
    NixXML_print_attribute_nix(file, "x", &drawCommand->x, indent_level, userdata, NixXML_print_int_nix);
    NixXML_print_attribute_nix(file, "y", &drawCommand->y, indent_level, userdata, NixXML_print_int_nix);
}

void print_draw_command_nix(FILE *file, const void *drawCommand, const int indent_level, void *userdata)
{
    NixXML_print_attrset_nix(file, drawCommand, indent_level, userdata, print_attributes_nix, NULL);
}

/* XML print functionality */

static void print_attributes_xml(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const DrawCommand *drawCommand = (const DrawCommand*)value;
    NixXML_print_simple_attribute_xml(file, "figure", drawCommand->figure, indent_level, userdata, NixXML_print_string_xml);
    NixXML_print_simple_attribute_xml(file, "x", &drawCommand->x, indent_level, userdata, NixXML_print_int_xml);
    NixXML_print_simple_attribute_xml(file, "y", &drawCommand->y, indent_level, userdata, NixXML_print_int_xml);
}

void print_draw_command_xml(FILE *file, const void *drawCommand, const int indent_level, void *userdata)
{
    NixXML_print_attrset_xml(file, drawCommand, indent_level, userdata, print_attributes_xml, NULL);
}

void execute_draw_command(const DrawCommand *drawCommand, xmlHashTablePtr figures_table, gdImagePtr image)
{
    Figure *figure = xmlHashLookup(figures_table, drawCommand->figure);
    draw_figure(figure, image, drawCommand->x, drawCommand->y);
}
