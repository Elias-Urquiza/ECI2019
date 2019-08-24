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
// depending on the k that has been chosen, the usable bits per pixel are different, we return the quantity in this function
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
	char* textInput_buffer = make_textBuffer(text, &file_size);
	printf("filesize %u",file_size);
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

	int char_index = 0;
	bool char_isDefined = false;
	int pixel_index = 0;
	bool pixel_isDefined = false;
	int textInput_index = 0;
	int pixel_MaximumBitCapacity = retMaxKCap(k);
	// estoy asumiendo que el tamanio del texto es menor que el que permite guardar la imagen	
	while(textInput_index <= file_size){
		if(!char_isDefined){
			char character = textInput_buffer[textInput_index];
			textInput_index++;
			//uint8_t bits_character[8] = separateInBits(character);
			char_isDefined = true;
			char_index = 0;
		}
		if(!pixel_isDefined){
//			char pixel[pixel_MaximumBitCapacity] = function that makes the pixel connected to it's bits.
			pixel_isDefined = true;
			pixel_index = 0;
		}
			while( pixel_index != pixel_MaximumBitCapacity && char_index != 7){
				//pixel[pixel_index] = bits_character[char_index];
				pixel_index++;
				char_index++;
			}
		// El while va a ir avanzando por la estructura y modificar los bits completables
		if(pixel_index == pixel_MaximumBitCapacity){ // Aka si se acabaron los bits libres para meter en el pixel
			pixel_isDefined = false;
			pixel_index = 0;
		}
		if(char_index == 7){
			char_isDefined = false;
			char_index = 0;
		}	
	}
}

