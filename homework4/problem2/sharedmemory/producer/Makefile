CC=~/APES/build/buildroot/output/host/usr/bin/arm-linux-gcc
#CC=gcc
CFLAGS=-g
LIBS=-lpthread
TARGET=producer
CFILES=producer.c
#HFILES:= 

SRCS:=${HFILES}${CFILES}
OBJS:=${CFILES:.c=.o}

all:$(TARGET)
	
producer: producer.c
	$(CC) $(CFLAGS) -o producer producer.c -lpthread -lrt

clean:
	-rm -f $(TARGET) *.o $(GARBAGE) *.NEW *.d *.~


