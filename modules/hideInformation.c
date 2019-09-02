/* Este archivo debe contener la implementacion de la funcion que escribe en un bmp el mensaje secreto en sus LSB. 
 * Tener en cuenta que hay que preguntarle al usuario en que archivo quiere esconder el mensaje, cual es el mensaje, y cuantos bits usa. 
 * Esta funcion es llamada apenas el usuario elige el modo de uso entre esconder informacion, descifrar informacion, o hacer benchmarking
 */

#include "hideInformation.h"

//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
int receiveTextVariables(char* file[], FILE** text){
	//open and receive text file
	
	return 0;
}

//------------------------------------------------------------------------
int receiveLSBAmountHideInfo(){
	int k = 0;
	printf("Input the amount of LSB you want to use:\n");
	scanf("%d", &k);

	if (k < 1 || k > 8){
		printf("Invalid Amount.\n" );
		exit(EXIT_FAILURE);//Antes de esto habría que cerrar el puntero al text file y liberar la memoria del buffer (!)
	}
	return k;
}
//------------------------------------------------------------------------

BMP* receiveSourceImageToHide(char* file[]){

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


	return src_img;
}
//------------------------------------------------------------------------

void separateInBits(char* bits_character, char character, int k, int pixelBit_index){
	int offset = pixelBit_index % k;
	int maxBitsPerByte = 8;
	for (int i = 0; i < maxBitsPerByte; i++){
		bits_character[i] = (0x01 << i) & character;	
	}
	if(k < 8){
		int internalOffset = 0;
		for (int j = 0; j < maxBitsPerByte;j++){
			internalOffset = internalOffset % k;
			int totalOffset = (internalOffset+offset) % k;
			if( (j - totalOffset) < 0 ){
				int incr = j - totalOffset; // abs(j - totalOffset)
				incr = -incr;
				bits_character[j] = bits_character[j] << incr;
			} else {
				bits_character[j] = bits_character[j] >> (j - (totalOffset));
			}
				internalOffset++;
		}
	}
}
//------------------------------------------------------------------------
void start_hideInformation(){

	char file[60];
	FILE* text = NULL;
	uint32_t file_size;

	printf("Input the text file to hide:\n");
	scanf("%s", file);

	text = fopen(file,"rb");
	if (text == 0) {
		fprintf(stderr, "Error opening file.\n");
		exit(EXIT_FAILURE);
	}

	char* text_buffer = make_textBuffer(text, &file_size);

	printf("filesize %u \n",file_size);

	int k = receiveLSBAmountHideInfo();

	//receive the image
	printf("Input the image where the information will be hidden:\n");
	scanf("%s", file);
	BMP *src_img = receiveSourceImageToHide(&file);

	//receive the new image
	BMP* bmpNEW = bmp_copy(src_img, 1);

	buffer_info_t info_src, info_new;
        setear_buffer(&info_src, src_img); // setear_buffer?
        setear_buffer(&info_new, bmpNEW);

        bgra_t (*src_matrix)[(info_src.row_size+3)/4] = (bgra_t (*)[(info_src.row_size+3)/4]) info_src.bytes;
    bgra_t (*new_matrix)[(info_new.row_size+3)/4] = (bgra_t (*)[(info_new.row_size+3)/4]) info_new.bytes;

        uint32_t width = info_new.width;
        uint32_t height = info_new.height;
        uint32_t row_size = info_new.row_size;

        //the edit

	uint8_t* dataNEW = (uint8_t*)bmp_data(bmpNEW);

	uint32_t char_index = 0;
	bool char_isDefined = false;
	uint32_t pixelBit_index = 0;
	bool pixel_isDefined = false;

	uint32_t text_index = 0;
	uint32_t pixel_MaximumBitCapacity = retMaxKCap(k);
	uint32_t pixelImage_total = height * row_size;
	uint32_t pixelImage_index = 0;

	uint8_t bits_character[8];
	uint32_t i = 0;
	uint32_t j = 0;
	bool byte_cleaned = false;
	while(text_index < file_size && pixelImage_index < pixelImage_total){
		if(!char_isDefined){
			char character = text_buffer[text_index];
			separateInBits(bits_character, character, k, pixelBit_index);
			char_isDefined = true;
			char_index = 0;
		}
		if(!pixel_isDefined){
			pixel_isDefined = true;
			pixelBit_index = 0;
		}
			while( pixelBit_index < pixel_MaximumBitCapacity && char_index < 8){

				for(;i<=3 && pixelBit_index < pixel_MaximumBitCapacity && char_index < 8;){
					if(!byte_cleaned){
					dataNEW[pixelImage_index*4+i] = (dataNEW[pixelImage_index*4+i] >> k) ;
					dataNEW[pixelImage_index*4+i] = (dataNEW[pixelImage_index*4+i] << k) ;
					byte_cleaned = true;
					}
						for(;j<k && char_index < 8 && pixelBit_index < pixel_MaximumBitCapacity;j++){
							char newData = dataNEW[pixelImage_index*4+i] | bits_character[char_index];
							dataNEW[pixelImage_index*4+i] = newData;
							pixelBit_index++;
							char_index++;
						}
						if(j == k){
							j = 0;
							i++;
							byte_cleaned = false;
						}
				}
				if(i == 4){
					i = 0;
				}
			}
		// The while-loop will move along the structure and modify the bits that can be completed
		if(pixelBit_index == pixel_MaximumBitCapacity){ // In this point, we've run out of available bits to use in the pixel
			pixel_isDefined = false;
			pixelImage_index++;
			printf("pixelImage_index: %u\n",pixelImage_index);
		}
		if(char_index == 8){
			char_isDefined = false;
			text_index++;
			printf("text index: %u \n",text_index);
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
