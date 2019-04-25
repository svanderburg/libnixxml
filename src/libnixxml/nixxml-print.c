#include "nixxml-print.h"

void NixXML_print_indentation(FILE *file, const int indent_level)
{
    if(indent_level >= 0)
    {
        unsigned int i;

        for(i = 0; i < indent_level; i++)
            fprintf(file, "  ");
    }
}
