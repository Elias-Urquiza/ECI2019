#ifndef __UNHIDEINFO__
#define __UNHIDEINFO__

#include <stdlib.h>
#include <stddef.h>
#include<unistd.h>
#include "./libbmp.h"






void start_unhideInformation();
BMP* receiveSourceImage(char* file[]);
void printText(char word);
uint8_t* receiveImage();
int receiveLSBAmount();
void start_unhideInformation();

#endif
