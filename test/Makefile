CC=gcc
CFLAG=-Wall -g
LIB_PATH=../lib/
MOD_PATH=../module/
TARGETS=log remove ini trim fetch

all:$(TARGETS)

log: log.c $(LIB_PATH)SPCLog.o
	$(CC) $(CFLAG) -o $@ $^
remove: remove.c  $(MOD_PATH)SPCTrim.o $(LIB_PATH)SPCLog.o $(LIB_PATH)SPCString.o
	$(CC) $(CFLAG) -o $@ $^
ini: ini.c $(LIB_PATH)INIRead.o $(LIB_PATH)SPCString.o $(LIB_PATH)SPCList.o
	$(CC) $(CFLAG) -o $@ $^
trim: trim.c $(LIB_PATH)SPCString.o $(LIB_PATH)SPCLog.o
	$(CC) $(CFLAG) -o $@ $^
fetch: fetch.c $(MOD_PATH)SPCFetch.o $(MOD_PATH)SPCTrim.o $(LIB_PATH)SPCLog.o $(LIB_PATH)SPCString.o
	$(CC) $(CFLAG) -o $@ $^

.PHONY:clean
clean:
	rm $(TARGETS)
