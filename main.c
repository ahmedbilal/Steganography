/******************************************************************************
 * Purpose                                                                    *
 *   This file executes the steganography application. The functionality of   *
 *   reading, writing, and encoding the images will be done in other files,   *
 *   so that this file only accesses the functionality at a high level, like  *
 *   a black box.                                                             *
 *                                                                            *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 28 Aug 2018   3           BJC        Moved reading in a file to utils.h    *
 * 27 Aug 2018   3           DMU        Added command line input for files    *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/steganography.h"
#include "include/util.h"

int main(int argc, char ** argv)
{
    const char * usage_msg = "\
Usage: ./hidden option [infile] [outfile]\n\n \
    option can be -h or -u\n \
                  -h: hide\n \
                  -u: unhide\n\n \
    infile: input file\n \
    outfile: output file\n\n \
    Examples:\n \
        ./hidden -u input.png\n \
        ./hidden -h input.png output.png\n";

    char option = 0;
    char * input_file = NULL;
    char * output_file = NULL;


    if (argc < 2) {
        printf("%s", usage_msg);
        return -1;
    }
    option = argv[1][1];

    if (option == 'h') {
        if (argc < 4) {
            printf("%s", usage_msg);
            return -1;
        }
    }
    else if (option == 'u') {
        if (argc < 3) {
            printf("%s", usage_msg);
            return -1;
        }
    }

    input_file  = malloc(strlen(argv[2]) + 1);
    strcpy(input_file, argv[2]);

    if (option == 'h'){
        output_file = malloc(strlen(argv[3]) + 1);
        strcpy(output_file, argv[3]);
    }
    
    if(is_valid_name(&input_file))
    {
        if (option == 'u') {
            unhide_text(input_file);
            free(input_file);
        }
        else if (option == 'h') {
            char *text = "Hello I am ABK";
            hide_text(input_file, output_file, text);
            printf("Text hidden!\n");
            free(input_file);
            free(output_file);
        }
        
    }
}

