.SUFFIXES: .c .o

TARGET = ssu_mntr
CC = gcc
CFLAGS = -g -Wall -Wextra -c
INC = 

OBJECTS = support.o monitoring.o prompt.o ssu_mntr.o
SRCS = support.c monitoring.c prompt.c ssu_mntr.c

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

$(OBJECTS): common.h monitoring.h prompt.h
	$(CC) $(CFLAGS) $(SRCS)

.c.o:
	$(cc) $(INC) $(CFLAGS) 

clean:
	rm $(OBJECTS) $(TARGET)


