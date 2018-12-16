#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char** argv) {
    FILE *file = fopen("main.cpp", "r");
    if (!file) {
        return 1;
    }

    // read one line
    // char line[256];
    // fgets(line, 256, file);
    // printf("%s\n", line);
    // return 0;

    // read the whole file
    // fseek(file, 0, SEEK_END);
    // int fileSize = ftell(file);
    // rewind(file);
    // char *fileText = new char[fileSize];
    // fread(fileText, sizeof(char), fileSize, file);
    // printf("%s\n", fileText);

    // fclose(file);

    // write to a file
    // file = fopen("newfile.txt", "a+");
    // fwrite("abcd",  sizeof(char), 4, file);
    // fclose(file);

    // getting rgb values for an image (iamge decompressor) THIS DOES NOT WORK
    int w, h, n;
    unsigned char *imageData = stbi_load("img_lights.jpg", &w, &h, &n, 4);
    file = fopen("imageData.txt", "w");
    unsigned char *pixelData = new unsigned char[w * h * 4];
    for (int i = 0; i < w * h * 4; i++) {
        pixelData[i] = (int)imageData[i];
    }
    fwrite(imageData, sizeof(char), w * h * 4, file);
    fclose(file);
}