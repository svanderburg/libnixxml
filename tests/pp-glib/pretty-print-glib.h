#ifndef __NIXXML_PRETTY_PRINT_GLIB_H
#define __NIXXML_PRETTY_PRINT_GLIB_H

typedef enum
{
    FORMAT_NIX = 1,
    FORMAT_SIMPLE_XML = 2,
    FORMAT_VERBOSE_XML = 3
}
FormatType;

int pretty_print_file_glib(const char *config_file, FormatType format, int indent_level, const char *root_element_name, const char *list_element_name, const char *attr_element_name, const char *name_property_name, const char *type_property_name);

#endif
