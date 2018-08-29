/******************************************************************************
 * Purpose                                                                    *
 *   This file provides common utilities.                                     *
 *                                                                            *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 28 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/util.h"

/******************************************************************************
 * Purpose                                                                    *
 *   This function appends the PNG extension to the end of the provided name  *
 *   only as necessary.                                                       *
 *                                                                            *
 * @param[in/out]  name     The name of the file.                             *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 28 Aug 2018   3            BJC        Separated into a function.           *
 *                                                                            *
 *****************************************************************************/
char* is_valid_name(char **name)
{
    const char ch = '.';
    char *ext;
    ext = strchr(*name, ch);

    if(ext == NULL) 
        strcat(*name, ".png"); 

    ext = strchr(*name, ch);

    if(strncmp(ext, ".png", 4)){ 
        printf("Invalid extension for file %s.\n", *name);
        return NULL;
    }

    if(access(*name, F_OK) == -1){
        printf("File %s not found.\n", *name);
        return NULL;
    }
    
    return *name;
}



