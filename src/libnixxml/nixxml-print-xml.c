#include "nixxml-print-xml.h"

void NixXML_print_string_xml(FILE *file, const void *value, const int indent_level, void *userdata)
{
    const char *string_value = (const char*)value;
    unsigned int i = 0;

    while(string_value[i] != '\0')
    {
        char c = string_value[i];

        switch(c)
        {
            case '<':
                fprintf(file, "&lt;");
                break;
            case '>':
                fprintf(file, "&gt;");
                break;
            default:
                fputc(c, file);
        }

        i++;
    }
}

void NixXML_print_value_xml(FILE *file, const void *value, const int indent_level, void *userdata)
{
    NixXML_print_string_xml(file, value, indent_level, userdata);
}

void NixXML_print_int_xml(FILE *file, const void *value, const int indent_level, void *userdata)
{
    fprintf(file, "%d", *((int*)value));
}

void NixXML_print_list_element_xml(FILE *file, const char *child_element_name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    fprintf(file, "\n");
    NixXML_print_indentation(file, indent_level);
    fprintf(file, "<%s>", child_element_name);
    print_value(file, value, indent_level, userdata);
    fprintf(file, "</%s>", child_element_name);
}

void NixXML_print_list_xml(FILE *file, const void *list, const char *child_element_name, const int indent_level, void *userdata, NixXML_PrintXMLMembersFunc print_list_elements, NixXML_PrintValueFunc print_value)
{
    print_list_elements(file, child_element_name, list, indent_level + 1, userdata, print_value);
    fprintf(file, "\n");
    NixXML_print_indentation(file, indent_level);
}

void NixXML_print_simple_attribute_xml(FILE *file, const char *name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    fprintf(file, "\n");
    NixXML_print_indentation(file, indent_level);
    fprintf(file, "<%s>", name);
    print_value(file, value, indent_level, userdata);
    fprintf(file, "</%s>", name);
    NixXML_print_indentation(file, indent_level);
}

void NixXML_print_verbose_attribute_xml(FILE *file, const char *child_element_name, const char *name_property_name, const char *name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    fprintf(file, "\n");
    NixXML_print_indentation(file, indent_level);
    fprintf(file, "<%s %s=\"", child_element_name, name_property_name);
    NixXML_print_string_xml(file, name, indent_level, userdata);
    fprintf(file, "\">");
    print_value(file, value, indent_level, userdata);
    fprintf(file, "</%s>", child_element_name);
    NixXML_print_indentation(file, indent_level);
}

void NixXML_print_attrset_xml(FILE *file, const void *table, const int indent_level, void *userdata, NixXML_PrintMembersFunc print_attributes, NixXML_PrintValueFunc print_value)
{
    print_attributes(file, table, indent_level + 1, userdata, print_value);
    fprintf(file, "\n");
    NixXML_print_indentation(file, indent_level);
}
