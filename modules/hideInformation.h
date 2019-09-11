#ifndef __HIDEINFO__
#define __HIDEINFO__

#include <stdlib.h>
#include <stddef.h>
#include "libbmp.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h> 
#include <string.h>

char* make_textBuffer(FILE* file_stream, int* file_size);

int retMaxKCap(int k);

void start_hideInformation();

void separateInBits(uint8_t* res, uint8_t character, int k, int pixelBit_index);

int receiveTextVariables(char* file[], FILE** text);

int receiveLSBAmountHideInfo();

BMP* receiveSourceImageToHide(char* file[]);

char* getRest(char* rest, uint64_t r);

char* make_textLenght(uint64_t  lenght, uint64_t  size);

#endif
