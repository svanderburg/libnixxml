/*
 * Copyright (c) 2019 Sander van der Burg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "pretty-print.h"

#include "nixxml-parse-generic.h"
#include "nixxml-print-generic-nix.h"
#include "nixxml-print-generic-xml.h"
#include "nixxml-ptrarray.h"
#include "nixxml-xmlhashtable.h"
#include "nixxml-ds.h"

static NixXML_Node *generic_parse_expr(xmlNodePtr element)
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
    NixXML_delete_ptr_array((void**)list, (NixXML_DeletePtrArrayElementFunc)delete_node);
}

static void delete_attrset(xmlHashTablePtr hash_table);

static void node_deallocator(void *payload, const xmlChar *name)
{
    delete_node((NixXML_Node*)payload);
}

static void delete_attrset(xmlHashTablePtr hash_table)
{
    xmlHashFree(hash_table, node_deallocator);
}

static void delete_node(NixXML_Node *node)
{
    NixXML_delete_node(node, delete_list, (NixXML_DeletePtrArrayElementFunc)delete_attrset);
}

int pretty_print_file(const char *config_file, FormatType format, int indent_level, const char *root_element_name, const char *list_element_name, const char *attr_element_name, const char *name_property_name, const char *type_property_name, int order_keys)
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
            case FORMAT_NONE:
                break;
            case FORMAT_NIX:
                if(order_keys)
                    NixXML_print_generic_expr_ds_ordered_nix(stdout, node, indent_level);
                else
                    NixXML_print_generic_expr_ds_nix(stdout, node, indent_level);
                break;
            case FORMAT_VERBOSE_XML:
                if(order_keys)
                    NixXML_print_generic_expr_ds_verbose_ordered_xml(stdout, node, indent_level, root_element_name, attr_element_name, name_property_name, list_element_name, type_property_name);
                else
                    NixXML_print_generic_expr_ds_verbose_xml(stdout, node, indent_level, root_element_name, attr_element_name, name_property_name, list_element_name, type_property_name);
                break;
            case FORMAT_SIMPLE_XML:
                if(order_keys)
                    NixXML_print_generic_expr_ds_simple_ordered_xml(stdout, node, indent_level, root_element_name, list_element_name, type_property_name);
                else
                    NixXML_print_generic_expr_ds_simple_xml(stdout, node, indent_level, root_element_name, list_element_name, type_property_name);
                break;
        }

        delete_node(node);
        return 0;
    }
}
