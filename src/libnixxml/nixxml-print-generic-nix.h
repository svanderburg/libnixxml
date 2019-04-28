#ifndef __NIXXML_PRINT_GENERIC_NIX_H
#define __NIXXML_PRINT_GENERIC_NIX_H
#include <stdio.h>
#include "nixxml-node.h"
#include "nixxml-print-nix.h"

/**
 * @brief Generic parameters for printing Nix expressions that get propagated to all print functions
 */
typedef struct
{
    /** Pointer to a function that prints the list elements */
    NixXML_PrintMembersFunc print_list_elements;
    /** Pointer to a function that prints the attributes */
    NixXML_PrintMembersFunc print_attributes;
}
NixXML_PrintExprParams;

/**
 * Internal function (that can be integrated into the printing infrastructure)
 * that generically prints a Nix representation of a data structure of XML_Node
 * objects.
 *
 * @param file File descriptor to write to
 * @param value Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param userdata A NixXML_PrintExprParams object that gets propagated to all print functions
 */
void NixXML_print_expr_nix(FILE *file, const void *value, const int indent_level, void *userdata);

/**
 * Generically prints a Nix representation of a data structure of XML_Node
 * objects.
 *
 * @param file File descriptor to write to
 * @param value Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param print_list_elements Pointer to a function that prints the list elements
 * @param print_attributes Pointer to a function that prints the attributes
 */
void NixXML_print_generic_expr_nix(FILE *file, const NixXML_Node *value, const int indent_level, NixXML_PrintMembersFunc print_list_elements, NixXML_PrintMembersFunc print_attributes);

#endif
