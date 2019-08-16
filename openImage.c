#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, int* argv[]){

	FILE *file_stream = NULL;
	int* buffer;
	char *mode = "rb"; // "rb" is read/binary mode

	file_stream = fopen((char *) argv[1], mode); // Read filename from the parameter that was given when the program was called
	
	if(file_stream == NULL){
                fprintf(stderr, "Can't open file, cancelling\n");
                return 1;
        }

	fseek(file_stream, 0, SEEK_END); // Go to the end of file
	int file_size = ftell(file_stream); // Offset from start
	rewind(file_stream); // Go to start

	buffer = (int *)malloc((file_size+1)*sizeof(int)); //Calculate file size
	fread(buffer, file_size, 1, file_stream); // Copy the file to the buffer
	fclose(file_stream);
	fwrite( buffer, file_size, 1, stdout); // print to stdout: file_size times 1 Byte from the pointer 'buffer'

	free(buffer);
	return 0;
}
