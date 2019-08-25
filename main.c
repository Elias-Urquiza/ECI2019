#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//#include "/opt/opencv/include/opencv2/opencv.hpp"
#include "./modules/hideInformation.h"
//#include "./modules/unhideInformation.h"
//#include "./modules/benchmarking.h"

void printUsage(){
		printf("usage:\nfirst parameter: write a number from 1 to 3.\n1 to hide information\n2 to extract information\n3 to do benchmarking\n");
}

// The code from main is only to ensure the usage, the solution to every algorithm is in the modules

int main(int argc, char* argv[]){

int mode = 0;

if(argc!=2){
	printUsage();
	return 1;
}
else {
	mode = atoi(argv[1]); //converts a given number expressed in ASCII to the equivalent int it represents (e.g. '1' turns into 1)
	//if the given argument is not conversible to an int, mode = 0;
	}
/*
	for(int i=1;i<argc;i++){
	printf("%s \n", argv[i]);
	}
*/
switch(mode){
	case 1 :
		printf("You have chosen to hide information.\n");
		start_hideInformation();	// This is the module written by Alex, found in hideinformation.c
		break;

	case 2 :
		printf("You have chosen to unhide information.\n");
//		start_unhideInformation(); // This is the module written by Jonatan, found in unhideinformation.c
		break;

	case 3 :
		printf("You have chosen to perform benchmarking.\n");
//		start_benchmarking(); // This is the module written by Tomás, David and Elías, found in benchmarking.c
		break;

	default:
		printf("Invalid Usage.\n");
	}
return 0;
}
