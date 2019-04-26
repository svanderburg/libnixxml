#include "nixxml-print-generic-nix.h"

typedef struct
{
    NixXML_PrintMembersFunc print_list_elements;
    NixXML_PrintMembersFunc print_attributes;
}
PrintExprParams;

void NixXML_print_expr_nix(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_Node *node = (NixXML_Node*)value;
    PrintExprParams *params = (PrintExprParams*)userdata;

    switch(node->type)
    {
        case NIX_XML_TYPE_STRING:
            NixXML_print_string_nix(file, node->value, indent_level, userdata);
            break;
        case NIX_XML_TYPE_INT:
        case NIX_XML_TYPE_FLOAT:
        case NIX_XML_TYPE_BOOL:
            NixXML_print_value_nix(file, node->value, indent_level, userdata);
            break;
        case NIX_XML_TYPE_LIST:
            NixXML_print_list_nix(file, node->value, indent_level, userdata, params->print_list_elements, NixXML_print_expr_nix);
            break;
        case NIX_XML_TYPE_ATTRSET:
            NixXML_print_attrset_nix(file, node->value, indent_level, userdata, params->print_attributes, NixXML_print_expr_nix);
            break;
    }
}

void NixXML_print_generic_expr_nix(FILE *file, const NixXML_Node *value, const int indent_level, NixXML_PrintMembersFunc print_list_elements, NixXML_PrintMembersFunc print_attributes)
{
    PrintExprParams params = { print_list_elements, print_attributes };
    return NixXML_print_expr_nix(file, value, indent_level, &params);
}
