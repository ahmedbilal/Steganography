#include <stdbool.h>
#include <png.h>
#include <string.h>
#include "include/image_handler.h"

void merge_images(char* one, char* two)
{
    Image imOne, imTwo;
        
    bool result = read_png_file(one, &imOne);
    if(result)
        return;
    //read_png_file(two, imTwo);
    
    RGB pixelA, pixelB;
    for(int y=0; y < imOne.height; y++){
        for(int x=0; x < imOne.width; x++){
            get_RGB_value(&imOne, &pixelA, x, y);
            printf("%04x %04x %04x\n", pixelA.r, pixelA.g, pixelA.b);
        } 
    }
}

