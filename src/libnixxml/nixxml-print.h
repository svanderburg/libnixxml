#ifndef __NIXXML_PRINT_H
#define __NIXXML_PRINT_H

#include <stdio.h>

/**
 * Prints indentation of 2 spaces per indent level.
 *
 * @param file File descriptor to write to
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 */
void NixXML_print_indentation(FILE *file, const int indent_level);

#endif
