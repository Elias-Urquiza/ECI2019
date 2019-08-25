#ifndef __HIDEINFO__
#define __HIDEINFO__

#include <stdlib.h>
#include <stddef.h>
#include "libbmp.h"
#include <stdio.h>
#include <stdbool.h>

char* make_textBuffer(FILE* file_stream, int* file_size);

int retMaxKCap(int k);

void start_hideInformation();

void separateInBits(char* res, char character, int k, int pixelBit_index);

int receiveTextVariables(char* file[], FILE** text);

int receiveLSBAmount();

BMP* receiveSourceImage(char* file[]);

#endif
