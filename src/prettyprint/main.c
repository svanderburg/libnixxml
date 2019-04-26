#include <stdio.h>
#include <string.h>
#include "nixxml-parse-generic.h"
#include "nixxml-print-generic-nix.h"
#include "nixxml-print-generic-xml.h"
#include "nixxml-ptrarray.h"
#include "nixxml-xmlhashtable.h"

static void print_usage(const char *command)
{
    printf("Usage: %s OUTPUT_TYPE CONFIG_XML\n", command);
}

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

int main(int argc, char *argv[])
{
    /* Parse command-line parameters */

    char *output_type;
    char *config_file;

    if(argc > 1)
        output_type = argv[1];
    else
        output_type = NULL;

    if(argc > 2)
        config_file = argv[2];
    else
        config_file = NULL;

    if(output_type == NULL || config_file == NULL)
    {
        fprintf(stderr, "Not all mandatory input parameters provided!\n");
        print_usage(argv[0]);
        return 1;
    }
    else
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

            if(strcmp(output_type, "nix") == 0)
                NixXML_print_generic_expr_nix(stdout, node, 0, NixXML_print_ptr_array_elements_nix, NixXML_print_xml_hash_table_attributes_nix);
            else if(strcmp(output_type, "xml") == 0)
                NixXML_print_generic_expr_verbose_xml(stdout, node, 0, "expr", "elem", "attr", "name", NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_verbose_attributes_xml);
            else if(strcmp(output_type, "simple-xml") == 0)
                NixXML_print_generic_expr_simple_xml(stdout, node, 0, "expr", "elem", NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_simple_attributes_xml);

            delete_node(node);
            return 0;
        }
    }
}
