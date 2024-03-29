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

#include "nixxml-generate-env-generic.h"

xmlChar *NixXML_generate_env_value_from_node(const void *value, void *userdata)
{
    const NixXML_Node *node = (const NixXML_Node*)value;
    NixXML_GenerateEnvGenericParams *params = (NixXML_GenerateEnvGenericParams*)userdata;

    switch(node->type)
    {
        case NIX_XML_TYPE_STRING:
        case NIX_XML_TYPE_PATH:
        case NIX_XML_TYPE_INT:
        case NIX_XML_TYPE_FLOAT:
            return NixXML_generate_env_value_from_string(node->value, userdata);
        case NIX_XML_TYPE_BOOL:
            return NixXML_generate_env_value_from_bool_string(node->value, userdata);
        case NIX_XML_TYPE_ATTRSET:
            return NULL;
        case NIX_XML_TYPE_LIST:
            return params->generate_list_env(node->value, userdata, NixXML_generate_env_value_from_node);
    }

    return NULL;
}

xmlChar *NixXML_generate_env_generic(const NixXML_Node *node, NixXML_GenerateListEnvValueFunc generate_list_env)
{
    NixXML_GenerateEnvGenericParams params;
    params.generate_list_env = generate_list_env;
    return NixXML_generate_env_value_from_node(node, &params);
}
