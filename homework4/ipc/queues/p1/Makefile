CC=~/APES/build/buildroot/output/host/usr/bin/arm-linux-gcc
#CC=gcc
CFLAGS=-g
LIBS=-lpthread
TARGET=p1
CFILES=p1.c
#HFILES:= 

SRCS:=${HFILES}${CFILES}
OBJS:=${CFILES:.c=.o}

all:$(TARGET)
	
p1: p1.c
	$(CC) $(CFLAGS) -o p1 p1.c -lpthread -lrt

clean:
	-rm -f $(TARGET) *.o $(GARBAGE) *.NEW *.d *.~


