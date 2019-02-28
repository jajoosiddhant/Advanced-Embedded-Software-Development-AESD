CC=~/APES/build/buildroot/output/host/usr/bin/arm-linux-gcc
CC=gcc
CFLAGS=-g
LIBS=-lpthread
TARGET=pipes
CFILES=pipes.c
#HFILES:= 

SRCS:=${HFILES}${CFILES}
OBJS:=${CFILES:.c=.o}

all:$(TARGET)
	
threading: threading.c
	$(CC) $(CFLAGS) -o pipes pipes.c -lpthread -lrt

clean:
	-rm -f $(TARGET) *.o $(GARBAGE) *.NEW *.d *.~


