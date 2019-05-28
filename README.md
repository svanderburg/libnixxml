libnixxml
=========
This package provides an infrastructure for generating, parsing and pretty
printing XML data that follows a so-called `NixXML` convention.

`NixXML` provides the following features:

* `NixXML` is more readable and can be parsed more easily than XML data generated
  with the built-in XML generation facilities in the
  [Nix package manager](http://nixos.org/nix).
* `NixXML` can be conveniently converted from and into Nix expressions.
* It can also be of practical use to handle XML input data even when it is not
  desired to use Nix, e.g. as a simple infrastructure to construct domain models
  from an XML configuration and writing an XML representation of a domain model
  to disk or the standard output.

The features in this package are specifically developed for use in
[Disnix](http://github.com/svanderburg/disnix) and its complementary tools, such
as [Dynamic Disnix](http://github.com/svanderburg/dydisnix) to allow convenient
integration of deployment specifications written in the Nix expression language
and the tools that carry out deployment steps (that are not provided by the Nix
package manager), such as the activation of services and deployment planning
algorithms.

Most tools in the Disnix ecosystem are primarily Nix expression-driven, but can
also work with equivalent (and somewhat) readable XML specifications. Some
tools (such as the deployment planning tools) can also generate output in both
Nix and XML. This library makes maintaining such an infrastructure more
convenient.

Prerequisites
=============
To build the libraries from source code (that provide parsing and pretty
printing), you need:

* [libxml2](http://xmlsoft.org)

For using the generator tools, you need to install the following packages:

* [The Nix package manager](http://nixos.org/nix)
* [libxslt](http://xmlsoft.org/libxslt)

Optionally, GLib data structure integration can be enabled. For this you will
need:

* [GLib](https://developer.gnome.org/glib)

Installation
============
Installation of `libnixxml` is very straight forward by running the standard
Autotools build procedure:

    $ ./configure
    $ make
    $ make install

GLib integration can be enabled by passing the `--with-glib` parameter to the
configure script.

Background
==========
The Nix expression language is used as a configuration language for various
purposes, such as:

* Package management
* System configurations
* Service oriented systems

A configuration defined in the Nix expression language typically translates to
data that can be consumed by tools that carry out deployment steps, such as build
tools. For most tools, this can be done by translating parameters to environment
variables that are exposed to the build environment.

Propagating complex parameters or configurations from a Nix expression is not
always convenient, e.g. when a tool requires an XML configuration as an input
file.

To propagate complex configurations to external tools, Nix has a variety of
functions that can export (sub)expressions to other formats: `builtins.toXML`,
`builtins.toJSON`. Nix can also import JSON (`builtins.fromJSON`) and TOML
(`builtins.fromTOML`).

Nix's XML export function basically exposes an XML serialization of an abstract
syntax tree. For example, the following Nix expression:

```nix
{
  message = "This is a test";
  tags = [ "test" "example" ];
}
```

can be converted to XML with the `builtins.toXML` primop or strictly evaluated
in XML mode by the Nix expression evaluator:

```bash
$ nix-instantiate --eval-only --xml --strict example.nix
```

resulting in the following XML data:

```xml
<?xml version='1.0' encoding='utf-8'?>
<expr>
  <attrs>
    <attr column="3" line="2" name="message" path="/home/sander/example.nix">
      <string value="This is a test" />
    </attr>
    <attr column="3" line="3" name="tags" path="/home/sander/example.nix">
      <list>
        <string value="test" />
        <string value="example" />
      </list>
    </attr>
  </attrs>
</expr>
```

The above XML snippet is very verbose and not very practical to be consumed by
an external tool (due to its structure) -- it almost always needs to be
transformed to another XML format.

This toolset also allows you to generate (and consume) a more practical XML
representation of the Nix expression shown earlier:

```xml
<?xml version="1.0"?>
<expr>
  <message>This is a test</message>
  <tags>
    <elem>test</elem>
    <elem>example</elem>
  </tags>
</expr>
```

In the above expression the type and meta information is discarded and the
attribute set is translated to a collection of XML
sub elements in which the element names correspond to the attribute keys.
The list elements are translated to generic sub elements (the above example
uses `elem`, but any element name can be chosen).

This representation is IMO far more readable, can be more easily consumed by
external tools and is still somewhat Nix-independent.

Attribute keys may be identifiers, but can also be strings containing characters
that invalidate certain XML element names (e.g. `<` or `>`). It is also possible
to use a slightly more verbose notation in which a generic element name is used
and the `name` property is used for each attribute set key:

```xml
<?xml version="1.0"?>
<expr>
  <attr name="message">This is a test</attr>
  <attr name="tags">
    <elem>test</elem>
    <elem>example</elem>
  </attr>
</expr>
```

When an application has a static domain model it is not necessary to know any
types (e.g. this conversion can be done in the application code using the
application domain model). However, it may also be desired to construct data
structures dynamically.

For dynamic object construction, type information needs to be known. Optionally,
XML elements can be annotated with type information:

```xml
<?xml version="1.0"?>
<expr type="attrs">
  <attr name="message" type="string">This is a test</attr>
  <attr name="tags" type="list">
    <elem type="string">test</elem>
    <elem type="string">example</elem>
  </attr>
</expr>
```

The alternative XML representations are easier to read and easier to parse by a
general purpose application. It can also easily be converted back into a Nix
representation.

In addition to generating `NixXML` data, this package provides functionality to
conveniently parse `NixXML` data to construct a domain model and to print XML
and Nix representations of an application domain model.

Overview
========
The following table summarizes the concepts of this library:

Concept        | Nix expression representation | XML representation                                     | C application domain model
---------------|-------------------------------|--------------------------------------------------------|----------------------------------
value          | `"hello"`                     | `hello`                                                | `char*`
list           | `[ "hello" "bye" ]`           | `<elem>hello</elem><elem>bye</elem>`                   | `void**`, linked list, ...
attribute set  | `{ a = "hello"; b = "bye"; }` | `<a>hello</a><b>bye</b>`                               | `xmlHashTablePtr`, `struct`, ...
attribute set  | `{ a = "hello"; b = "bye"; }` | `<attr name="a">hello</attr><attr name="b">bye</attr>` | `xmlHashTablePtr`, `struct`, ...

The above table shows the concepts that the `NixXML` defines, and how they can
be represented in the Nix expression language, XML and in a domain model of a C
application.

The representations of these concepts can be translated as follows:

* To convert a raw AST XML representation of a Nix expression to NixXML, we can
  use the included XSL stylesheet or run the `nixexpr2xml` command.
* XML concepts can be parsed to a domain model in a C application by invoking
  `NixXML_parse_*` functions for the appropriate concepts and XML
  representation.
* Domain model elements can be printed as XML by invoking `NixXML_print_*_xml`
  functions.
* Domain model elements can be printed in the Nix expression language by
  invoking `NixXML_print_*_nix` functions.

Since the C programming language has no standardized data structures (e.g. to
represent lists or sets), the core library only provides an infrastructure to
make parsing and printing possible. The functions provide generic interfaces
allowing the infrastructure to work with a variety of data structures. Providing
data structure support is the responsibility of the user.

To demonstrate a possible data structure implementation, this package provides a
library called `libnixxml-data` that serves as an example to show how to
represent lists as pointer arrays (`void**`) and attribute sets as hash tables
(using `xmlHashTable` that is included with `libxml2`). This package also
provides an optional alternative implementation called `libnixxml-glib` (that
needs be enabled at build-time) that integrates with GLib.

By using generic data structures and type annotated XML data, it is also
possible to *generically* parse and print a `NixXML` file with nested arbitrary
data structures by using `NixXML_parse_generic_*` and `NixXML_print_generic_*`
functions.

Usage
=====
The libraries in this package support a variety of use cases.

Manually parsing NixXML documents
---------------------------------
The API has convenience functions to parse sections of an XML document that
follow the `NixXML` convention.

For example, the following XML document only containing a string:

```xml
<expr>hello</expr>
```

can be parsed as follows:

```C
#include <nixxml-parse.h>

xmlNodePtr element;
/* Open XML file and obtain root element */
xmlChar *value = NixXML_parse_value(element, NULL);
printf("value is: %s\n"); // value is: hello
```

The above code fragment requires the presence of an XML root element that needs
to be retrieved by using the `libxml2` API first. The `NixXML_parse_value()`
function invocation retrieves the string value from the XML element's contents.

We can also use functions to parse (nested) data structures. For example, to
parse the following XML code fragment representing an attribute set:

```xml
<expr>
  <attr name="firstName">Sander</attr>
  <attr name="lastName">van der Burg</attr>
</expr>
```

we can use the following code snippet:

```C
#include <stdlib.h>
#include <nixxml-parse.h>

xmlNodePtr element;

typedef struct
{
    xmlChar *firstName;
    xmlChar *lastName;
}
ExampleStruct;

void *create_example_struct(xmlNodePtr element, void *userdata)
{
    return calloc(1, sizeof(ExampleStruct));
}

void parse_and_insert_example_struct_member(xmlNodePtr element, void *table, const xmlChar *key, void *userdata)
{
    ExampleStruct *example = (ExampleStruct*)table;

    if(xmlStrcmp(key, (xmlChar*) "firstName") == 0)
        example->firstName = NixXML_parse_value(element, userdata);
    else if(xmlStrcmp(key, (xmlChar*) "lastName") == 0)
        example->lastName = NixXML_parse_value(element, userdata);
}

/* Open XML file and obtain root element */

ExampleStruct *example = NixXML_parse_verbose_heterogeneous_attrset(element, "attr", "name", NULL, create_example_struct, parse_and_insert_example_struct_member);
```

To parse the attribute set in the XML code fragment above (that uses a verbose
notation) and derive a struct from it, we invoke the
`NixXML_parse_verbose_heterogeneous_attrset()` function. The parameters specify
that the XML code fragment should be parsed as follows:

* It expects the name of the XML element of each attribute to be called: `attr`.
* The property that refers to the name of the attribute is called: `name`.
* To create a struct that stores the attributes in the XML file, the function:
  `create_example_struct()` will be executed that allocates memory for it and
  initializes all fields with NULL values.
* The logic that parses the attribute values and assigns them to the struct
  members is in the `parse_and_insert_example_member()` function. The
  implementation uses `NixXML_parse_value()` (as shown in the previous example)
  to parse the attribute values.

In addition to parsing values as strings and attribute sets as structs, it is
also possible to:

* Parse lists, by invoking: `NixXML_parse_list()`
* Parse uniformly typed attribute sets (in which every attribute set member has
  the same type), by invoking: `NixXML_parse_verbose_attrset()`
* Parse attribute sets using a simple XML notation for attribute sets (as
  opposed to the verbose notation): `NixXML_parse_simple_attrset()` and
  `NixXML_parse_simple_heterogeneous_attrset()`.

Manually printing data structures in XML or Nix
-----------------------------------------------
In addition to parsing `NixXML` data to construct a domain model, the inverse
process is also possible -- the API also provides convenience functions to
print an XML or Nix representation of a domain model.

For example, the following string in C:

```C
char *greeting = "Hello";
```

can be displayed as a string in the Nix expression language as follows:

```C
#include <nixxml-print-nix.h>

NixXML_print_string_nix(stdout, greeting, 0, NULL); // outputs: "Hello"
```

or as an XML document, by running:

```C
#include <nixxml-print-xml.h>

NixXML_print_open_root_tag(stdout, "expr");
NixXML_print_string_xml(stdout, greeting, 0, NULL, NULL);
NixXML_print_close_root_tag(stdout, "expr");
```

producing the following output:

```xml
<expr>Hello</expr>
```

As with the parsing infrastructure, we can also print (nested) data structures.
For example, the struct shown in the previous section:

```C
typedef struct
{
    xmlChar *firstName;
    xmlChar *lastName;
}
ExampleStruct;

ExampleStruct example = { "Sander", "van der Burg" };
```

can be printed as a Nix expression with the following code:

```C
#include <nixxml-print-nix.h>

void print_example_attributes_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    ExampleStruct *example = (ExampleStruct*)value;
    NixXML_print_attribute_nix(file, "firstName", example->firstName, indent_level, userdata, NixXML_print_string_nix);
    NixXML_print_attribute_nix(file, "lastName", example->lastName, indent_level, userdata, NixXML_print_string_nix);
}

NixXML_print_attrset_nix(stdout, &example, 0, NULL, print_example_attributes_nix, NULL);
```

The above code fragment executes the function: `NixXML_print_attrset_nix()` to
print the example struct as an attribute set. The attribute set printing
function invokes the function: `print_example_attributes_nix()` to print the
attribute set members.

The `print_example_attributes_nix()` function prints each attribute assignment.
It uses the `NixXML_print_string_nix()` function (shown in the previous example)
to print each member as a string in the Nix expression language.

The result of running the above code is the following Nix expression:

```nix
{
  "firstName" = "Sander";
  "lastName" = "van der Burg";
}
```

the same struct can be printed as XML (using the verbose notation for attribute
sets) with the following code:

```C
#include <nixxml-print-xml.h>

void print_example_attributes_xml(FILE *file, const void *value, const char *child_element_name, const char *name_property_name, const int indent_level, const char *type_property_name, void *userdata, NixXML_PrintXMLValueFunc print_value)
{
    ExampleStruct *example = (ExampleStruct*)value;
    NixXML_print_verbose_attribute_xml(file, child_element_name, name_property_name, "firstName", example->firstName, indent_level, NULL, userdata, NixXML_print_string_xml);
    NixXML_print_verbose_attribute_xml(file, child_element_name, name_property_name, "lastName", example->lastName, indent_level, NULL, userdata, NixXML_print_string_xml);
}

NixXML_print_open_root_tag(stdout, "expr");
NixXML_print_verbose_attrset_xml(stdout, &example, "attr", "name", 0, NULL, NULL, print_example_attributes_xml, NULL);
NixXML_print_close_root_tag(stdout, "expr");
```

The above code fragment uses a similar strategy as the previous example (by
invoking `NixXML_print_verbose_attrset_xml()`) to print the example struct as an
XML file using a verbose notation for attribute sets.

The attribute set members are printed by the `print_example_attributes_xml()`
function.

The result of running the above code is the following XML output:

```xml
<expr>
  <attr name="firstName">Sander</attr>
  <attr name="lastName">van der Burg</attr>
</expr>
```

In addition to printing values and attribute sets, it is also possible to:
* Print lists in Nix and XML format: `NixXML_print_list_nix()`,
  `NixXML_print_list_xml()`
* Print attribute sets in simple XML notation:
  `NixXML_print_simple_attrset_xml()`
* Print strings as `int`, `float` or `bool`: `NixXML_print_string_as_*_xml`.
* Print integers: `NixXML_print_int_xml()`
* Disable indentation by setting the `indent_level` parameter to `-1`.
* Print type annotated XML, by setting the `type_property_name` parameter to
  a string that is not `NULL`.

Using the example data structures
---------------------------------
There is no standardized library for abstract data structures in C, e.g. lists,
maps, trees etc. As a result, each framework provides their own implementations
of them.

To parse lists and attribute sets (that have arbitrary structures), you need
generalized data structures that are list-like or table-like.

`libnixxml` provides sub library called `libnixxml-data` that uses pointer
arrays for lists and `xmlHashTable` for attribute sets.

The following XML document:

```xml
<expr>
  <elem>test</elem>
  <elem>example</elem>
</expr>
```

can be parsed as a pointer array (array of strings) as follows:

```C
#include <nixxml-ptrarray.h>

xmlNodePtr element;
/* Open XML file and obtain root element */
void **array = NixXML_parse_ptr_array(element, "elem", NULL, NixXML_parse_value);
```

and printed as a Nix expression with:

```C
NixXML_print_ptr_array_nix(stdout, array, 0, NULL, NixXML_print_string_nix);
```

and as XML with:

```C
NixXML_print_open_root_tag(stdout, "expr");
NixXML_print_ptr_array_xml(stdout, array, "elem", 0, NULL, NULL, NixXML_print_string_xml);
NixXML_print_close_root_tag(stdout, "expr");
```

Similarly, there is a module that works with `xmlHashTable`s
(`nixxml-xmlhashtable.h`) providing a similar function interface as the pointer
array module.

Integrating with GLib data structures
-------------------------------------
In addition to simple example data structures, it is also possible to integrate
with data structures provided by the GLib library. The `nixxml-gptrarray.h`
allows lists to be converted to `GPtrArray`s. The `nixxml-ghashtable.h` allows
you to use attribute sets as `GHashTable`s.

The function interfaces of these modules are similar to the pointer array and
`xmlHashTable` modules of the example data structure library.

Working with generic NixXML nodes
---------------------------------
The examples so far, require the user to implement all the rules to parse and
pretty print a data structure.

By using generic data structures to represent lists and tables, type annotated
`NixXML` data and a generic `NixXML_Node` struct (that indicates what kind of
node we have, such as a value, list or attribute set) we can also automatically
parse an *entire* document by using a single function call:

```C
#include <nixxml-ptrarray.h>
#include <nixxml-xmlhashtable.h>
#include <nixxml-parse-generic.h>

xmlNodePtr element;
/* Open XML file and obtain root element */
NixXML_Node *node = NixXML_generic_parse_expr(element, "type", "name", NixXML_create_ptr_array, NixXML_create_xml_hash_table, NixXML_add_value_to_ptr_array, NixXML_insert_into_xml_hash_table, NixXML_finalize_ptr_array);
```

The above function composes a generic `NixXML_Node` object. The function
interface uses function pointers to compose lists and tables. These functions
are provided by the pointer array and `xmlHashTable` modules in the
`libnixxml-data` library.

We can also print an entire `NixXML_Node` object structure as a Nix expression:

```C
#include <nixxml-print-generic-nix.h>

NixXML_print_generic_expr_nix(stdout, node, 0, NixXML_print_ptr_array_nix, NixXML_print_xml_hash_table_nix);
```

as well as XML (using simple or verbose notation for attribute sets):

```C
#include <nixxml-print-generic-xml.h>

NixXML_print_generic_expr_verbose_xml(stdout, node, 0, "expr", "elem", "attr", "name", "type", NixXML_print_ptr_array_xml, NixXML_print_xml_hash_table_verbose_xml);
```

Command-line utilities
======================
This package also include two command-line tools.

nixexpr2xml
-----------
The `nixexpr2xml` tool can be used to transform a Nix expression into a more
application-friendly XML representation:

```bash
$ nixexpr2xml test.nix
```

By default, the tool uses the verbose notation for attribute sets. It is also
possible to use the simple (but limited) attribute style notation:

```bash
$ nixexpr2xml --attr-style simple test.nix
```

You can also enable type annotations if desired:

```bash
$ nixexpr2xml --enable-types test.nix
```

The root, attribute and list element representation uses generic XML element
names. Their names can also be adjusted, if desired:

```bash
$ nixexpr2xml --root-element-name root \
  --list-element-name item \
  --attr-element-name property \
  --name-attribute-name key \
  test.nix
```

nixxml-pp
---------
The `nixxml-pp` tool can be used to open any type annotated `NixXML` file and
pretty print its structure in Nix or XML (using simple or verbose attribute
style notation).

The following command prints a Nix representation of a data structure in
`NixXML` format:

```bash
$ nixxml-pp -f nix test-ast.nix
```

we can also use verbose XML format:

```bash
$ nixxml-pp -f xml test-ast.nix
```

or simple XML:

```bash
$ nixxml-pp -f simple-xml test-ast.nix
```

You can also pick different names for the root, attribute and list elements,
the attribute and type attribute names, if desired:

```bash
$ nixxml-pp -f xml \
  --root-element-name root \
  --list-element-name item \
  --attr-element-name property \
  --name-attribute-name key \
  --type-attribute-name mytype \
  test-ast.nix
```

License
=======
This package is [MIT licensed](https://opensource.org/licenses/MIT). The
`libnixxml-glib` sub library (when enabled) is convered by the
[LGPL v2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html) or
higher.
