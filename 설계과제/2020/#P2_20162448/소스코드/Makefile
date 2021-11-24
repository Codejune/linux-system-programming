# 확장자 규칙
.SUFFIXES: .c .o
.PHONY: clean

# Compiler
CC = gcc 
# Compile option
CFLAGS = -c -W -Wall -Wextra -g

# Execute program file
TARGET = ssu_mntr
# Source file
SRCS = support.c monitoring.c prompt.c ssu_mntr.c
# Object file
OBJECTS = support.o monitoring.o prompt.o ssu_mntr.o
# Header file
HDRS = common.h monitoring.h prompt.h

# Execute program file generation
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

$(OBJECTS): $(HDRS)
	$(CC) $(CFLAGS) $(SRCS)

# .c|.o를 만날 경우
#.c.o: 
#	$(cc) $(CFLAGS) $(SRCS)

# make all
all: 
	$(TARGET) clean

# make clean
clean:
	rm -rf $(OBJECTS) $(TARGET)


