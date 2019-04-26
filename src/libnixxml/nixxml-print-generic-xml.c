#include "nixxml-print-generic-xml.h"

typedef struct
{
    const char *child_element_name;
    NixXML_PrintXMLMembersFunc print_list_elements;
    NixXML_PrintMembersFunc print_attributes;
}
SimplePrintExprParams;

void NixXML_print_expr_simple_xml(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_Node *node = (NixXML_Node*)value;
    SimplePrintExprParams *params = (SimplePrintExprParams*)userdata;

    switch(node->type)
    {
        case NIX_XML_TYPE_STRING:
        case NIX_XML_TYPE_INT:
        case NIX_XML_TYPE_FLOAT:
        case NIX_XML_TYPE_BOOL:
            NixXML_print_string_xml(file, node->value, indent_level, userdata);
            break;
        case NIX_XML_TYPE_LIST:
            NixXML_print_list_xml(file, node->value, params->child_element_name, indent_level, userdata, params->print_list_elements, NixXML_print_expr_simple_xml);
            break;
        case NIX_XML_TYPE_ATTRSET:
            NixXML_print_simple_attrset_xml(file, node->value, indent_level, userdata, params->print_attributes, NixXML_print_expr_simple_xml);
            break;
    }
}

void NixXML_print_generic_expr_simple_xml(FILE *file, const NixXML_Node *value, const int indent_level, const char *root_element_name, const char *child_element_name, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintMembersFunc print_attributes)
{
    SimplePrintExprParams params = { child_element_name, print_list_elements, print_attributes };
    NixXML_print_open_root_element(file, root_element_name);
    NixXML_print_expr_simple_xml(file, value, indent_level, &params);
    NixXML_print_close_root_element(file, child_element_name);
}

typedef struct
{
    const char *list_element_name;
    const char *attr_element_name;
    const char *name_property_name;
    NixXML_PrintXMLMembersFunc print_list_elements;
    NixXML_PrintVerboseXMLMembersFunc print_attributes;
}
VerbosePrintExprParams;

void NixXML_print_expr_verbose_xml(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_Node *node = (NixXML_Node*)value;
    VerbosePrintExprParams *params = (VerbosePrintExprParams*)userdata;

    switch(node->type)
    {
        case NIX_XML_TYPE_STRING:
        case NIX_XML_TYPE_INT:
        case NIX_XML_TYPE_FLOAT:
        case NIX_XML_TYPE_BOOL:
            NixXML_print_string_xml(file, node->value, indent_level, userdata);
            break;
        case NIX_XML_TYPE_LIST:
            NixXML_print_list_xml(file, node->value, params->list_element_name, indent_level, userdata, params->print_list_elements, NixXML_print_expr_verbose_xml);
            break;
        case NIX_XML_TYPE_ATTRSET:
            NixXML_print_verbose_attrset_xml(file, node->value, params->attr_element_name, params->name_property_name, indent_level, userdata, params->print_attributes, NixXML_print_expr_verbose_xml);
            break;
    }
}

void NixXML_print_generic_expr_verbose_xml(FILE *file, const NixXML_Node *value, const int indent_level, const char *root_element_name, const char *list_element_name, const char *attr_element_name, const char *name_property_name, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintVerboseXMLMembersFunc print_attributes)
{
    VerbosePrintExprParams params = { list_element_name, attr_element_name, name_property_name, print_list_elements, print_attributes };
    NixXML_print_open_root_element(file, root_element_name);
    NixXML_print_expr_verbose_xml(file, value, indent_level, &params);
    NixXML_print_close_root_element(file, root_element_name);
}
