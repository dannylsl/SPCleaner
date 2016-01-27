CC=gcc
CFLAGS=-Wall -g
SRCS=$(wildcard *.c)
SOS=$(patsubst %.c, %.so, $(SRCS))
OBJS=$(patsubst %.c, %.o, $(SRCS))

all:$(SOS) $(OBJS)

%.so: %.c %.h
	$(CC) $^ -fPIC -shared $(CFLAGS) -o $@

SPCTrim.o: SPCTrim.c SPCTrim.h
	$(CC) $(CFLAGS) -c $^
SPCFetch.o: SPCFetch.c SPCFetch.h
	$(CC) $(CFLAGS) -c $^
SPCHAnalyser.o: SPCHAnalyser.c SPCHAnalyser.h
	$(CC) $(CFLAGS) -c $^
SPCStatis.o: SPCStatis.c SPCStatis.h
	$(CC) $(CFLAGS) -c $^

.PHONY:clean
clean:
	rm *.so *.o *.gch
