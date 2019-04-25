#ifndef __FIGURE_H
#define __FIGURE_H
#include <libxml/parser.h>
#include <gd.h>

typedef struct
{
    xmlChar *type;

    int width;
    int height;

    int radius;
}
Figure;

void *parse_figure(xmlNodePtr element, void *userdata);

void delete_figure(Figure *figure);

int check_figure(const Figure *figure);

void print_figure_nix(FILE *file, const void *figure, const int indent_level, void *userdata);

void print_figure_xml(FILE *file, const void *figure, const int indent_level, void *userdata);

void draw_figure(const Figure *figure, gdImagePtr image, int x, int y);

#endif
