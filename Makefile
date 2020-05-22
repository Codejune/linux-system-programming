.SUFFIXES: .c .o
.PHONY: dep all new clean

# Compiler
CC = gcc
# Compile option
# -c: Generate object file
# -W, -Wall: Print warning about all ambigous grammer 
# -Wextra: Print warning out of -W, -Wall
# -O2: Optimization
# -g: Debugging, PLEASE DELETE AFTER PROJECT COMPLETE!
CFLAGS = -c -W -Wall -Wextra -g $(INC)

# Execute program file
CRONTAB = ssu_crontab
CROND = ssu_crond
RSYNC = ssu_rsync
# Source file
CRONTAB_SRCS = ssu_crontab.c
CROND_SRCS = ssu_crond.c
RSYNC_SRCS = ssu_rsync.c
SRCS = $(CRONTAB_SRCS) $(CROND_SRCS) $(RSYNC_SRCS)
# Object file
CRONTAB_OBJS = $(CRONTAB_SRCS:.c=.o)
CROND_OBJS = $(CROND_SRCS:.c=.o)
RSYNC_OBJS = $(RSYNC_SRCS:.c=.o)
OBJS = $(CRONTAB_OBJS) $(CROND_OBJS) $(RSYNC_OBJS)
# Header file
HDRS = 
# Library file
LIBS = 
# Include path
INC = 

# Execute file grneration
# $@ = TARGET
# $^ = DEPENDENCY
# make all: Make all execute file
all : $(OBJS)
	$(CC) -o $(CRONTAB) $(CRONTAB_OBJS) $(LIBS)
	$(CC) -o $(CROND) $(CROND_OBJS) $(LIBS)
	$(CC) -o $(RSYNC) $(RSYNC_OBJS) $(LIBS)
$(CRONTAB) : $(CRONTAB_OBJS)
	$(CC) -o $@ $^ $(LIBS)
$(CROND) : $(CROND_OBJS)
	$(CC) -o $@ $^ $(LIBS)
$(RSYNC) : $(RSYNC_OBJS)
	$(CC) -o $@ $^ $(LIBS)

# Object file generation
$(OBJS): $(HDRS)
	$(CC) $(CFLAGS) $(SRCS)

# make dep: Make dependency information file
dep:
	$(CC) -M $(INC) $(SRCS) > .dependency

# make new: Re-generation 
new:
	$(MAKE) clean
	$(MAKE)

# make clean: Remove all generated file
clean:
	rm -rf $(OBJS) $(CRONTAB) $(CROND) $(RSYNC)

