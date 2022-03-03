/*
 * Copyright (c) 2019-2022 Sander van der Burg
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

#include "nixxml-ds.h"
#include "nixxml-parse-generic.h"
#include "nixxml-print-generic-nix.h"
#include "nixxml-print-generic-xml.h"
#include "nixxml-generate-env-generic.h"
#include "nixxml-xmlhashtable.h"
#include "nixxml-ptrarray.h"

static void delete_list(void *list)
{
    NixXML_delete_ptr_array((void**)list, (NixXML_DeletePtrArrayElementFunc)NixXML_delete_node_ds);
}

static void delete_attrset(xmlHashTablePtr hash_table);

static void node_deallocator(void *payload, const xmlChar *name)
{
    NixXML_delete_node_ds((NixXML_Node*)payload);
}

static void delete_attrset(xmlHashTablePtr hash_table)
{
    xmlHashFree(hash_table, node_deallocator);
}

void NixXML_delete_node_ds(NixXML_Node *node)
{
    NixXML_delete_node(node, delete_list, (NixXML_DeletePtrArrayElementFunc)delete_attrset);
}

static NixXML_bool compare_lists(const void **left, const void **right)
{
    return NixXML_compare_ptr_arrays(left, right, (NixXML_ComparePtrArrayElementFunc)NixXML_compare_nodes_ds);
}

static NixXML_bool compare_attrsets(xmlHashTablePtr left, xmlHashTablePtr right)
{
    return NixXML_compare_xml_hash_tables(left, right, (NixXML_CompareXMLHashTableValueFunc)NixXML_compare_nodes_ds);
}

NixXML_bool NixXML_compare_nodes_ds(const NixXML_Node *left, const NixXML_Node *right)
{
    return NixXML_compare_nodes(left, right, (NixXML_CompareObjectFunc)compare_lists, (NixXML_CompareObjectFunc)compare_attrsets);
}

void *NixXML_generic_parse_simple_expr_ds(xmlNodePtr element, const char *type_property_name, void *userdata)
{
    return NixXML_generic_parse_simple_expr(element, type_property_name, NixXML_create_ptr_array_from_element, NixXML_create_xml_hash_table_from_element, NixXML_add_value_to_ptr_array, NixXML_insert_into_xml_hash_table, NixXML_finalize_ptr_array);
}

void *NixXML_generic_parse_verbose_expr_ds(xmlNodePtr element, const char *type_property_name, const char *name_property_name, void *userdata)
{
    return NixXML_generic_parse_verbose_expr(element, type_property_name, name_property_name, NixXML_create_ptr_array_from_element, NixXML_create_xml_hash_table_from_element, NixXML_add_value_to_ptr_array, NixXML_insert_into_xml_hash_table, NixXML_finalize_ptr_array);
}

void NixXML_print_generic_expr_ds_nix(FILE *file, const NixXML_Node *node, const int indent_level)
{
    NixXML_print_generic_expr_nix(file, node, indent_level, NixXML_print_ptr_array_elements_nix, NixXML_print_xml_hash_table_attributes_nix);
}

void NixXML_print_generic_expr_ds_ordered_nix(FILE *file, const NixXML_Node *node, const int indent_level)
{
    NixXML_print_generic_expr_nix(file, node, indent_level, NixXML_print_ptr_array_elements_nix, NixXML_print_xml_hash_table_ordered_attributes_nix);
}

void NixXML_print_generic_expr_ds_simple_xml(FILE *file, const NixXML_Node *node, const int indent_level, const char *root_element_name, const char *list_element_name, const char *type_property_name)
{
    NixXML_print_generic_expr_simple_xml(file, node, indent_level, root_element_name, list_element_name, type_property_name, NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_simple_attributes_xml);
}

void NixXML_print_generic_expr_ds_simple_ordered_xml(FILE *file, const NixXML_Node *node, const int indent_level, const char *root_element_name, const char *list_element_name, const char *type_property_name)
{
    NixXML_print_generic_expr_simple_xml(file, node, indent_level, root_element_name, list_element_name, type_property_name, NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_simple_ordered_attributes_xml);
}

void NixXML_print_generic_expr_ds_verbose_xml(FILE *file, const NixXML_Node *node, const int indent_level, const char *root_element_name, const char *attr_element_name, const char *name_property_name, const char *list_element_name, const char *type_property_name)
{
    NixXML_print_generic_expr_verbose_xml(file, node, indent_level, root_element_name, list_element_name, attr_element_name, name_property_name, type_property_name, NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_verbose_attributes_xml);
}

void NixXML_print_generic_expr_ds_verbose_ordered_xml(FILE *file, const NixXML_Node *node, const int indent_level, const char *root_element_name, const char *attr_element_name, const char *name_property_name, const char *list_element_name, const char *type_property_name)
{
    NixXML_print_generic_expr_simple_xml(stdout, node, indent_level, root_element_name, list_element_name, type_property_name, NixXML_print_ptr_array_elements_xml, NixXML_print_xml_hash_table_simple_ordered_attributes_xml);
}

xmlChar **NixXML_generate_env_vars_generic_ds(xmlHashTablePtr hash_table)
{
    NixXML_GenerateEnvGenericParams params;
    params.generate_list_env = NixXML_generate_env_value_from_ptr_array;
    return NixXML_generate_env_vars_from_xml_hash_table(hash_table, &params, NixXML_generate_env_value_from_node);
}
