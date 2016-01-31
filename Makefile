CC=gcc
CFLAGS=-Wall -g
BIN = SPCleaner
dirs = lib \
	   module \
	   test

MAKE=make

.PHONY: all clean $(dirs)
all: $(dirs) $(BIN)

$(dirs):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(BIN): SPCleaner.c \
		lib/SPCLog.o lib/INIRead.o lib/SPCString.o lib/SPCList.o lib/SPCStack.o
	$(CC) $(CFLAGS) -o $@ $^

clean: $(dirs)
	rm $(BIN)
	rm SPCleaner.log

