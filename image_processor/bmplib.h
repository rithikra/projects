#ifndef BMPLIB_H
#define BMPLIB_H

const int SIZE = 256;
const int RGB = 3;
int readGSBMP(const char filename[], unsigned char image[][SIZE]);
int writeGSBMP(const char filename[], unsigned char outputImage[][SIZE]);
void showGSBMP(unsigned char inputImage[][SIZE]);
int readRGBBMP(const char filename[], unsigned char inputImage[][SIZE][RGB]);
int writeRGBBMP(const char filename[], unsigned char outputImage[][SIZE][RGB]);
void showRGBBMP(unsigned char inputImage[][SIZE][RGB]);

#endif
