# _*_ MakeFile _*_
CC=gcc 
CFLAGS= -g -lm
main: 
	$(CC) main.c ./modules/libbmp.c ./modules/hideInformation.c ./modules/unhideInformation.c $(CFLAGS) $^ -o $@
