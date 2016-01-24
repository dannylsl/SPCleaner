CC=gcc
CFLAGS= -Wall -g
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))

all:$(OBJS)

SPCLog.o: SPCLog.c SPCLog.h
	$(CC) $(CFLAGS) -c $^
SPCString.o: SPCString.c SPCString.h
	$(CC) $(CFLAGS) -c $^
SPCList.o: SPCList.c SPCList.h
	$(CC) $(CFLAGS) -c $^
INIRead.o: INIRead.c INIRead.h
	$(CC) $(CFLAGS) -c $^


.PHONY:clean
clean:
	rm *.o *.gch
