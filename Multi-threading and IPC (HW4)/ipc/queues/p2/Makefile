CC=~/APES/build/buildroot/output/host/usr/bin/arm-linux-gcc
#CC=gcc
CFLAGS=-g
LIBS=-lpthread
TARGET=p2
CFILES=p2.c
#HFILES:= 

SRCS:=${HFILES}${CFILES}
OBJS:=${CFILES:.c=.o}

all:$(TARGET)
	
p2: p2.c
	$(CC) $(CFLAGS) -o p2 p2.c -lpthread -lrt

clean:
	-rm -f $(TARGET) *.o $(GARBAGE) *.NEW *.d *.~


