#include "nixxml-print-generic-xml.h"

typedef struct
{
    const char *child_element_name;
    NixXML_PrintXMLMembersFunc print_list_elements;
    NixXML_PrintMembersFunc print_attributes;
}
PrintExprParams;

void NixXML_print_expr_xml(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_Object *obj = (NixXML_Object*)value;
    PrintExprParams *params = (PrintExprParams*)userdata;

    switch(obj->type)
    {
        case NIX_XML_TYPE_STRING:
        case NIX_XML_TYPE_INT:
        case NIX_XML_TYPE_FLOAT:
        case NIX_XML_TYPE_BOOL:
            NixXML_print_string_xml(file, obj->value, indent_level, userdata);
            break;
        case NIX_XML_TYPE_LIST:
            NixXML_print_list_xml(file, obj->value, params->child_element_name, indent_level, userdata, params->print_list_elements, NixXML_print_expr_xml);
            break;
        case NIX_XML_TYPE_ATTRSET:
            NixXML_print_attrset_xml(file, obj->value, indent_level, userdata, params->print_attributes, NixXML_print_expr_xml);
            break;
    }
}

void NixXML_print_generic_expr_xml(FILE *file, const NixXML_Object *value, const int indent_level, const char *root_element_name, const char *child_element_name, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintMembersFunc print_attributes)
{
    PrintExprParams params = { child_element_name, print_list_elements, print_attributes };
    fprintf(stderr, "<%s>", root_element_name);
    NixXML_print_expr_xml(file, value, indent_level, &params);
    fprintf(stderr, "</%s>", root_element_name);
}
