CC=~/APES/build/buildroot/output/host/usr/bin/arm-linux-gcc
#CC=gcc
CFLAGS=-g
LIBS=-lpthread
TARGET=client
CFILES=client.c
#HFILES:= 

SRCS:=${HFILES}${CFILES}
OBJS:=${CFILES:.c=.o}

all:$(TARGET)
	
client: client.c
	$(CC) $(CFLAGS) -o client client.c -lpthread -lrt

clean:
	-rm -f $(TARGET) *.o $(GARBAGE) *.NEW *.d *.~


