#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, int* argv[]){

	FILE *file_stream = NULL;
	uint8_t* buffer; // We use integers with Byte length to have an easier manipulation of the file
	char *mode = "rb"; // rb is read in binary mode

	file_stream = fopen((char *) argv[1], mode); // Read filename from the parameter that was given when the program was called
	
	if(file_stream == NULL){
                fprintf(stderr, "Can't open file, cancelling\n");
                return 1;
        }

	fseek(file_stream, 0, SEEK_END); // Go to the end of file
	int file_size = ftell(file_stream); // Offset from start
	rewind(file_stream); // Go to start

	buffer = (uint8_t *)malloc((file_size+1)*sizeof(uint8_t)); //Calculate file size
	fread(buffer, file_size, 1, file_stream); // Copy the file to the buffer

	fclose(file_stream);

// Test to see how to manipulate the file byte for byte
	fwrite( buffer, file_size, 1, stdout); // print to stdout: file_size times 1 Byte from the pointer 'buffer'
	printf("\n \n");
	
	buffer[0] = 0x4d;

	fwrite( buffer, file_size, 1, stdout);
	free(buffer);
	return 0;

}
