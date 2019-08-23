/* Este archivo debe contener la implementacion de la funcion que hace benchmarking. 
 * Esta funcion es llamada cuando en el script principal se elige la opcion de benchmarking. 
 * Antes de llamar la funcion no se le pide nada al usuario, 
 * ningun archivo de texto ni imagen para esconder/extraer/loquesea, 
 * de eso se van a encargar dentro de esta funcion */

#include "benchmarking.h"
/*
double mse (u_int32_t* image1, u_int32_t* image2, u_int32_t height, u_int32_t width){
	// Iniciatilaze the sum.
		 long long sum = 0;
	
	//Cast from uint8_t* to u_int32_t* because we want to iterate per pixel
		
	 //It iterates every 4 bytes, the same as a pixel.
	 		for (int j=0; j<457; j++) {

	 			for (int i=0; i<width; i++) {

	 				double error    = image1[j*width + i] - image2[j*width + i];
	 				double cuadrado = error*error;
	 					   sum     += cuadrado;
	 					  
	 		}
	 	}

	 	double res = sum/(height*width);
	 	return res;
}
*/

double mse (u_int8_t* image1, u_int8_t* image2, u_int32_t height, u_int32_t width){
	// Iniciatilaze the sum.
		 long long sum = 0;
	
	//Cast from uint8_t* to u_int32_t* because we want to iterate per pixel
		
	 //It iterates every 4 bytes, the same as a pixel.
	 		for (int j=0; j<height; j++) {

	 			for (int i=0; i<width*4; i=i+4) {

	 				double errorA    = image1[j*width + i] - image2[j*width + i];
	 				double errorR    = image1[j*width + i+1] - image2[j*width + i+1];
	 				double errorG    = image1[j*width + i+2] - image2[j*width + i+2];
	 				double errorB    = image1[j*width + i+3] - image2[j*width + i+3];
	 				double cuadrado = errorA*errorA + errorR*errorR + errorG*errorG + errorB*errorB;
	 					   sum     += cuadrado;
	 					  
	 		}
	 	}

	 	double res = (sum)/(height*width*4);
	 	return res;
}

double psnr (u_int8_t* image1, u_int8_t* image2, u_int32_t height, u_int32_t width) {
	double res  = 0;
	double mse1 = mse(image1, image2, height, width);

	unsigned long long max = pow(2,32)-1;

		if (mse1 != 0)	{

			res = 10*log10((pow(max,2))/(mse1));
		
		} 


	return res;
	
}


void start_benchmarking(){
		

		BMP *src_img1    = bmp_read ("photo1.bmp");

		BMP *src_img2    = bmp_read ("photo1C.bmp");

		//Save the width and height of each BMP.
	
		u_int32_t height  = bmp_height(src_img1); 	
		u_int32_t width   = bmp_width(src_img1);
		u_int32_t height1 = bmp_height(src_img2);
		u_int32_t width1  = bmp_width(src_img2);



		//Compare the size of each BMP, if they are different, the function will not continue.

		if (height != height1 || width != width1) {
			printf("The images have different size");
			
		} else {
			/*
			u_int32_t* data1 = (u_int32_t*)bmp_data(src_img1);
		 	u_int32_t* data2 = (u_int32_t*)bmp_data(src_img2);
*/
		 	u_int8_t* data1 = (u_int8_t*)bmp_data(src_img1);
		 	u_int8_t* data2 = (u_int8_t*)bmp_data(src_img2);

			double mse1  = mse(data1, data2, height, width);
			double psnr1 = psnr(data1, data2, height, width);

			printf("MSE: %lf \n",mse1);
			printf("PSNR: %lf \n",psnr1);
		}
}




