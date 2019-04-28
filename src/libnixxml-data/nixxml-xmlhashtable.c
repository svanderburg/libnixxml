#include "nixxml-xmlhashtable.h"

void *NixXML_create_xml_hash_table(xmlNodePtr element, void *userdata)
{
    unsigned long numOfElements = xmlChildElementCount(element);
    return xmlHashCreate(numOfElements);
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

/* Parse functionality */

void *NixXML_parse_xml_hash_table(xmlNodePtr element, void *userdata, NixXML_ParseObjectFunc parse_object)
{
    return NixXML_parse_simple_attrset(element, userdata, NixXML_create_xml_hash_table, parse_object, NixXML_insert_into_xml_hash_table);
}
