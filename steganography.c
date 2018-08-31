/******************************************************************************
 * Purpose                                                                    *
 *   This file contains the functionality and algorithm for merging text or   *
 *   images within another image.                                             *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 31 Aug 2018               BJC        Added functionality to hide text.     *
 * 28 Aug 2018               BJC        Updated print hex format.             *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/

#include <png.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "include/image_handler.h"

/******************************************************************************
 *                                                                            *
 * This function hides an image inside of another image.                      *
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
void hide_image(char* destination, char* source)
{
    Image imOne, imTwo;
        
    read_png_file(destination, &imOne);
    read_png_file(source,      &imTwo);
    
    RGB pixelA, pixelB;
    for(int y=0; y < imOne.height; y++){
        for(int x=0; x < imOne.width; x++){
            // Get pixels.
            get_RGB_value(&imOne, &pixelA, x, y);
            get_RGB_value(&imTwo, &pixelB, x, y);
            
            printf("%02x %02x %02x\n", pixelA.r, pixelA.g, pixelA.b);
            printf("%02x %02x %02x\n", pixelB.r, pixelB.g, pixelB.b);
        } 
    }
}

/******************************************************************************
 *                                                                            *
 * This function encodes a value into an RGB pixel. Any changes to how the    *
 * text is hidden in an image is done here.                                   *
 *                                                                            *
 * @param[in/out] sourcePixel Pixel in which to encode the value.             *
 * @param[in]     value       Value to encode into the pixel.                 *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/
void encode_text(RGB *sourcePixel, int value)
{
    sourcePixel->r = (sourcePixel->r & 0xFC) | ((value & 0xC0) >> 6);
    sourcePixel->g = (sourcePixel->g & 0xF8) | ((value & 0x38) >> 3);
    sourcePixel->b = (sourcePixel->b & 0xF8) | ((value & 0x07)     );
}

/******************************************************************************
 *                                                                            *
 * This function encodes a value into an RGB pixel. Any changes to how the    *
 * text is hidden in an image is done here.                                   *
 *                                                                            *
 * Its functionality must correspond with the encode_text function to be able *
 * to undo it.                                                                *
 *                                                                            *
 * @param[in/out] sourcePixel Pixel that contains the hidden value.           *
 *                                                                            *
 * @return The hidden value.                                                  *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/
int decode_text(RGB *sourcePixel)
{
    return ((sourcePixel->r & 0x03) << 6) | ((sourcePixel->g & 0x07) << 3) |
           ((sourcePixel->b & 0x07));
}

/******************************************************************************
 *                                                                            *
 * This function hides the text inside of an image, and then saves the result *
 * out to a PNG file that preserves the encoding.                             *
 *                                                                            *
 * @param[in] image   The filename of the PNG image to contain the text.      *
 * @param[in] text    The text to hide in the PNG image.                      *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/
void hide_text(char *image, char *text)
{
    Image imOne;
    read_png_file(image, &imOne);
       
    // If the message is too big for the picture.
    int len = strlen(text);
    if(len > imOne.width * imOne.height){
        printf("Message is too long.\n");
        return;
    }

    // If the string length is too big to encode into one pixel.
    // The longest string to encode would be 16,777,215 characters long.
    // According to people on Quora, the length of a novel is in the orders
    // of magnitude of 100K characters. +16M characters is a tad much.
    char *strlen_len = malloc(sizeof(char) * len);
    sprintf(strlen_len, "%d", len);
    if(strlen(strlen_len) > 0xFFFFFF){
        printf("Message is too long.\n");
        free(strlen_len);
        return;
    }
    free(strlen_len);
    
    // Write string length to the first pixel.
    RGB sourcePixel;
    get_RGB_value(&imOne, &sourcePixel, 0, 0);
    encode_text(&sourcePixel, len);
    set_RGB_value(&imOne, &sourcePixel, 0, 0);

    // Write the text to the picture.
    for(int y = 0, charIndex = 0; y < imOne.height && charIndex < len; y++){
        for(int x=(y == 0); x < imOne.width && charIndex < len; x++){
            get_RGB_value(&imOne, &sourcePixel, x, y);
            encode_text(&sourcePixel, text[charIndex++]);
            set_RGB_value(&imOne, &sourcePixel, x, y);
        }
    }

    write_png_file("result.png", &imOne);
    printf("File written!\n");
}

/******************************************************************************
 *                                                                            *
 * This function extracts the text from the image.                            *
 *                                                                            *
 * @param[in] image   The filename of the PNG image to contain the text.      *
 *                                                                            *
 * Date          Issue No.   Author     Description                           *
 * -------------------------------------------------------------------------- *
 * 26 Aug 2018               BJC        Initial commit.                       *
 *                                                                            *
 *****************************************************************************/
void unhide_text(char *image)
{
    Image imOne;

    read_png_file(image, &imOne);
    RGB sourcePixel;

    // Write string length to the first character.
    get_RGB_value(&imOne, &sourcePixel, 0, 0);
    int len = decode_text(&sourcePixel);
    char *message = malloc(sizeof(char) * len + 1);
    
    // Get the actual message.
    for(int y = 0, charIndex = 0; y < imOne.height && charIndex < len; y++){
        for(int x=(y == 0); x < imOne.width && charIndex < len; x++){
            get_RGB_value(&imOne, &sourcePixel, x, y);
            message[charIndex++] = decode_text(&sourcePixel);
        }
    }
    message[len] = '\0';
    
    // Display the result.
    printf("The string is: '%s'\n", message);
    
    // Make sure to cleanup any memory allocations.
    free(message);
}
