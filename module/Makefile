CC=gcc
CFLAGS= -Wall -g
SRCS=$(wildcard *.c)
SOS=$(patsubst %.c, %.so, $(SRCS))
OBJS=$(patsubst %.c, %.o, $(SRCS))

all:$(SOS) $(OBJS)

%.so: %.c %.h
	$(CC) $^ -fPIC -shared $(CFLAGS) -o $@

SPCTrim.o: SPCTrim.c SPCTrim.h
	$(CC) $(CFLAG) -c $^
SPCFetch.o: SPCFetch.c SPCFetch.h
	$(CC) $(CFLAG) -c $^
SPCHAnalyser.o: SPCHAnalyser.c SPCHAnalyser.h
	$(CC) $(CFLAG) -c $^

.PHONY:clean
clean:
	rm *.so *.o
