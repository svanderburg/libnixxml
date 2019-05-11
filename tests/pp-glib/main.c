#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "pretty-print-glib.h"

static void print_usage(const char *command)
{
    printf("Usage: %s -f OUTPUT_FORMAT CONFIG_XML\n\n", command);

    puts(
    "The command `nixxml-pp' parses a provided XML file using conventions that\n"
    "are similar to Nix expressions and pretty prints it in a desired output\n"
    "format.\n"

    "\nOptions:\n"
    "  -f, --format=OUTPUT_FORMAT     Specifies the desired output format ('nix',\n"
    "                                 'xml' or 'simple-xml').\n"
    "      --disable-indent           Do not use indentation\n"
    "  -h, --help                     Shows the usage of this command to the user"
    );

    puts(
    "\nXML Options:\n"
    "      --disable-types            Disable type annotations\n"
    "      --root-element-name=NAME   Name of the root element (defaults to: expr)\n"
    "      --list-element-name=NAME   Name of a list element (defaults to: elem)\n"
    "      --attr-element-name=NAME   Name of a attribute (defaults to: attr)\n"
    "      --name-property-name=NAME  Name of the attribute name property (defaults to:\n"
    "                                 name)\n"
    "      --type-property-name=NAME  Name of the type property (defaults to: type)"
    );
}

int main(int argc, char *argv[])
{
    /* Declarations */
    int c, option_index = 0;
    struct option long_options[] =
    {
        {"format", required_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {"disable-indent", no_argument, 0, 'i'},
        {"disable-types", no_argument, 0, '0'},
        {"root-element-name", required_argument, 0, '1'},
        {"list-element-name", required_argument, 0, '2'},
        {"attr-element-name", required_argument, 0, '3'},
        {"name-property-name", required_argument, 0, '4'},
        {"type-property-name", required_argument, 0, '5'},
        {0, 0, 0, 0}
    };
    FormatType format = 0;
    int indent_level = 0;
    char *root_element_name = "expr";
    char *list_element_name = "list";
    char *attr_element_name = "attr";
    char *name_property_name = "name";
    char *type_property_name = "type";

    /* Parse command-line options */
    while((c = getopt_long(argc, argv, "f:h", long_options, &option_index)) != -1)
    {
        switch(c)
        {
            case 'f':
                if(strcmp(optarg, "nix") == 0)
                    format = FORMAT_NIX;
                else if(strcmp(optarg, "xml") == 0)
                    format = FORMAT_VERBOSE_XML;
                else if(strcmp(optarg, "simple-xml") == 0)
                    format = FORMAT_SIMPLE_XML;
                break;
            case 'i':
                indent_level = -1;
                break;
            case '0':
                type_property_name = NULL;
                break;
            case '1':
                root_element_name = optarg;
                break;
            case '2':
                list_element_name = optarg;
                break;
            case '3':
                attr_element_name = optarg;
                break;
            case '4':
                name_property_name = optarg;
                break;
            case '5':
                type_property_name = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    /* Validate options */
    if(format == 0)
    {
        fprintf(stderr, "No valid output format specified!\n");
        return 1;
    }

    /* Validate non-options */
    if(optind >= argc)
    {
        fprintf(stderr, "An infrastructure Nix expression has to be specified!\n");
        return 1;
    }

    /* Execute operation */
    return pretty_print_file_glib(argv[optind], format, indent_level, root_element_name, list_element_name, attr_element_name, name_property_name, type_property_name);
}