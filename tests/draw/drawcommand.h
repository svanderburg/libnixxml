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

void print_draw_command_nix(FILE *file, const void *drawCommand, const int indent_level, void *userdata);

void print_draw_command_xml(FILE *file, const void *drawCommand, const int indent_level, const char *type_property_name, void *userdata);

void execute_draw_command(const DrawCommand *drawCommand, xmlHashTablePtr figures_table, gdImagePtr image);

#endif
