CC=gcc
CFLAG= -Wall -g
TARGETS=log remove fetch

all:${TARGETS}

SPCLog.o: SPCLog.c SPCLog.h
	${CC} ${CFLAG} -c $^
log: log.c SPCLog.o
	${CC} ${CFLAG} -o $@ $^

SPCTrim.o: SPCTrim.c SPCTrim.h
	${CC} ${CFLAG} -c $^
remove: remove.c SPCTrim.o SPCLog.o
	${CC} ${CFLAG} -o $@ $^

SPCFetch.o: SPCFetch.c SPCFetch.h
	${CC} ${CFLAG} -c $^
fetch: fetch.c SPCFetch.o SPCLog.o SPCTrim.o
	${CC} ${CFLAG} -o $@ $^

.PHONY:clean
clean:
	rm *.o *.gch
	rm ${TARGETS}
