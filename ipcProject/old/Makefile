# *** MACROS
CC=gcc
CFLAGS=-g -ansi -pedantic -Wall -Wconversion

INC_PATH=../../include/
INC_PATH2=./Includes/
HASH_PATH=./cdrHash/
LIST_PATH=../../advancedC/genericList/
MODULES_PATH=./Modules/
CFLAGS += -I$(INC_PATH) -I$(INC_PATH2)

# *** TARGETS
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
HASH_SRC= $(wildcard $(HASH_PATH)genericHash.c)
HASH_OBJS=$(HASH_SRC:.c=.o)
LIST_SRC= $(wildcard $(LIST_PATH)genericDoubleList.c)
LIST_OBJS=$(LIST_SRC:.c=.o)


MODULES_SRC= $(wildcard $(MODULES_PATH)*.c)
MODULES_OBJS=$(MODULES_SRC:.c=.o)

TARGET=testSuite.out

$(TARGET): $(OBJS) $(LIST_OBJS) $(HASH_OBJS) $(MODULES_OBJS)
	$(CC) $(OBJS) $(LIST_OBJS) $(HASH_OBJS) $(MODULES_OBJS) -o $(TARGET) -lm -pthread

all: $(TARGET)

include depends

depends:
	$(CC) -MM -I$(INC_PATH) -I$(INC_PATH2) $(SRCS) $(LIST_SRC) $(HASH_SRC) $(MODULES_SRC) > depends

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

