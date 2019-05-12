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

#include "pretty-print-glib.h"

#include "nixxml-parse-generic.h"
#include "nixxml-print-generic-nix.h"
#include "nixxml-print-generic-xml.h"
#include "nixxml-gptrarray.h"
#include "nixxml-ghashtable.h"

static void *generic_parse_expr(xmlNodePtr element)
{
    return NixXML_generic_parse_expr(element, "type", "name", NixXML_create_g_ptr_array, NixXML_create_g_hash_table, NixXML_add_value_to_g_ptr_array, NixXML_insert_into_g_hash_table, NixXML_finalize_g_ptr_array);
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
    NixXML_delete_g_ptr_array(list, (NixXML_DeletePtrArrayElementFunc)delete_node);
}

static void delete_attrset(GHashTable *hash_table)
{
    GHashTableIter iter;
    gpointer *key;
    gpointer *obj;

    g_hash_table_iter_init(&iter, hash_table);
    while(g_hash_table_iter_next(&iter, (gpointer*)&key, (gpointer*)&obj))
        delete_node((NixXML_Node*)obj);

    g_hash_table_destroy(hash_table);
}

static void delete_node(NixXML_Node *node)
{
    NixXML_delete_node(node, delete_list, (NixXML_DeletePtrArrayElementFunc)delete_attrset);
}

int pretty_print_file_glib(const char *config_file, FormatType format, int indent_level, const char *root_element_name, const char *list_element_name, const char *attr_element_name, const char *name_property_name, const char *type_property_name)
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
                NixXML_print_generic_expr_nix(stdout, node, indent_level, NixXML_print_g_ptr_array_elements_nix, NixXML_print_g_hash_table_attributes_nix);
                break;
            case FORMAT_VERBOSE_XML:
                NixXML_print_generic_expr_verbose_xml(stdout, node, indent_level, root_element_name, list_element_name, attr_element_name, name_property_name, type_property_name, NixXML_print_g_ptr_array_elements_xml, NixXML_print_g_hash_table_verbose_attributes_xml);
                break;
            case FORMAT_SIMPLE_XML:
                NixXML_print_generic_expr_simple_xml(stdout, node, indent_level, root_element_name, list_element_name, type_property_name, NixXML_print_g_ptr_array_elements_xml, NixXML_print_g_hash_table_simple_attributes_xml);
                break;
        }

        delete_node(node);
        return 0;
    }
}
