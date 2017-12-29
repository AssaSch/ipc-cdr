# *** MACROS
CC=gcc
CFLAGS=-g -ansi -pedantic -Wall -Wconversion

INC_PATH=../../../include/
INC_PATH_PROJECT=../Includes/
GENERAL_PATH=../generalModules/

CFLAGS += -I$(INC_PATH) -I$(INC_PATH_PROJECT)

# *** TARGETS
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
GENERAL_SRC= $(wildcard $(GENERAL_PATH)*.c)
GENERAL_OBJS=$(GENERAL_SRC:.c=.o)

TARGET=ui

$(TARGET): $(OBJS) $(GENERAL_OBJS)
	$(CC) $(OBJS) $(GENERAL_OBJS) -o $(TARGET) -lm -pthread

all: $(TARGET)

include depends

depends:
	$(CC) -MM -I$(INC_PATH) -I$(INC_PATH_PROJECT) $(SRCS) $(GENERAL_SRC) > depends

# *** COMMANDS
clean:
	-rm $(TARGET)
	-rm *.o
	-rm depends

run:
	./$(TARGET)
	
gdb:
	gdb $(TARGET)
##################################

