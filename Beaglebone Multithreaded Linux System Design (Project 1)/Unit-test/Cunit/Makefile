# _*_ MakeFile _*_
#CC = gcc
CC = arm-linux-gcc
CFLAGS = -I../inc/
CUNIT = -lm -lrt

vpath %.c ../src

SRC := test.c temp.c light.c queue.c

OBJ := $(SRC:.c=.o)

unittest: $(OBJ)
	$(CC) -o unittest $(OBJ) $(CFLAGS) $(CUNIT)

clean:
	rm -f *.o unittest 
