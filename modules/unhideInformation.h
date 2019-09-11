#ifndef __UNHIDEINFO__
#define __UNHIDEINFO__


#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "libbmp.h"






void start_unhideInformation();
BMP* receiveSourceImage(char file[]);
void printTextStego(char* word[], int WordLength);
uint8_t* receiveImage();
int receiveLSBAmount();
void start_unhideInformation();
uint32_t getSize (char* restChar, uint64_t image_size);
//void separateInBits(char* bits_character, int k, int pixelBit_index);
#endif
