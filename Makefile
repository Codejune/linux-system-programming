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
CFLAGS = -c -W -Wall -Wextra -O2 -g $(INC)

# Execute program file
CRONTAB = ssu_crontab
RSYNC = ssu_rsync
# Source file
CRONTAB_SRCS = ssu_crontab.c
RSYNC_SRCS = ssu_rsync.c
SRCS = $(CRONTAB_SRCS) $(RSYNC_SRCS)
# Object file
CRONTAB_OBJS = $(CRONTAB_SRCS:.c=.o)
RSYNC_OBJS = $(RSYNC_SRCS:.c=.o)
OBJS = $(CRONTAB_OBJS) $(RSYNC_OBJS)
# Header file
HDRS = 
# Library file
LIBS = 
# Include path
INC = 

# Execute program file grneration
# $@ = TARGET
$(CRONTAB) : $(CRONTAB_OBJS)
	$(CC) -o $@ $(CRONTAB_OBJS) $(LIBS)
$(RSYNC) : $(RSYNC_OBJS)
	$(CC) -o $@ $(RSYNC_OBJS) $(LIBS)

# Object file generation
$(OBJS): $(HDRS)
	$(CC) $(CFLAGS) $(SRCS)

# Dependency
dep:
	$(CC) -M $(INC) $(SRCS) > .dependency

# make all
all: $(CRONTAB) $(RSYNC)

# make new
new:
	$(MAKE) clean
	$(MAKE)

# make clean
clean:
	rm -rf $(OBJS) $(CRONTAB) $(RSYNC) core

