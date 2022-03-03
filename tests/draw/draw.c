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

#include "drawspec.h"
#include <stdio.h>
#include <string.h>

static void print_usage(const char *command)
{
    printf("Usage: %s OUTPUT_TYPE CONFIG_XML\n", command);
}

int main(int argc, char *argv[])
{
    /* Parse command-line parameters */

    char *output_type;
    char *config_file;

    if(argc > 1)
        output_type = argv[1];
    else
        output_type = NULL;

    if(argc > 2)
        config_file = argv[2];
    else
        config_file = NULL;

    if(output_type == NULL || config_file == NULL)
    {
        fprintf(stderr, "Not all mandatory input parameters provided!\n\n");
        print_usage(argv[0]);
        return 1;
    }
    else
    {
        /* Open and check input model */

        DrawSpec *drawSpec = open_drawspec(config_file);
        int exit_status;

        if(drawSpec == NULL)
        {
            fprintf(stderr, "Cannot open draw spec file!\n");
            return 1;
        }
        else if(check_drawspec(drawSpec))
        {
            /* Execute the desired operation */

            if(strcmp(output_type, "nix") == 0)
                print_drawspec_nix(stdout, drawSpec, 0, NULL);
            else if(strcmp(output_type, "xml") == 0)
                print_drawspec_xml(stdout, drawSpec, 0, NULL, NULL);
            else
                draw_image_from_drawspec(stdout, drawSpec);

            exit_status = 0;
        }
        else
            exit_status = 1;

        delete_drawspec(drawSpec);
        return exit_status;
    }
}
