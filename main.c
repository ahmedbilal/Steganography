#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "include/steganography.h"

int main(int argc, char ** argv){
    char image_name[128];
    printf("Enter first .png file name:\n");
    scanf("%s", image_name);
    strcat(image_name, ".png");
    if(access(image_name, F_OK) != -1){
        char image_name1[128];
        printf("Enter second .png file name:\n");
        scanf("%s", image_name1);
        strcat(image_name1, ".png");
        if(access(image_name1, F_OK) != -1){
            merge_images(image_name, image_name1);
        } else {
            printf("Could not find filename %s\n", image_name1);
        }
    } else {
        printf("Could not find filename %s\n", image_name);
    }
}
