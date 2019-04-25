#include "nixxml-parse-generic.h"

static void *parse_generic_value(xmlNodePtr element, NixXML_Type type, void *userdata)
{
    NixXML_Object *obj = (NixXML_Object*)malloc(sizeof(NixXML_Object));
    obj->type = type;
    obj->value = NixXML_parse_value(element, userdata);
    return obj;
}

void *NixXML_parse_generic_string(xmlNodePtr element, void *userdata)
{
    return parse_generic_value(element, NIX_XML_TYPE_STRING, userdata);
}

void *NixXML_parse_generic_int(xmlNodePtr element, void *userdata)
{
    return parse_generic_value(element, NIX_XML_TYPE_INT, userdata);
}

void *NixXML_parse_generic_float(xmlNodePtr element, void *userdata)
{
    return parse_generic_value(element, NIX_XML_TYPE_FLOAT, userdata);
}

void *NixXML_parse_generic_bool(xmlNodePtr element, void *userdata)
{
    return parse_generic_value(element, NIX_XML_TYPE_BOOL, userdata);
}

void *NixXML_create_generic_list(xmlNodePtr element, void *userdata)
{
    NixXML_ParseExprParams *params = (NixXML_ParseExprParams*)userdata;

    NixXML_Object *obj = (NixXML_Object*)malloc(sizeof(NixXML_Object));
    obj->type = NIX_XML_TYPE_LIST;
    obj->value = params->create_list(element, userdata);
    return obj;
}

void *NixXML_create_generic_attrset(xmlNodePtr element, void *userdata)
{
    NixXML_ParseExprParams *params = (NixXML_ParseExprParams*)userdata;

    NixXML_Object *obj = (NixXML_Object*)malloc(sizeof(NixXML_Object));
    obj->type = NIX_XML_TYPE_ATTRSET;
    obj->value = params->create_table(element, userdata);
    return obj;
}

void NixXML_add_element_to_generic_list(void *list, void *value, void *userdata)
{
    NixXML_ParseExprParams *params = (NixXML_ParseExprParams*)userdata;

    NixXML_Object *obj = (NixXML_Object*)list;
    params->add_element(obj->value, value, userdata);
}

void NixXML_insert_object_into_generic_attrset(void *table, const xmlChar *key, void *value, void *userdata)
{
    NixXML_ParseExprParams *params = (NixXML_ParseExprParams*)userdata;

    NixXML_Object *obj = (NixXML_Object*)table;
    params->insert_object(obj->value, key, value, userdata);
}

void *NixXML_finalize_generic_list(void *list, void *userdata)
{
    NixXML_ParseExprParams *params = (NixXML_ParseExprParams*)userdata;

    NixXML_Object *obj = (NixXML_Object*)list;
    obj->value = params->finalize_list(obj->value, userdata);
    return obj;
}

void *NixXML_parse_expr(xmlNodePtr element, void *userdata)
{
    NixXML_ParseExprParams *params = (NixXML_ParseExprParams*)userdata;

    xmlChar *type = NixXML_find_property(element, params->type_property_name);

    if(type == NULL)
    {
        fprintf(stderr, "Element: %s has no type annotation!\n", element->name);
        return NULL;
    }
    else if(xmlStrcmp(type, (xmlChar*) "list") == 0)
        return NixXML_parse_list(element, NULL, params, NixXML_create_generic_list, NixXML_add_element_to_generic_list, NixXML_parse_expr, NixXML_finalize_generic_list);
    else if(xmlStrcmp(type, (xmlChar*) "attrs") == 0)
        return NixXML_parse_verbose_attrset(element, NULL, params->name_property_name, params, NixXML_create_generic_attrset, NixXML_parse_expr, NixXML_insert_object_into_generic_attrset);
    else if(xmlStrcmp(type, (xmlChar*) "string") == 0)
        return NixXML_parse_generic_string(element, userdata);
    else if(xmlStrcmp(type, (xmlChar*) "int") == 0)
        return NixXML_parse_generic_int(element, userdata);
    else if(xmlStrcmp(type, (xmlChar*) "float") == 0)
        return NixXML_parse_generic_float(element, userdata);
    else if(xmlStrcmp(type, (xmlChar*) "bool") == 0)
        return NixXML_parse_generic_bool(element, userdata);
    else
    {
        fprintf(stderr, "Unknown type encountered: %s\n", type);
        return NULL;
    }
}

NixXML_Object *NixXML_generic_parse_expr(xmlNodePtr element, const char *type_property_name, const char *name_property_name, NixXML_CreateObjectFunc create_list, NixXML_CreateObjectFunc create_table, NixXML_AddElementFunc add_element, NixXML_InsertObjectFunc insert_object, NixXML_FinalizeListFunc finalize_list)
{
    NixXML_ParseExprParams params = { type_property_name, name_property_name, create_list, create_table, add_element, insert_object, finalize_list };
    return NixXML_parse_expr(element, &params);
}
