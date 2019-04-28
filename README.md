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
  desired to use Nix, e.g. as a simple infrastructure to construct a domain model
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

Installation
============
Installation of `libnixxml` is very straight forward by running the standard
Autotools build procedure:

    $ ./configure
    $ make
    $ make install

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
variables that are exposed in the build environment.

Propagating complex parameters or configurations from a Nix expression is not
always convenient, e.g. when a tool requires an XML configuration as an input
file.

To propagate complex configurations to external tools, Nix has a variety of
functions that can export (sub)expressions to other formats: `builtins.toXML`,
`builtins.toJSON`. Nix can also import JSON (`builtins.fromJSON`) and TOML
(`builtins.fromTOML`).

Nix's XML representation is basically just a serialization of the abstract syntax
tree. For example, the following Nix expression:

```nix
{
  message = "This is a test";
  tags = [ "test" "example" ];
}
```

can be strictly evaluated in XML mode by the Nix expression evaluator:

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
an external tool -- it almost always needs to be transformed to another XML
format.

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
The list elements are translated to generic sub elements.

Attribute keys may contains characters making certain element names invalid
(e.g. `<` or `>`). It is also possible to use a slightly more verbose notation
in which a generic element name is used and the `name` attribute is used for the
attribute set key:

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

In addition to generate NixXML data, this package provides functionality to
conveniently parse that XML data to construct a domain model and to print XML
and Nix representations of an application domain model.

Overview
========
The following table summarizes the concepts of this library:

Concept        | Nix expression representation | XML representation                                  | C application domain model
---------------|-------------------------------|-----------------------------------------------------|----------------------------------
value          | `"hello"`                     | `hello`                                             | `char*`
list           | `[ "hello" "bye" ]`           | `<elem>hello</elem><elem>bye</elem>`                | `void**`, linked list, ...
attribute set  | `{ a = "hello"; b = "bye"; }` | `<a>hello</a><b>bye</b>`                            | `xmlHashTablePtr`, `struct`, ...
attribute set  | `{ a = "hello"; b = "bye"; }` | `<attr name="a">hello</a><attr name="b">bye</attr>` | `xmlHashTablePtr`, `struct`, ...

The above table shows the concepts that the `NixXML` defines, and how they are
represented in the Nix expression language, XML and in a domain model of a C
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

Since the C programming language has no standardized data structures (e.g. to represent
lists or sets), the core library only provides an infrastructure to make parsing and
printing possible. The functions provide generic interfaces allowing the
infrastructure to work with a variety of data structures. Providing data
structure support is the responsibility of the user.

To demonstrate a possible data structure implementation, this package provides a
library called `libnixxml-data` that serves as an example to show how to
represent lists as pointer arrays (`void**`) and attribute sets as hash tables
(using `xmlHashTable` that is included with `libxml2`).

By using generic data structures and type annotated XML data, it is also
possible to *generically* parse and print a `NixXML` file with nested arbitrary
data structures by using `NixXML_parse_generic_*` and `NixXML_print_generic_*
functions.

Usage
=====
The libraries in this package support a variety of use cases.

- You must somehow open a XML using libxml2, and obtain the root XML element that you want to use as a basis

```C
xmlNodePtr element;
/* Open XML file and obtain root element */
char *value = NixXML_parse_value(element, NULL);
```


- Manually parse a NixXML file
- Manually pretty print an object tree

- Generic parsing of a type annotated NixXML file
- Automatically pretty print a generic object tree

Data structures
===============
- There is no standardized library for abstract data structures in C, e.g. lists, maps, trees etc.
- Each framework provide their own implementations of these abstract data structures
- To parse lists and attribute sets, you may need generalized data structures, e.g. list-like or table-like
- libnixxml provides sub library called libxxml-data that uses pointer arrays for lists and xmlHashTable for attribute sets

Implementing custom data structure support
==========================================
- you may want to integrate your own list-like or table-like data structures

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

You can also pick different names for the root, attribute and list elements, if
desired:

```bash
$ nixxml-pp -f xml \
  --root-element-name root \
  --list-element-name item \
  --attr-element-name property \
  --name-attribute-name key \
  test-ast.nix
```

License
=======
This package is [MIT licensed](https://opensource.org/licenses/MIT).
