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
    if(argc != 3)
    {
        printf("Invalid number of arguments.\n");
        return -1;
    }
    
    char *imOne = malloc(sizeof(argv[1]));
    char *imTwo = malloc(sizeof(argv[2]));
    
    strcpy(imOne, argv[1]);
    strcpy(imTwo, argv[2]);
    
    if(is_valid_name(&imOne))
    {
        char *text = "Hello world!";
        hide_text(imOne, text);
        printf("Text hidden!\n");
        unhide_text("result.png");
    }
}

