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

uint32_t getSize (char* restChar, uint64_t image_size) {
	image_size = (image_size/512);
	int i=7;
	uint32_t c = 2479;
	while (restChar[i]<97) {
		i--;
	}
	uint64_t charDiv = restChar[i];
	uint64_t charRest;
	charDiv = charDiv - 97;
	charDiv = charDiv*27;
	i--;
	while (i > 0){
		charRest = restChar[i];
		charRest = charRest - 97;
		charDiv  = charDiv + charRest;
		charDiv  = charDiv*27;
		i--;
	}
	charRest = restChar[i];
	charRest = charRest - 97;
	charDiv = charDiv+charRest;
	charDiv = charDiv - c;
	charDiv =  (charDiv/image_size)  ;
	return charDiv;

}

void start_unhideInformation() {
	uint32_t height = 0;
	uint32_t width = 0;
	

	char file[60];
	printf("Input the image where the information has been hidden:\n");
	scanf("%s", file);
	BMP *src_img = receiveSourceImage(file);

	
        width = bmp_width(src_img);
        height = bmp_height(src_img);
      

        //the edit

	uint8_t* dataNew = (uint8_t*)bmp_data(src_img);

	uint32_t k  = receiveLSBAmount();
	uint64_t image_size = (height*4)*height;

	 

	uint8_t textWord[8];
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

//Read the first 32 bytes for the lenght of the text

	while(text_index < 8){
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
	
			textWord[text_index] = createChar(bits_character, k, pixelBit_index);	
			char_isDefined = false;
			text_index++;
		}
	}

	uint32_t word_size = getSize(textWord, image_size); 

	char word[word_size];
	text_index = 0;

//start values 

	j=0;
	i=0;
	incrementador = 0;
	cantAlign = 0;
	pixel_index = 0;
	bits_character[8];
	char_isDefined = false;
	pixel_isDefined = false;
	char_index = 0;
	pixelBit_index = 0;
	pixel_MaximumBitCapacity = k*4;
	
//Read the cypher text from stego image

	while(text_index < word_size){
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
			if (pixelImage_index < 16) {

			}


			word[text_index] = createChar(bits_character, k, pixelBit_index);	
			char_isDefined = false;
			text_index++;
		}
	}
//Writing the cypher text
FILE* text = NULL;
text = fopen("stego-text.txt","w+");	

	for (int i=0; i<word_size; i++) {
		if (i%4 == 0){
			printf("Pixel: %i \n",i/4);
		}
		
		char a = word[i];
		fprintf(text, "%c",a);
		printf("%c ",a);
		
	}
	fclose(text);
}


