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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/image_handler.h"

void abort_(const char * s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}

/******************************************************************************
 *                                                                            *
 * Writes the PNG image out to a file.                                        *
 *                                                                            *
 * @param[in]  file_name  Name of the file to read from.                      *
 * @param[out] image      Image to read from the file.                        *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/
bool read_png_file(char* file_name, Image* image)
{
    if(image == NULL)
        return -1;
    char header[8];    // 8 is the maximum size that can be checked
    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");

    if (!fp)
        abort_("[read_png_file] File %s could not be opened for reading", 
                file_name);
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
        abort_("[read_png_file] File %s is not recognized as a PNG file", 
                file_name);

    /* initialize stuff */
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                            NULL, NULL, NULL);

    if (!image->png_ptr)
        abort_("[read_png_file] png_create_read_struct failed");

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr)
        abort_("[read_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(image->png_ptr)))
        abort_("[read_png_file] Error during init_io");

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);

    png_read_info(image->png_ptr, image->info_ptr);

    image->width      = png_get_image_width (image->png_ptr, image->info_ptr);
    image->height     = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type  (image->png_ptr, image->info_ptr);
    image->bit_depth  = png_get_bit_depth   (image->png_ptr, image->info_ptr);

    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(image->png_ptr)))
        abort_("[read_png_file] Error during read_image");

    image->row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * image->height);
    for (int y=0; y<image->height; y++)
        image->row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(
                                                        image->png_ptr,
                                                        image->info_ptr));

    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
}

/******************************************************************************
 *                                                                            *
 * Writes the PNG image out to a file.                                        *
 *                                                                            *
 * @param[in]  file_name  Name of the file to create.                         *
 * @param[in]  image      Image to save to the file.                          *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/
void write_png_file(char* file_name, Image* image)
{
    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp)
        abort_("[write_png_file] File %s could not be opened for writing", file_name);

    /* initialize stuff */
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr)
        abort_("[write_png_file] png_create_write_struct failed");

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr)
        abort_("[write_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(image->png_ptr)))
        abort_("[write_png_file] Error during init_io");

    png_init_io(image->png_ptr, fp);

    /* write header */
    if (setjmp(png_jmpbuf(image->png_ptr)))
        abort_("[write_png_file] Error during writing header");

    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(image->png_ptr, image->info_ptr);

    /* write bytes */
    if (setjmp(png_jmpbuf(image->png_ptr)))
        abort_("[write_png_file] Error during writing bytes");

    png_write_image(image->png_ptr, image->row_pointers);

    /* end write */
    if (setjmp(png_jmpbuf(image->png_ptr)))
        abort_("[write_png_file] Error during end of write");

    png_write_end(image->png_ptr, NULL);

    /* cleanup heap allocation */
    for(int y=0; y<image->height; y++)
        free(image->row_pointers[y]);
    free(image->row_pointers);
    free(image);
    fclose(fp);
}

/******************************************************************************
 *                                                                            *
 * Returns the pixel of an image at the coordinates x and y.                  *
 *                                                                            *
 * @param[in]  image  Image to retrieve the pixel.                            *
 * @param[out] rgb    RGB values of the pixel.                                *
 * @param[in]  x      X coordinate of the pixel.                              *
 * @param[in]  y      Y coordinate of the pixel.                              *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/
void get_RGB_value(Image* image, RGB* rgb, int x, int y)
{
    png_byte* pixel = &(image->row_pointers[y][x*4]);
    rgb->r = pixel[0];
    rgb->g = pixel[1];
    rgb->b = pixel[2];
    return;
}
