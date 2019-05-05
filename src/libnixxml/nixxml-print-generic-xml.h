#ifndef __NIXXML_PRINT_GENERIC_XML_H
#define __NIXXML_PRINT_GENERIC_XML_H
#include <stdio.h>
#include "nixxml-node.h"
#include "nixxml-print-xml.h"

/**
 * @brief Generic parameters for printing simple XML that get propagated to all print functions
 */
typedef struct
{
    /** Name of the list elements */
    const char *child_element_name;
    /** Pointer to a function that prints the list elements */
    NixXML_PrintXMLMembersFunc print_list_elements;
    /** Pointer to a function that prints the attributes */
    NixXML_PrintSimpleXMLMembersFunc print_attributes;
}
NixXML_SimplePrintExprParams;

/**
 * Internal function (that can be integrated into the printing infrastructure)
 * that generically prints a simple XML representation of a data structure of
 * XML_Node objects.
 *
 * @param file File descriptor to write to
 * @param value Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata A NixXML_SimplePrintExprParams object that gets propagated to all print functions
 */
void NixXML_print_expr_simple_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata);

/**
 * Generically prints a simple XML representation of a data structure of XML_Node
 * objects.
 *
 * @param file File descriptor to write to
 * @param value Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param root_element_name Name of the root element
 * @param child_element_name Name of the list elements
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param print_list_elements Pointer to a function that prints the list elements
 * @param print_attributes Pointer to a function that prints the attributes
 */
void NixXML_print_generic_expr_simple_xml(FILE *file, const NixXML_Node *value, const int indent_level, const char *root_element_name, const char *child_element_name, const char *type_property_name, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintSimpleXMLMembersFunc print_attributes);

/**
 * @brief Generic parameters for printing verbose XML that get propagated to all print functions
 */
typedef struct
{
    /** List element name */
    const char *list_element_name;
    /** Attribute element name */
    const char *attr_element_name;
    /** Name of the name property */
    const char *name_property_name;
    /** Pointer to a function that prints the list elements */
    NixXML_PrintXMLMembersFunc print_list_elements;
    /** Pointer to a function that prints the attributes */
    NixXML_PrintVerboseXMLMembersFunc print_attributes;
}
NixXML_VerbosePrintExprParams;

/**
 * Internal function (that can be integrated into the printing infrastructure)
 * that generically prints a verbose XML representation of a data structure of
 * XML_Node objects.
 *
 * @param file File descriptor to write to
 * @param value Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param userdata A NixXML_VerbosePrintExprParams object that gets propagated to all print functions
 */
void NixXML_print_expr_verbose_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata);

/**
 * Generically prints a verbose XML representation of a data structure of
 * XML_Node objects.
 *
 * @param file File descriptor to write to
 * @param value Root XML_Node of the data structure to print
 * @param indent_level Specifies the indent level, or -1 to disable indentation
 * @param root_element_name Name of the root element
 * @param list_element_name List element name
 * @param attr_element_name Attribute element name
 * @param name_property_name Name of the name property
 * @param type_property_name Name of the type property or NULL to not display any type annotations
 * @param print_list_elements Pointer to a function that prints the list elements
 * @param print_attributes Pointer to a function that prints the attributes
 */
void NixXML_print_generic_expr_verbose_xml(FILE *file, const NixXML_Node *value, const int indent_level, const char *root_element_name, const char *list_element_name, const char *attr_element_name, const char *name_property_name, const char *type_property_name, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintVerboseXMLMembersFunc print_attributes);

#endif
