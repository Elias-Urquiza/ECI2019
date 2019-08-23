/* Este archivo debe contener la implementacion de la funcion que escribe en un bmp el mensaje secreto en sus LSB.
 * Tener en cuenta que hay que preguntarle al usuario en que archivo quiere esconder el mensaje, cual es el mensaje, y cuantos bits usa.
 * Esta funcion es llamada apenas el usuario elige el modo de uso entre esconder informacion, descifrar informacion, o hacer benchmarking
 */

#include <stdlib.h>
#include <stddef.h>
#include "libbmp.h"
#include <stdio.h>

void start_hideInformation(){

	char file[60];

	//open and prepare text file
	printf("Input the text to hide:\n");
	scanf("%s", file);

	FILE* text = fopen(file,"r");
	if (text == 0) {
		fprintf(stderr, "Error opening file.\n");
		exit(EXIT_FAILURE);
	}

	printf("Input the image where the information will be hidden:\n");
	scanf("%s", file);

	BMP *src_img = bmp_read (file);

	//check the image is ok to use
	if (bmp_compression(src_img) != BI_RGB) {
		fprintf(stderr, "Error: The source image is compressed.\n");
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

	//the edit
	char c;

	while((c=fgetc(text))!=EOF){
		for (int i = 0; i < height; i++) {
        	for (int j = 0; j < width; j++) {
            	new_matrix[i][j].b = (int)c;
			}
		}
	}


	/*
	for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            new_matrix[i][j].b = 255;
            new_matrix[i][j].g = 0;
            new_matrix[i][j].r = 0;
            new_matrix[i][j].a = 255;
        }
    } */
/*
    uint8_t* dataNEW = (uint8_t*)bmp_data(bmpNEW);

	for(int j=0;j<height;j++) {
		for(int i=0;i<width;i++) {
			dataNEW[j*info_new.row_size+i*4+0] = 255;
			dataNEW[j*info_new.row_size+i*4+1] = 00;
			dataNEW[j*info_new.row_size+i*4+2] = 00;
			dataNEW[j*info_new.row_size+i*4+3] = 255;
		}
	}*/

	// save the image
	char exit[60];
	printf("New image name:\n");
	scanf("%s", exit);
	bmp_save(exit, bmpNEW);

	// delete bmp
	bmp_delete(src_img);
	bmp_delete(bmpNEW);
}
/*
int convert_to_ASCII (char c){
	return 0+'c';
}*/
