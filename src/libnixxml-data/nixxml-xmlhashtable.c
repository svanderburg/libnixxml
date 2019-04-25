#include "nixxml-xmlhashtable.h"
#include "nixxml-print-nix.h"
#include "nixxml-print-xml.h"

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

typedef struct
{
    FILE *file;
    const int indent_level;
    void *userdata;
    NixXML_PrintValueFunc print_value;
}
SimpleScannerParams;

/* Nix printing */

static void scanner_nix(void *payload, void *data, const xmlChar *name)
{
    SimpleScannerParams *params = (SimpleScannerParams*)data;
    NixXML_print_attribute_nix(params->file, (const char*)name, payload, params->indent_level, params->userdata, params->print_value);
}

void NixXML_print_xml_hash_table_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const xmlHashTablePtr hash_table = (const xmlHashTablePtr)value;
    SimpleScannerParams params = { file, indent_level, userdata, print_value };
    xmlHashScan(hash_table, scanner_nix, &params);
}

void NixXML_print_xml_hash_table_nix(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    NixXML_print_attrset_nix(file, hash_table, indent_level, userdata, NixXML_print_xml_hash_table_attributes_nix, print_value);
}

/* Simple XML printing */

static void scanner_simple_xml(void *payload, void *data, const xmlChar *name)
{
    SimpleScannerParams *params = (SimpleScannerParams*)data;
    NixXML_print_simple_attribute_xml(params->file, (const char*)name, payload, params->indent_level, params->userdata, params->print_value);
}

void NixXML_print_xml_hash_table_simple_attributes_xml(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const xmlHashTablePtr hash_table = (const xmlHashTablePtr)value;
    SimpleScannerParams params = { file, indent_level, userdata, print_value };
    xmlHashScan(hash_table, scanner_simple_xml, &params);
}

void NixXML_print_xml_hash_table_simple_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    NixXML_print_attrset_xml(file, hash_table, indent_level, userdata, NixXML_print_xml_hash_table_simple_attributes_xml, print_value);
}

/* Verbose XML printing */

typedef struct
{
    FILE *file;
    const char *child_element_name;
    const char *name_property_name;
    const int indent_level;
    void *userdata;
    NixXML_PrintValueFunc print_value;
}
VerboseScannerParams;

static void scanner_verbose_xml(void *payload, void *data, const xmlChar *name)
{
    VerboseScannerParams *params = (VerboseScannerParams*)data;
    NixXML_print_verbose_attribute_xml(params->file, params->child_element_name, params->name_property_name, (const char *)name, payload, params->indent_level, params->userdata, params->print_value);
}

void NixXML_print_xml_hash_table_verbose_attributes_xml(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    const xmlHashTablePtr hash_table = (const xmlHashTablePtr)value;
    VerboseScannerParams params = { file, "elem", "name", indent_level, userdata, print_value }; // TODO: make elem, name configurable
    xmlHashScan(hash_table, scanner_verbose_xml, &params);
}

void NixXML_print_xml_hash_table_verbose_xml(FILE *file, xmlHashTablePtr hash_table, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    NixXML_print_attrset_xml(file, hash_table, indent_level, userdata, NixXML_print_xml_hash_table_verbose_attributes_xml, print_value);
}

/* Parse functionality */

void *NixXML_parse_xml_hash_table(xmlNodePtr element, void *userdata, NixXML_ParseObjectFunc parse_object)
{
    return NixXML_parse_simple_attrset(element, userdata, NixXML_create_xml_hash_table, parse_object, NixXML_insert_into_xml_hash_table);
}
