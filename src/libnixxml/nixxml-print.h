#ifndef __NIXXML_PRINT_H
#define __NIXXML_PRINT_H

#include <stdio.h>

typedef void (*NixXML_PrintValueFunc) (FILE *file, const void *value, const int indent_level, void *userdata);
typedef void (*NixXML_PrintMembersFunc) (FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value);

void NixXML_print_indentation(FILE *file, const int indent_level);

#endif
