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

static NixXML_Object *open_expr(const char *filename)
{
    xmlDocPtr doc;
    xmlNodePtr node_root;
    NixXML_Object *obj;

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
    obj = generic_parse_expr(node_root);

    /* Cleanup */
    xmlFreeDoc(doc);
    xmlCleanupCharEncodingHandlers();
    xmlCleanupParser();

    /* Return expression data structure */
    return obj;
}

static void delete_object(NixXML_Object *obj);

static void delete_list(void *list)
{
    NixXML_delete_ptr_array(list, (NixXML_DeletePtrArrayElementFunc)delete_object);
}

static void delete_attrset(xmlHashTablePtr hash_table);

static void object_deallocator(void *payload, const xmlChar *name)
{
    delete_object(payload);
}

static void delete_attrset(xmlHashTablePtr hash_table)
{
    xmlHashFree(hash_table, object_deallocator);
}

static void delete_object(NixXML_Object *obj)
{
    NixXML_delete_object(obj, delete_list, (NixXML_DeletePtrArrayElementFunc)delete_attrset);
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
        NixXML_Object *obj = open_expr(config_file);

        if(obj == NULL)
        {
            fprintf(stderr, "Cannot open expression XML file!\n");
            return 1;
        }
        else
        {
            /* Execute the desired operation */

            if(strcmp(output_type, "nix") == 0)
                NixXML_print_generic_expr_nix(stdout, obj, 0, NixXML_print_ptr_array_elements_nix, NixXML_print_xml_hash_table_attributes_nix);
            else if(strcmp(output_type, "xml") == 0)
                NixXML_print_generic_expr_xml(stdout, obj, 0, "expr", "elem", NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_verbose_attributes_xml);
            else if(strcmp(output_type, "simple-xml") == 0)
                NixXML_print_generic_expr_xml(stdout, obj, 0, "expr", "elem", NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_simple_attributes_xml);

            delete_object(obj);
            return 0;
        }
    }
}
