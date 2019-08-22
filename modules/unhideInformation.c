#include <stdlib.h>
#include <stddef.h>
#include "libbmp.h"
#include <unistd.h>

void start_unhideInformation(){

int buffer_size = 60;
uint8_t filename_buffer[buffer_size];

printf("Your input's shouldn't be more than 60 characters long.\nInput your file to read from:\n");

read(STDIN_FILENO, filename_buffer, buffer_size);
// Open Image and open object for pixel manipulation, check if there are any errors and then ask what is the size of k.
// After that, extract the message and store it in a text file

int LSBquantity_buffer
printf("How many LSB did you use in your image?: \n");
read(STDIN_FILENO, LSBquantity_buffer, buffer_size);

}
