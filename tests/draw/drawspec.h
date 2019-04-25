#ifndef __DRAWSPEC_H
#define __DRAWSPEC_H
#include <libxml/parser.h>
#include "dimensions.h"
#include "drawcommand.h"

typedef struct
{
    Dimensions *dimensions;

    xmlHashTablePtr figures;

    DrawCommand **draw;

    xmlHashTablePtr meta;

    xmlChar **tags;
}
DrawSpec;

DrawSpec *open_drawspec(const char *filename);

void delete_drawspec(DrawSpec *drawSpec);

int check_drawspec(const DrawSpec *drawSpec);

void print_drawspec_nix(FILE *file, const DrawSpec *drawSpec);

void print_drawspec_xml(FILE *file, const DrawSpec *drawSpec);

void draw_image_from_drawspec(FILE *file, const DrawSpec *drawSpec);

#endif
