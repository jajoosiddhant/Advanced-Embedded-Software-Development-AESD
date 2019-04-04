#Makefile
#Author Satya Mehta

#CC=arm-linux-gcc -g
CC=gcc -g
sock1: sock1.o
	$(CC) -o sock1 sock1.o -lrt

sock1.o: sock1.c
	$(CC) -c sock1.c

clean: 
	rm -f sock1 sock1.o
