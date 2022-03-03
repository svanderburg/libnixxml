/*
 * Copyright (c) 2019-2020 Sander van der Burg
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

/**
 * @file
 * @brief Data-XmlHashTable-Scanner module
 * @defgroup Data-XmlHashTable-Scanner
 * @{
 */

#ifndef __NIXXML_XMLHASHTABLE_SCANNER_H
#define __NIXXML_XMLHASHTABLE_SCANNER_H
#include <libxml/parser.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A function with a similar purpose as xmlHashScan() but traverses the keys
 * in a deterministic order.
 *
 * @param hash_table The hash table
 * @param scanner_function Scanner function for each item in the hash table
 * @param data Extra data passed to the scanner function
 */
void NixXML_xmlHashScanOrdered(xmlHashTablePtr hash_table, xmlHashScanner scanner_function, void *data);

#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */
