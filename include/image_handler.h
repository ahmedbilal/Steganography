/*****************************************************************************
 * Purpose                                                                   *
 *   This file contains the functionality relating to reading, writing, and  *
 *   access to the images.                                                   *
 *                                                                           *
 * Resources                                                                 *
 *   Initial source code for this file provided by                           *
 *          http://zarb.org/%7Egc/html/libpng.html                           *
 *   LibPNG                                                                  *
 *                                                                           *
 * Date          Issue No.   Author     Description                          *
 * --------------------------------------------------------------------------*
 * 26 Aug 2018               BJC        Initial commit.                      *
 *                                                                           *
 *****************************************************************************/

#define PNG_DEBUG 3
#include <png.h>
#include <stdbool.h>

typedef struct RGB_t{
    int r;
    int g;
    int b;
} RGB;

typedef struct Image_t{
    int         height;
    int         width;
    png_byte    color_type;
    png_byte    bit_depth;
    png_structp png_ptr;
    png_infop   info_ptr;
    int         number_of_passes;
    png_bytep * row_pointers;
} Image;

bool read_png_file(char* file_name, Image* image);
void write_png_file(char* file_name, Image* image);
void get_RGB_value(Image* image, RGB* rgb, int x, int y);
