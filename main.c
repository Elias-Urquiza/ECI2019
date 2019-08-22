#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "./modules/hideInformation.c"
#include "./modules/unhideInformation.c"
#include "./modules/benchmarking.c"

void printUsage(){
		printf("usage:\nfirst parameter: write a number from 1 to 3.\n1 to hide information\n2 to extract information\n3 to do benchmarking\n");
}

int main(int argc, char* argv[]){

int mode;	// The code from here it's only to ensure the usage, the solution to every algorithm is in the modules
if(argc==1){
	printUsage();
	return 0;
}

if(argc > 2){
	printUsage();
	return 0;

	} else {
	if(*argv[1]=='1'){
	mode = 1;
	} else if(*argv[1]=='2'){
	mode = 2;
	} else if(*argv[1]=='3'){
	mode = 3;
	} else {
	printf("Wrong usage!\n");
	return 0;
	}
}
/*
	for(int i=1;i<argc;i++){
	printf("%s \n", argv[i]);
	}
*/
	if(mode == 1){
	printf("You have chosen to hide information\n");
	start_hideInformation();	// Here goes the module written by Alex, this function must be written in hideinformation.c
	} else if(mode == 2){
		printf("You have chosen to unhide information\n");
		start_unhideInformation(); // Here goes the module written by Me, written in unhideinformation.c
	} else if(mode == 3){
			printf("You have chosen to do benchmarking\n");
			start_benchmarking();	// Here goes the module written by Tomi, David and Elias. Defined in benchmarking
	}
return 0;
}
