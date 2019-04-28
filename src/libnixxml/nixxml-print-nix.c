#include "nixxml-print-nix.h"

void NixXML_print_string_nix(FILE *file, const void *value, const int indent_level, void *userdata)
{
    const char *string_value = (const char*)value;
    unsigned int i = 0;

    fputc('\"', file);

    while(string_value[i] != '\0')
    {
        char c = string_value[i];

        switch(c)
        {
            case '\"':
            case '\\':
                fputc('\\', file);
                fputc(c, file);
                break;
            case '\n':
                fputc('\\', file);
                fputc('n', file);
                break;
            default:
                fputc(c, file);
        }

        i++;
    }

    fputc('\"', file);
}

void NixXML_print_value_nix(FILE *file, const void *value, const int indent_level, void *userdata)
{
    fprintf(file, "%s", (const char*)value);
}

void NixXML_print_int_nix(FILE *file, const void *value, const int indent_level, void *userdata)
{
    fprintf(file, "%d", *((int*)value));
}

void NixXML_print_list_element_nix(FILE *file, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    NixXML_print_indentation(file, indent_level);
    print_value(file, value, indent_level, userdata);
    fprintf(file, "\n");
}

void NixXML_print_list_nix(FILE *file, const void *list, const int indent_level, void *userdata, NixXML_PrintMembersFunc print_list_elements, NixXML_PrintValueFunc print_value)
{
    fprintf(file, "[\n");
    print_list_elements(file, list, indent_level >= 0 ? indent_level + 1 : indent_level, userdata, print_value);
    NixXML_print_indentation(file, indent_level);
    fprintf(file, "]");
}

void NixXML_print_attribute_nix(FILE *file, const char *name, const void *value, const int indent_level, void *userdata, NixXML_PrintValueFunc print_value)
{
    NixXML_print_indentation(file, indent_level);
    NixXML_print_string_nix(file, name, indent_level, userdata);
    fprintf(file, " = ");
    print_value(file, value, indent_level, userdata);
    fprintf(file, ";\n");
}

void NixXML_print_attrset_nix(FILE *file, const void *table, const int indent_level, void *userdata, NixXML_PrintMembersFunc print_attributes, NixXML_PrintValueFunc print_value)
{
    fprintf(file, "{\n");
    print_attributes(file, table, indent_level >= 0 ? indent_level + 1 : indent_level, userdata, print_value);
    NixXML_print_indentation(file, indent_level);
    fprintf(file, "}");
}
