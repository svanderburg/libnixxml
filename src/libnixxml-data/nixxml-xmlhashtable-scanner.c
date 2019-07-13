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

#include "nixxml-xmlhashtable-scanner.h"
#include <stdlib.h>

typedef struct
{
    xmlChar **names;
    unsigned int length;
}
KeyParams;

static void scanner_keys(void *payload, void *data, const xmlChar *name)
{
    KeyParams *params = (KeyParams*)data;
    params->names = (xmlChar**)realloc(params->names, (params->length + 1) * sizeof(xmlChar*));
    params->names[params->length] = (xmlChar*)name;
    params->length++;
}

static int compare_keys(const void *l, const void *r)
{
    return xmlStrcmp(*((xmlChar**)l), *((xmlChar**)r));
}

void NixXML_xmlHashScanOrdered(xmlHashTablePtr hash_table, xmlHashScanner scanner_function, void *data)
{
    KeyParams params = { NULL, 0 };
    unsigned int i;

    xmlHashScan(hash_table, scanner_keys, &params);
    qsort(params.names, params.length, sizeof(xmlChar*), compare_keys);

    for(i = 0; i < params.length; i++)
    {
        xmlChar *name = params.names[i];
        void *payload = xmlHashLookup(hash_table, name);
        scanner_function(payload, data, name);
    }

    free(params.names);
}
