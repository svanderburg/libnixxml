#ifndef __DIMENSIONS_H
#define __DIMENSIONS_H
#include <libxml/parser.h>
#include <gd.h>

typedef struct
{
    int width;
    int height;
}
Dimensions;

void *parse_dimensions(xmlNodePtr element, void *userdata);

void delete_dimensions(Dimensions *dimensions);

int check_dimensions(const Dimensions *dimensions);

void print_dimensions_nix(FILE *file, const void *figure, const int indent_level, void *userdata);

void print_dimensions_xml(FILE *file, const void *figure, const int indent_level, const char *type_property_name, void *userdata);

gdImagePtr allocate_image_with_dimensions(const Dimensions *dimensions);

#endif
