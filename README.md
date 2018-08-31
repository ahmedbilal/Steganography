# Steganography

Written in C, this application is intended to hide text and images within another image.

## Requirements

* Linux Mint
* GCC version 5.4.0
* libpng (`sudo apt install zlib1g-dev libpng-dev`)

## To Run

Run `make` to compile, and `./hidden <filename_1>[.png] <filename_2>[.png]` to execute the file.

This application uses steganography on PNG files because using JPEG compresses the images, disturbing the encoding. 

## Progress

The Issues tab will include what is left to complete.
