#ifndef __NIXXML_PRINT_GENERIC_NIX_H
#define __NIXXML_PRINT_GENERIC_NIX_H

#include <stdio.h>
#include "nixxml-node.h"
#include "nixxml-print-nix.h"

void NixXML_print_expr_nix(FILE *file, const void *value, const int indent_level, void *userdata);

void NixXML_print_generic_expr_nix(FILE *file, const NixXML_Node *value, const int indent_level, NixXML_PrintMembersFunc print_list_elements, NixXML_PrintMembersFunc print_attributes);

#endif
