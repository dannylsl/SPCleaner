CC=gcc
CFLAG= -Wall -g
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))

all:$(OBJS)

SPCLog.o: SPCLog.c SPCLog.h
	$(CC) $(CFLAG) -c $^
SPCString.o: SPCString.c SPCString.h
	$(CC) $(CFLAG) -c $^
SPCList.o: SPCList.c SPCList.h
	$(CC) $(CFLAG) -c $^
INIRead.o: INIRead.c INIRead.h
	$(CC) $(CFLAG) -c $^


.PHONY:clean
clean:
	rm *.o *.gch
