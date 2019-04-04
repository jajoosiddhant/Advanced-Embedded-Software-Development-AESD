PLATFORM=BBG
vpath %.c src/
TARGET= project1
INCLUDES_COMMON= -I inc/

ifeq 	($(PLATFORM),HOST)
	CC = gcc
	FLAGS= -D$(TARGET)
	LDFLAGS = -lpthread -lrt -lm
	SRC := main.c logger.c temp.c light.c sockets.c queue.c my_signal.c gpio.c timer.c
	OBJ := $(SRC:.c=.o)
	CFLAGS = -g $(INCLUDES_COMMON)
endif

ifeq 	($(PLATFORM),BBG)
	CC=arm-linux-gcc
	FLAGS= -D$(TARGET)
	LDFLAGS = -lpthread -lrt -lm
	SRC := main.c logger.c temp.c light.c sockets.c queue.c my_signal.c gpio.c timer.c
	OBJ := $(SRC:.c=.o)
	CFLAGS = -g $(INCLUDES_COMMON)

endif


build: 	$(OBJ)
	$(CC) $(CFLAGS) $(FLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

	
clean:
	rm -f $(TARGET) *.o *.elf *.map
