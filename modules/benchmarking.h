#ifndef __BENCHMARKING__
#define __BENCHMARKING__
/* Este archivo debe contener la implementacion de la funcion que hace benchmarking. 
 * Esta funcion es llamada cuando en el script principal se elige la opcion de benchmarking. 
 * Antes de llamar la funcion no se le pide nada al usuario, 
 * ningun archivo de texto ni imagen para esconder/extraer/loquesea, 
 * de eso se van a encargar dentro de esta funcion */

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "libbmp.c"


double mse (u_int8_t* image1, u_int8_t* image2, u_int32_t height, u_int32_t width);

double psnr (u_int8_t* image1, u_int8_t* image2, u_int32_t height, u_int32_t width);
	
void start_benchmarking();
		

#endif
