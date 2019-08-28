# _*_ MakeFile _*_
CC=gcc 
CFLAGS= -lm
all: 
	$(CC) -g -o main main.c ./modules/libbmp.c ./modules/hideInformation.c ./modules/unhideInformation.c $(CFLAGS)
