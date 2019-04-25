#include "nixxml-print-generic-nix.h"

typedef struct
{
    NixXML_PrintMembersFunc print_list_elements;
    NixXML_PrintMembersFunc print_attributes;
}
PrintExprParams;

void NixXML_print_expr_nix(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_Object *obj = (NixXML_Object*)value;
    PrintExprParams *params = (PrintExprParams*)userdata;

    switch(obj->type)
    {
        case NIX_XML_TYPE_STRING:
            NixXML_print_string_nix(file, obj->value, indent_level, userdata);
            break;
        case NIX_XML_TYPE_INT:
        case NIX_XML_TYPE_FLOAT:
        case NIX_XML_TYPE_BOOL:
            NixXML_print_value_nix(file, obj->value, indent_level, userdata);
            break;
        case NIX_XML_TYPE_LIST:
            NixXML_print_list_nix(file, obj->value, indent_level, userdata, params->print_list_elements, NixXML_print_expr_nix);
            break;
        case NIX_XML_TYPE_ATTRSET:
            NixXML_print_attrset_nix(file, obj->value, indent_level, userdata, params->print_attributes, NixXML_print_expr_nix);
            break;
    }
}

void NixXML_print_generic_expr_nix(FILE *file, const NixXML_Object *value, const int indent_level, NixXML_PrintMembersFunc print_list_elements, NixXML_PrintMembersFunc print_attributes)
{
    PrintExprParams params = { print_list_elements, print_attributes };
    return NixXML_print_expr_nix(file, value, indent_level, &params);
}
