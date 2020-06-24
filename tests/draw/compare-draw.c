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

#include "drawspec.h"
#include <stdio.h>
#include <string.h>

static void print_usage(const char *command)
{
    printf("Usage: %s CONFIG_XML CONFIG_XML\n", command);
}

int main(int argc, char *argv[])
{
    /* Parse command-line parameters */

    char *config_file1, *config_file2;

    if(argc > 1)
        config_file1 = argv[1];
    else
        config_file1 = NULL;

    if(argc > 2)
        config_file2 = argv[2];
    else
        config_file2 = NULL;

    if(config_file1 == NULL || config_file2 == NULL)
    {
        fprintf(stderr, "Not all mandatory input parameters provided!\n\n");
        print_usage(argv[0]);
        return 1;
    }
    else
    {
        /* Open and check input model */

        DrawSpec *drawSpec1 = open_drawspec(config_file1);
        DrawSpec *drawSpec2 = open_drawspec(config_file2);
        int exit_status;

        if(drawSpec1 == NULL || drawSpec2 == NULL)
        {
            fprintf(stderr, "Cannot open draw spec files!\n");
            exit_status = 1;
        }
        else if(check_drawspec(drawSpec1) || check_drawspec(drawSpec2))
            exit_status = !compare_drawspecs(drawSpec1, drawSpec2); /* Do the comparisons */
        else
            exit_status = 1;

        delete_drawspec(drawSpec1);
        delete_drawspec(drawSpec2);

        return exit_status;
    }
}
