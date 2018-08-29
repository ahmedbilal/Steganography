/******************************************************************************
 * Purpose                                                                    *
 *   This file contains the functionality and algorithm for merging text or   *
 *   images within another image.                                             *
 *                                                                            *
 * Resources                                                                  *
 *   Algorithm: https://tinyurl.com/y7hrhfxo                                  *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 28 Aug 2018               BJC        Updated print hex format.             *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/

#include <png.h>
#include <stdbool.h>
#include <string.h>

#include "include/image_handler.h"

void merge_images(char* one, char* two)
{
    Image imOne, imTwo;
        
    read_png_file(one, &imOne);
    read_png_file(two, &imTwo);
    
    RGB pixelA, pixelB;
    for(int y=0; y < imOne.height; y++){
        for(int x=0; x < imOne.width; x++){
            get_RGB_value(&imOne, &pixelA, x, y);
            printf("%02x %02x %02x\n", pixelA.r, pixelA.g, pixelA.b);
        } 
    }
}

