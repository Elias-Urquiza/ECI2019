/* Este archivo debe contener la implementacion de la funcion que escribe en un bmp el mensaje secreto en sus LSB. 
 * Tener en cuenta que hay que preguntarle al usuario en que archivo quiere esconder el mensaje, cual es el mensaje, y cuantos bits usa. 
 * Esta funcion es llamada apenas el usuario elige el modo de uso entre esconder informacion, descifrar informacion, o hacer benchmarking
 */

#include <stdlib.h>
#include <stddef.h>
#include "libbmp.h"
#include <stdio.h> 

char* make_text_buffer(FILE* file_stream, int* file_size){
	char* buffer;
	char *mode = "rb"; // "rb" is read/binary mode
	
	fseek(file_stream, 0, SEEK_END); // Go to the end of file
	*file_size = ftell(file_stream); // Offset from start
	rewind(file_stream); // Go to start

	buffer = (char *)malloc((*file_size+1)*sizeof(char)); //Calculate file size
	fread(buffer, *file_size, 1, file_stream); // Copy the file to the buffer
	fclose(file_stream);

	return buffer;
}


void start_hideInformation(){

	char file[60];

	//open and prepare text file
	printf("Input the text file to hide:\n");
	scanf("%s", file);

	FILE* text = fopen(file,"r");
	if (text == 0) {
		fprintf(stderr, "Error opening file.\n");
		exit(EXIT_FAILURE);
	}

	int file_size =0;
	char* buffer = make_text_buffer(text, &file_size);

	//save the k value
	int k;
	printf("Input the amount of LSB you want to use:\n");
	scanf("%d", &k);

	if (k < 1 || k > 8){
		printf("Amount not valid.\n" );
		exit(EXIT_FAILURE);
	}

	//read the image
	printf("Input the image where the information will be hidden:\n");
	scanf("%s", file);

	BMP *src_img = bmp_read (file);

	//check if the image is ok to use
	if (bmp_compression(src_img) != BI_RGB) {
		fprintf(stderr, "Error: the image is compressed\n");
		exit(EXIT_FAILURE);
	}

	if (bmp_bit_count(src_img) == 24) {
		bmp_convert_24_to_32_bpp(src_img);
	}
	if (bmp_bit_count(src_img) == 8) {
		bmp_convert_8_to_32_bpp(src_img);
	}

    //prepare the new image
	BMP* bmpNEW = bmp_copy(src_img, 1);

	buffer_info_t info_src, info_new;
	setear_buffer(&info_src, src_img);
	setear_buffer(&info_new, bmpNEW);

	bgra_t (*src_matrix)[(info_src.row_size+3)/4] = (bgra_t (*)[(info_src.row_size+3)/4]) info_src.bytes;
    bgra_t (*new_matrix)[(info_new.row_size+3)/4] = (bgra_t (*)[(info_new.row_size+3)/4]) info_new.bytes;

	uint32_t width = info_new.width;
	uint32_t height = info_new.height;
	uint32_t row_size = info_new.row_size;

	//the edit

	uint32_t pixel_total = height * row_size;

	uint8_t* dataNEW = (uint8_t*)bmp_data(bmpNEW);

	for (int i = 0; i<pixel_total && i<file_size; i++){
		
		uint8_t Bit1, Bit2, Bit3, Bit4, Bit5, Bit6, Bit7, Bit8;

		Bit1 = buffer[i] & 0x01;
		Bit2 = buffer[i] & 0x02;
		Bit3 = buffer[i] & 0x04;
		Bit4 = buffer[i] & 0x08;
		Bit5 = buffer[i] & 0x10;
		Bit6 = buffer[i] & 0x20;
		Bit7 = buffer[i] & 0x40;
		Bit8 = buffer[i] & 0x80;

		if (k == 1){
			dataNEW[i*4+0] >> k;
			dataNEW[i*4+1] >> k;
			dataNEW[i*4+2] >> k;
			dataNEW[i*4+3] >> k;

			dataNEW[i*4+0] << k;
			dataNEW[i*4+1] << k;
			dataNEW[i*4+2] << k;
			dataNEW[i*4+3] << k;

			dataNEW[i*4+0] | Bit1;
			dataNEW[i*4+1] | Bit2;
			dataNEW[i*4+2] | Bit3;
			dataNEW[i*4+3] | Bit4;

			dataNEW[i*4+0+4] >> k;
			dataNEW[i*4+1+4] >> k;
			dataNEW[i*4+2+4] >> k;
			dataNEW[i*4+3+4] >> k;

			dataNEW[i*4+0+4] << k;
			dataNEW[i*4+1+4] << k;
			dataNEW[i*4+2+4] << k;
			dataNEW[i*4+3+4] << k;

			dataNEW[i*4+0+4] | Bit5;
			dataNEW[i*4+1+4] | Bit6;
			dataNEW[i*4+2+4] | Bit7;
			dataNEW[i*4+3+4] | Bit8;

			i++;

		} else if (k == 2){
			dataNEW[i*4+0] >> k;
			dataNEW[i*4+1] >> k;
			dataNEW[i*4+2] >> k;
			dataNEW[i*4+3] >> k;

			dataNEW[i*4+0] << k;
			dataNEW[i*4+1] << k;
			dataNEW[i*4+2] << k;
			dataNEW[i*4+3] << k;

			dataNEW[i*4+0] | (Bit1 | Bit2);
			dataNEW[i*4+1] | (Bit3 | Bit4);
			dataNEW[i*4+2] | (Bit5 | Bit6);
			dataNEW[i*4+3] | (Bit7 | Bit8);

		} else if (k == 3){
			dataNEW[i*4+0] >> k;
			dataNEW[i*4+1] >> k;
			dataNEW[i*4+2] >> (k-1);

			dataNEW[i*4+0] << k;
			dataNEW[i*4+1] << k;
			dataNEW[i*4+2] << (k-1);

			dataNEW[i*4+0] | (Bit1 | Bit2 | Bit3);
			dataNEW[i*4+1] | (Bit