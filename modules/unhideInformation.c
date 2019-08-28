#include "unhideInformation.h"








BMP* receiveSourceImage(char file[]){

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

void printTextStego(char* word[], int wordLength){
	FILE* pf = fopen("texto.csv", "a+");
	

	for (int i=0; i<wordLength; i++) {
		fprintf(pf, "%s ", word[i]);
	}
}

uint8_t* receiveImage(uint32_t *width, uint32_t *height){
	char file[60];
	printf("Input the image where the information has been hidden:\n");
	scanf("%s", file);
	BMP *src_img = receiveSourceImage(file);

	
        width = bmp_width(src_img);
        height = bmp_height(src_img);
      

        //the edit

	uint8_t* dataNEW = (uint8_t*)bmp_data(src_img);
	return dataNEW;
}


int receiveLSBAmount(){
	int k = 0;
	printf("Input the amount of LSB you have used:\n");
	scanf("%d", &k);

	if (k < 1 || k > 8){
		printf("Invalid Amount.\n" );
		exit(EXIT_FAILURE);//Antes de esto habría que cerrar el puntero al text file y liberar la memoria del buffer (!)
	}
	return k;
}

void start_unhideInformation() {
	uint32_t height  = 0;
	uint32_t width = 0; 
	uint8_t* dataNew = receiveImage(&height, &width);
	int k  = receiveLSBAmount();
	uint8_t extractor = pow(2, k)-1;
	uint8_t temp_byte;
	uint8_t conteiner = 0;
	uint8_t word[5000];
	int incrementadorWord = 0;
	u_int8_t anothertemp_byte;
	int j=0;
	int i=0;
	int incrementador = 0;
	int cantAlign = 0;
	int pixel_index = 0;
		while (incrementadorWord != 1000){
			temp_byte = dataNew[pixel_index*4 + i] & extractor;
			printf("Bits originales: %d \n", temp_byte);
			int cant = 8-k-k*incrementador-cantAlign;
			if (cant > 0){
				printf("Bits extraidos: %d \n", temp_byte);
				temp_byte  = temp_byte<<(cant);
				conteiner  = temp_byte | conteiner;
				printf("Bits del conteiner cant: %d \n", conteiner);
				incrementador++;

			} else if (cant == 0){
				conteiner  = temp_byte | conteiner;
				printf("Bits del conteiner: %d \n", conteiner);
				word[incrementadorWord] = conteiner;
				incrementadorWord++;
				incrementador = 0;
				conteiner = 0;
				cantAlign = 0;

			} else {
				cantAlign = abs(cant);
				printf("Bits de alineacion: %d \n",cantAlign);
				uint8_t extractorAlign = pow(2, cantAlign)-1;
				anothertemp_byte = temp_byte & extractorAlign;
				anothertemp_byte = anothertemp_byte<<8-cantAlign;
				temp_byte = temp_byte>>cantAlign;
				conteiner  = temp_byte | conteiner;
				word[incrementadorWord] = conteiner;
				incrementadorWord++;
				incrementador = 0;
				conteiner = 0;
				conteiner = conteiner | anothertemp_byte;
			}
			if(i == 4){
			pixel_index++;
			}
			i++;

	}

		
		FILE* pf = fopen("texto.csv", "w");
	

	for (int i=0; i<1000; i++) {
		if (i%4 == 0){
			printf("Pixel: %d \n",i/4);
		}
		
		fprintf(pf, "%d ", word[i]);
		printf("%d ",word[i]);
	}
}


