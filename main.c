#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "libbmp.h"

void printUsage(){
		printf("usage:\nfirst parameter: write a number from 1 to 3.\n1 to hide information\n2 to extract information\n3 to do benchmarking\n");
}

int main(int argc, char* argv[]){
int mode = 0;
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

	for(int i=1;i<argc;i++){
	printf("%s \n", argv[i]);
	}
}
