#include "unhideInformation.h"
#include "hideInformation.h"
#include <stdbool.h>






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

uint8_t reverseBits(uint8_t conteiner) 
{ 
    uint8_t count = sizeof(conteiner) * 8 - 1; 
    uint8_t reverse_conteiner = conteiner; 
      
    conteiner >>= 1;  
    while(conteiner) 
    { 
       reverse_conteiner <<= 1;        
       reverse_conteiner |= conteiner & 1; 
       conteiner >>= 1; 
       count--; 
    } 
    reverse_conteiner <<= count; 
    return reverse_conteiner; 
} 
char createChar(char bits_character[], int k, int pixelBit_index){
	int offset = pixelBit_index % k;
	char newCharacter = 0x00;
	char mask[8];
	for(int i = 0; i < 8;i++){
		mask[i] = (0x01) << i;
	}
	int internalOffset = 0;
	for(int j = 0; j < 8; j++){
		if(bits_character[j] == 0){
			mask[j] = mask[j] & 0x00;
		}
	}
	for(int j = 0; j < 8;j++){
	newCharacter = newCharacter | mask[j];
	}
	return newCharacter;
}

void start_unhideInformation() {
	uint32_t height = 0;
	uint32_t width = 0;
	uint8_t* dataNew = receiveImage(&height, &width);
	uint32_t k  = receiveLSBAmount();
	uint8_t word[5000];
	uint32_t j=0;
	uint32_t i=0;
	uint32_t incrementador = 0;
	uint32_t cantAlign = 0;
	uint32_t pixel_index = 0;
	uint8_t bits_character[8];
	bool char_isDefined = false;
	bool pixel_isDefined = false;
	uint32_t char_index = 0;
	uint32_t pixelBit_index = 0;
	uint32_t pixel_MaximumBitCapacity = k*4;
	uint32_t pixelImage_index = 0;
	uint32_t text_index = 0;
	while(text_index < 5000){
		if(!char_isDefined){
			uint8_t character = 0xff;
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

				for(;j<k && char_index < 8 && pixelBit_index < pixel_MaximumBitCapacity;j++)    {

					bits_character[char_index] = dataNew[pixelImage_index*4+i] & bits_character[char_index];
					pixelBit_index++;
					char_index++;
				}
				if(j == k){
					j = 0;
					i++;
				}
			}
			if(i == 4){
				i = 0;
			}
		}
		if(pixelBit_index == pixel_MaximumBitCapacity){ 
				pixel_isDefined = false;
				pixelImage_index++;
		}
		if(char_index == 8){
			word[text_index] = createChar(bits_character, k, pixelBit_index);	
			char_isDefined = false;
			text_index++;
		}
	}

		
	for (int i=0; i<1000; i++) {
		if (i%4 == 0){
			printf("Pixel: %d \n",i/4);
		}
		printf("%d ",word[i]);
	}
}


