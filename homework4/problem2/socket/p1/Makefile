CC=~/APES/build/buildroot/output/host/usr/bin/arm-linux-gcc
#CC=gcc
CFLAGS=-g
LIBS=-lpthread
TARGET=server
CFILES=server.c
#HFILES:= 

SRCS:=${HFILES}${CFILES}
OBJS:=${CFILES:.c=.o}

all:$(TARGET)
	
server: server.c
	$(CC) $(CFLAGS) -o server server.c -lpthread -lrt

clean:
	-rm -f $(TARGET) *.o $(GARBAGE) *.NEW *.d *.~


