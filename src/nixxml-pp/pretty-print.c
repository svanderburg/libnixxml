#include "pretty-print.h"

#include "nixxml-parse-generic.h"
#include "nixxml-print-generic-nix.h"
#include "nixxml-print-generic-xml.h"
#include "nixxml-ptrarray.h"
#include "nixxml-xmlhashtable.h"

static void *generic_parse_expr(xmlNodePtr element)
{
    return NixXML_generic_parse_expr(element, "type", "name", NixXML_create_ptr_array, NixXML_create_xml_hash_table, NixXML_add_value_to_ptr_array, NixXML_insert_into_xml_hash_table, NixXML_finalize_ptr_array);
}

static NixXML_Node *open_expr(const char *filename)
{
    xmlDocPtr doc;
    xmlNodePtr node_root;
    NixXML_Node *node;

    /* Parse the XML document */

    if((doc = xmlParseFile(filename)) == NULL)
    {
        fprintf(stderr, "Error with parsing the XML file!\n");
        xmlCleanupCharEncodingHandlers();
        xmlCleanupParser();
        return NULL;
    }

    /* Retrieve root element */
    node_root = xmlDocGetRootElement(doc);

    if(node_root == NULL)
    {
        fprintf(stderr, "The XML file is empty!\n");
        xmlFreeDoc(doc);
        xmlCleanupCharEncodingHandlers();
        xmlCleanupParser();
        return NULL;
    }

    /* Parse expression */
    node = generic_parse_expr(node_root);

    /* Cleanup */
    xmlFreeDoc(doc);
    xmlCleanupCharEncodingHandlers();
    xmlCleanupParser();

    /* Return expression data structure */
    return node;
}

static void delete_node(NixXML_Node *node);

static void delete_list(void *list)
{
    NixXML_delete_ptr_array(list, (NixXML_DeletePtrArrayElementFunc)delete_node);
}

static void delete_attrset(xmlHashTablePtr hash_table);

static void node_deallocator(void *payload, const xmlChar *name)
{
    delete_node(payload);
}

static void delete_attrset(xmlHashTablePtr hash_table)
{
    xmlHashFree(hash_table, node_deallocator);
}

static void delete_node(NixXML_Node *node)
{
    NixXML_delete_node(node, delete_list, (NixXML_DeletePtrArrayElementFunc)delete_attrset);
}

int pretty_print_file(const char *config_file, FormatType format, int indent_level, const char *root_element_name, const char *list_element_name, const char *attr_element_name, const char *name_property_name, const char *type_property_name)
{
    NixXML_Node *node = open_expr(config_file);

    if(node == NULL)
    {
        fprintf(stderr, "Cannot open expression XML file!\n");
        return 1;
    }
    else
    {
        /* Execute the desired operation */

        switch(format)
        {
            case FORMAT_NIX:
                NixXML_print_generic_expr_nix(stdout, node, indent_level, NixXML_print_ptr_array_elements_nix, NixXML_print_xml_hash_table_attributes_nix);
                break;
                ;;
            case FORMAT_VERBOSE_XML:
                NixXML_print_generic_expr_verbose_xml(stdout, node, indent_level, root_element_name, list_element_name, attr_element_name, name_property_name, type_property_name, NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_verbose_attributes_xml);
                break;
                ;;
            case FORMAT_SIMPLE_XML:
                NixXML_print_generic_expr_simple_xml(stdout, node, indent_level, root_element_name, list_element_name, type_property_name, NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_simple_attributes_xml);
                break;
                ;;
        }

        delete_node(node);
        return 0;
    }
}
