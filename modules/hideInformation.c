/* Este archivo debe contener la implementacion de la funcion que escribe en un bmp el mensaje secreto en sus LSB. 
 * Tener en cuenta que hay que preguntarle al usuario en que archivo quiere esconder el mensaje, cual es el mensaje, y cuantos bits usa. 
 * Esta funcion es llamada apenas el usuario elige el modo de uso entre esconder informacion, descifrar informacion, o hacer benchmarking
 */

#include "hideInformation.h"

char* make_textBuffer(FILE* file_stream, int* file_size){
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

int retMaxKCap(int k){
	return 4*k;
}

int receiveTextVariables(char* file[], FILE** text){
	//open and receive text file
	
	return 0;
}

int receiveLSBAmount(){
	int k = 0;
	printf("Input the amount of LSB you want to use:\n");
	scanf("%d", &k);

	if (k < 1 || k > 8){
		printf("Invalid Amount.\n" );
		exit(EXIT_FAILURE);//Antes de esto habría que cerrar el puntero al text file y liberar la memoria del buffer (!)
	}
	return k;
}

BMP* receiveSourceImage(char* file[]){

	BMP *src_img = bmp_read (file);

	//check if the image is ok to use
	if (bmp_compression(src_img) != BI_RGB) {
		fprintf(stderr, "Error: the image is compressed\n"); 
		exit(EXIT_FAILURE); //Ídem de lo anterior + liberar BMP* src_img utilizando la función bmp_delete(src_img); (!)
	}
	
	// We convert the bmp to a 32bpp format
	if (bmp_bit_count(src_img) == 24) {
		bmp_convert_24_to_32_bpp(src_img);
	}
	if (bmp_bit_count(src_img) == 8) {
		bmp_convert_8_to_32_bpp(src_img);
	}	


	return src_img	
}

void separateInBits(char* res, char character, int k, int* offset){
	int j, i;
	for ( i = 0; i < 8; i++){
		for( j = 0; j < k; j++){
			res[i] = character | (0x01 << ((j+*offset) % k));
		}
	}
	*offset = (j+*offset) % k;
}

void start_hideInformation(){

	char file[60];
	FILE* text = NULL;
	int file_size;

	printf("Input the text file to hide:\n");
	scanf("%s", file);

	text = fopen(file,"r");
	if (text == 0) {
		fprintf(stderr, "Error opening file.\n");
		exit(EXIT_FAILURE);
	}

	//int file_size = receiveTextVariables(&file,&text);

	char* text_buffer = make_textBuffer(text, &file_size);

	printf("filesize %u \n",file_size);

	int k = receiveLSBAmount();

	//receive the image
	printf("Input the image where the information will be hidden:\n");
	scanf("%s", file);
	BMP *src_img = receiveSourceImage(&file);

	//receive the new image
	BMP* bmpNEW = bmp_copy(src_img, 1);

	uint8_t* dataNEW = (uint8_t*)bmp_data(bmpNEW);

	int char_index = 0;
	bool char_isDefined = false;
	int pixelBit_index = 0;
	bool pixel_isDefined = false;

	int text_index = 0;
	int pixel_MaximumBitCapacity = retMaxKCap(k);
	uint32_t pixelImage_total = height * row_size;
	int pixelImage_index = 0;

	int offset = 0;
	uint8_t bits_character[8];

	printf("afuera\n");
	// assuming the size of the text is smaller than the size limit allowed by the image
	while(text_index <= file_size && pixelImage_index <= pixelImage_total){
		printf("1\n");
		if(!char_isDefined){
			char character = text_buffer[text_index];
			text_index++;
			separateInBits(bits_character, character, k, &offset);
			char_isDefined = true;
			char_index = 0;
		}
		if(!pixel_isDefined){
			pixel_isDefined = true;
			pixelBit_index = 0;
		}
			while( pixelBit_index < pixel_MaximumBitCapacity && char_index < 7){
				printf("2\n");

				for(int i = 0;i<3 && pixelBit_index < pixel_MaximumBitCapacity && char_index < 7;i++){
					printf("3\n");
					dataNEW[pixelImage_index*4+i] >> k ;
					dataNEW[pixelImage_index*4+i] << k ;
						for(int j = 0;j<k && char_index <= 7;j++){
							printf("4\n");
							dataNEW[pixelImage_index*4+i] | bits_character[char_index];
							pixelBit_index++;
							char_index++;
						}
				}
			}
		// The while-loop will move along the structure and modify the bits that can be completed
		if(pixelBit_index == pixel_MaximumBitCapacity){ // In this point, we've run out of available bits to use in the pixel
			pixel_isDefined = false;
			pixelBit_index = 0;
			pixelImage_index++;
		}
		if(char_index == 7){
			char_isDefined = false;
			text_index++;
			char_index = 0;
		}	
	}


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
	while((c=fgetc(text))!=EOF){
		for (int i = 0; i < height; i++) {
        	for (int j = 0; j < width; j++) {
            	new_matrix[i][j].b = (int)c;
			}
		}
	}


	
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
