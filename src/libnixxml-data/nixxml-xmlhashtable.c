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

#include "nixxml-xmlhashtable.h"
#include <stdlib.h>
#include "nixxml-util.h"
#include "nixxml-xmlhashtable-scanner.h"

#define TRUE 1
#define FALSE 0

void *NixXML_create_xml_hash_table(xmlNodePtr element, void *userdata)
{
    unsigned long numOfElements = xmlChildElementCount(element);
    return xmlHashCreate(numOfElements);
}

typedef struct
{
    int result;
    NixXML_CheckXMLHashTableValueFunc check_function;
}
CheckParams;

static void scanner_check(void *payload, void *data, const xmlChar *name)
{
    CheckParams *params = (CheckParams*)data;
    if(!params->check_function(payload))
        params->result = FALSE;
}

int NixXML_check_xml_hash_table(xmlHashTablePtr hash_table, NixXML_CheckXMLHashTableValueFunc check_function)
{
    CheckParams params;
    params.result = TRUE;
    params.check_function = check_function;

    xmlHashScan(hash_table, scanner_check, &params);
    return params.result;
}

int NixXML_check_xml_property_table(xmlHashTablePtr property_table)
{
    return NixXML_check_xml_hash_table(property_table, NixXML_check_value_is_not_null);
}

typedef struct
{
    int result;
    xmlHashTablePtr hash_table2;
    NixXML_CompareXMLHashTableValueFunc compare_function;
}
CompareParams;

static void scanner_compare(void *payload, void *data, const xmlChar *name)
{
    CompareParams *params = (CompareParams*)data;
    void *value = xmlHashLookup(params->hash_table2, name);

    if(value == NULL)
        params->result = FALSE;
    else if(!params->compare_function(value, payload))
        params->result = FALSE;
}

int NixXML_compare_xml_hash_tables(xmlHashTablePtr hash_table1, xmlHashTablePtr hash_table2, NixXML_CompareXMLHashTableValueFunc compare_function)
{
    if(xmlHashSize(hash_table1) == xmlHashSize(hash_table2))
    {
        CompareParams params;
        params.result = TRUE;
        params.compare_function = compare_function;

        xmlHashScan(hash_table1, scanner_compare, &params);
        return params.result;
    }
    else
        return FALSE;
}

int NixXML_compare_xml_property_tables(xmlHashTablePtr property_table1, xmlHashTablePtr property_table2)
{
    return NixXML_compare_xml_hash_tables(property_table1, property_table2, (NixXML_CompareXMLHashTableValueFunc)NixXML_compare_xml_strings);
}

void NixXML_insert_into_xml_hash_table(void *table, const xmlChar *key, void *value, void *userdata)
{
    xmlHashTablePtr hash_table = (xmlHashTablePtr)table;
    xmlHashAddEntry(hash_table, key, value);
}

/* Nix printing */

typedef struct
{
    FILE *file;
    int indent_level;
    void *userdata;
    NixXML_PrintValueFunc print_value;
}
ScannerParams;

static void scanner_nix(void *payload, void *data, const xmlChar *name)
{
    ScannerParams *params = (ScannerParams*)data;
    NixXML_print_attribute_nix(params->file, (const char*)name, payload, params->indent_level, params->userdata, params->print_value);
}

void NixXML_print_xml_hash_table_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const xmlHashTablePtr hash_table = (const xmlHashTablePtr)value;
    ScannerParams params;
    params.file = file;
    params.indent_level = indent_level;
    params.userdata = userdata;
    params.print_value = print_value;

    xmlHashScan(hash_table, scanner_nix, &params);
}

void NixXML_print_xml_hash_table_nix(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    NixXML_print_attrset_nix(file, hash_table, indent_level, userdata, NixXML_print_xml_hash_table_attributes_nix, print_value);
}

void NixXML_print_xml_hash_table_ordered_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const xmlHashTablePtr hash_table = (const xmlHashTablePtr)value;
    ScannerParams params;
    params.file = file;
    params.indent_level = indent_level;
    params.userdata = userdata;
    params.print_value = print_value;

    NixXML_xmlHashScanOrdered(hash_table, scanner_nix, &params);
}

void NixXML_print_xml_hash_table_ordered_nix(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    NixXML_print_attrset_nix(file, hash_table, indent_level, userdata, NixXML_print_xml_hash_table_ordered_attributes_nix, print_value);
}

/* Simple XML printing */

typedef struct
{
    FILE *file;
    int indent_level;
    const char *type_property_name;
    void *userdata;
    NixXML_PrintXMLValueFunc print_value;
}
SimpleScannerParams;

static void scanner_simple_xml(void *payload, void *data, const xmlChar *name)
{
    SimpleScannerParams *params = (SimpleScannerParams*)data;
    NixXML_print_simple_attribute_xml(params->file, (const char*)name, payload, params->indent_level, params->type_property_name, params->userdata, params->print_value);
}

void NixXML_print_xml_hash_table_simple_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    const xmlHashTablePtr hash_table = (const xmlHashTablePtr)value;
    SimpleScannerParams params;
    params.file = file;
    params.indent_level = indent_level;
    params.type_property_name = type_property_name;
    params.userdata = userdata;
    params.print_value = print_value;

    xmlHashScan(hash_table, scanner_simple_xml, &params);
}

void NixXML_print_xml_hash_table_simple_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    NixXML_print_simple_attrset_xml(file, hash_table, indent_level, type_property_name, userdata, NixXML_print_xml_hash_table_simple_attributes_xml, print_value);
}

void NixXML_print_xml_hash_table_simple_ordered_attributes_xml(FILE *file, const void *value, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    const xmlHashTablePtr hash_table = (const xmlHashTablePtr)value;
    SimpleScannerParams params;
    params.file = file;
    params.indent_level = indent_level;
    params.type_property_name = type_property_name;
    params.userdata = userdata;
    params.print_value = print_value;

    NixXML_xmlHashScanOrdered(hash_table, scanner_simple_xml, &params);
}

void NixXML_print_xml_hash_table_simple_ordered_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    NixXML_print_simple_attrset_xml(file, hash_table, indent_level, type_property_name, userdata, NixXML_print_xml_hash_table_simple_ordered_attributes_xml, print_value);
}

/* Verbose XML printing */

typedef struct
{
    FILE *file;
    const char *child_element_name;
    const char *name_property_name;
    int indent_level;
    const char *type_property_name;
    void *userdata;
    NixXML_PrintXMLValueFunc print_value;
}
VerboseScannerParams;

static void scanner_verbose_xml(void *payload, void *data, const xmlChar *name)
{
    VerboseScannerParams *params = (VerboseScannerParams*)data;
    NixXML_print_verbose_attribute_xml(params->file, params->child_element_name, params->name_property_name, (const char *)name, payload, params->indent_level, params->type_property_name, params->userdata, params->print_value);
}

void NixXML_print_xml_hash_table_verbose_attributes_xml(FILE *file, const void *value, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    const xmlHashTablePtr hash_table = (const xmlHashTablePtr)value;
    VerboseScannerParams params;
    params.file = file;
    params.child_element_name = child_element_name;
    params.name_property_name = name_property_name;
    params.indent_level = indent_level;
    params.type_property_name = type_property_name;
    params.userdata = userdata;
    params.print_value = print_value;

    xmlHashScan(hash_table, scanner_verbose_xml, &params);
}

void NixXML_print_xml_hash_table_verbose_xml(FILE *file, xmlHashTablePtr hash_table, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    NixXML_print_verbose_attrset_xml(file, hash_table, child_element_name, name_property_name, indent_level, type_property_name, userdata, NixXML_print_xml_hash_table_verbose_attributes_xml, print_value);
}

void NixXML_print_xml_hash_table_verbose_ordered_attributes_xml(FILE *file, const void *value, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    const xmlHashTablePtr hash_table = (const xmlHashTablePtr)value;
    VerboseScannerParams params;
    params.file = file;
    params.child_element_name = child_element_name;
    params.name_property_name = name_property_name;
    params.indent_level = indent_level;
    params.type_property_name = type_property_name;
    params.userdata = userdata;
    params.print_value = print_value;

    NixXML_xmlHashScanOrdered(hash_table, scanner_verbose_xml, &params);
}

void NixXML_print_xml_hash_table_verbose_ordered_xml(FILE *file, xmlHashTablePtr hash_table, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    NixXML_print_verbose_attrset_xml(file, hash_table, child_element_name, name_property_name, indent_level, type_property_name, userdata, NixXML_print_xml_hash_table_verbose_ordered_attributes_xml, print_value);
}

/* Parse functionality */

void *NixXML_parse_xml_hash_table_simple(xmlNodePtr element, void *userdata, NixXML_ParseObjectFunc parse_object)
{
    return NixXML_parse_simple_attrset(element, userdata, NixXML_create_xml_hash_table, parse_object, NixXML_insert_into_xml_hash_table);
}

void *NixXML_parse_xml_hash_table_verbose(xmlNodePtr element, const char *child_element_name, const char *name_property_name, void *userdata, NixXML_ParseObjectFunc parse_object)
{
    return NixXML_parse_verbose_attrset(element, child_element_name, name_property_name, userdata, NixXML_create_xml_hash_table, parse_object, NixXML_insert_into_xml_hash_table);
}

/* Generate environment variables functionality */

typedef struct
{
    xmlChar **result;
    unsigned int count;
    void *userdata;
    NixXML_GenerateEnvValueFunc generate_value;
}
GenerateEnvScannerParams;

static void scanner_generate_env(void *payload, void *data, const xmlChar *name)
{
    GenerateEnvScannerParams *params = (GenerateEnvScannerParams*)data;

    params->result = (xmlChar**)realloc(params->result, (params->count + 1) * sizeof(xmlChar*));
    params->result[params->count] = NixXML_generate_env_variable(name, payload, params->userdata, params->generate_value);
    params->count++;
}

xmlChar **NixXML_generate_env_vars_from_xml_hash_table(xmlHashTablePtr hash_table, void *userdata, NixXML_GenerateEnvValueFunc generate_value)
{
    GenerateEnvScannerParams params;
    params.result = NULL;
    params.count = 0;
    params.userdata = userdata;
    params.generate_value = generate_value;

    xmlHashScan(hash_table, scanner_generate_env, &params);

    params.result = (xmlChar**)realloc(params.result, (params.count + 1) * sizeof(xmlChar*));
    params.result[params.count] = NULL; /* Add NULL termination to the end */

    return params.result;
}
