#include "unhideInformation.h"








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


	return src_img;
}

void printTextStego(char* word){
	FILE* pf = fopen("texto.csv", "a+");
	

	for (int i=0; i<sizeof(word); i++) {
		fprintf(pf, "%s ", word[i]);
	}
}

	uint8_t* receiveImage(){
	char file[60];
	printf("Input the image where the information will be hidden:\n");
	scanf("%s", file);
	BMP *src_img = receiveSourceImage(&file);

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
	return dataNEW;
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

void start_unhideInformation() {

	uint8_t* dataNew = receiveImage();
	int k 			 = receiveLSBAmount();
	int extractor = pow(2, k)-1;
	int temp_byte;
	int conteiner = 0;
	char word[60];
	int incrementadorWord = 0;
	int anothertemp_byte;
	int j=0;
	int i=0;
	int incrementador = 0;
	int alineador = 0;
	u_int32_t height  = bmp_height(dataNew);
	u_int32_t width   = bmp_width(dataNew);
	while (j<height && incrementadorWord!= sizeof(word)){
		while (i<width*4 && incrementadorWord!= sizeof(word)){
			temp_byte = dataNew[j*width + i] & extractor;
			int cant= 8-k-k*incrementador-alineador;
			if (cant > 0){
				temp_byte  = temp_byte<<(8-k-k*incrementador-alineador);
				conteiner  = temp_byte | conteiner;
				incrementador++;

			} else if (cant == 0){
				temp_byte  = temp_byte<<(8-k-k*incrementador-alineador);
				conteiner  = temp_byte | conteiner;
				word[incrementadorWord] = (char) conteiner;
				incrementadorWord++;
				incrementador = 0;
				conteiner = 0;

			} else {
				int cantAlign = abs(cant);
				int extractorAlign = pow(2, cantAlign)-1;
				anothertemp_byte = temp_byte | extractorAlign;
				anothertemp_byte = anothertemp_byte<<k-alineador;
				temp_byte = temp_byte>>cantAlign;
				conteiner  = temp_byte | conteiner;
				word[incrementadorWord] = (char) conteiner;
				incrementadorWord++;
				incrementador = 0;
				conteiner = 0;
				conteiner = conteiner | anothertemp_byte;
			}

			i++;

	}
	j++;
}

		printtextStego(*word);

}

int main(){
	start_unhideInformation();

}

