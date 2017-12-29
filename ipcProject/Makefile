# *** MACROS
CC=gcc
CFLAGS=-g -ansi -pedantic -Wall -Wconversion

INC_PATH=../../include/
INC_PATH_PROJECT=Includes/
LIST_PATH=../../advancedC/genericList/
GENERAL_PATH=generalModules/
PRCS_PATH=Proccesor/
READER_PATH=Reader/
UI_PATH=Ui/
LOGGER_PATH=../../advancedC/logger/

CFLAGS += -I$(INC_PATH) -I$(INC_PATH_PROJECT)

# *** TARGETS
PRCS_SRCS=$(wildcard $(PRCS_PATH)*.c)
PRCS_OBJS=$(PRCS_SRCS:.c=.o)
READER_SRCS=$(wildcard $(READER_PATH)*.c)
READER_OBJS=$(READER_SRCS:.c=.o)
UI_SRCS=$(wildcard $(UI_PATH)*.c)
UI_OBJS=$(UI_SRCS:.c=.o)
LIST_SRC= $(wildcard $(LIST_PATH)genericDoubleList.c)
LIST_OBJS=$(LIST_SRC:.c=.o)
GENERAL_SRC= $(wildcard $(GENERAL_PATH)*.c)
GENERAL_OBJS=$(GENERAL_SRC:.c=.o)
LOGGER_SRCS=$(wildcard $(LOGGER_PATH)logger.c)
LOGGER_OBJS=$(LOGGER_SRCS:.c=.o)

TARGET1=processor
TARGET2=reader
TARGET3=ui

$(TARGET1): $(OBJS) $(LIST_OBJS) $(GENERAL_OBJS) $(PRCS_OBJS) $(LOGGER_OBJS)
	$(CC) $(OBJS) $(LIST_OBJS) $(GENERAL_OBJS) $(PRCS_OBJS) $(LOGGER_OBJS) -o $(TARGET1) -lm -pthread

$(TARGET2): $(OBJS) $(LIST_OBJS) $(GENERAL_OBJS) $(READER_OBJS) $(LOGGER_OBJS)
	$(CC) $(OBJS) $(LIST_OBJS) $(GENERAL_OBJS) $(READER_OBJS) $(LOGGER_OBJS) -o $(TARGET2) -lm -pthread
	
$(TARGET3): $(OBJS) $(GENERAL_OBJS) $(UI_OBJS) $(LOGGER_OBJS)
	$(CC) $(OBJS) $(LIST_OBJS) $(GENERAL_OBJS) $(UI_OBJS) $(LOGGER_OBJS) -o $(TARGET3) -lm -pthread

all: $(TARGET1) $(TARGET2) $(TARGET3)

include depends

depends:
	$(CC) -MM -I$(INC_PATH) -I$(INC_PATH_PROJECT) $(LIST_SRC) $(GENERAL_SRC) $(PRCS_SRCS) $(READER_SRCS) $(UI_SRCS) $(LOGGER_SRCS) > depends

# *** COMMANDS
clean:
	-rm $(TARGET)
	-rm *.o
	-rm depends

run:
	./$(TARGET2)
	
gdb:
	gdb $(TARGET2)
##################################

