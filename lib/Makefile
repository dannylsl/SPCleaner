CC=gcc
CFLAG= -Wall -g
TARGETS=log remove

all:${TARGETS}

SPCLog.o: SPCLog.c SPCLog.h
	${CC} ${CFLAG} -c $^
log: log.c SPCLog.o
	${CC} ${CFLAG} -o $@ $^

SPCTrim.o: SPCTrim.c SPCTrim.h
	${CC} ${CFLAG} -c $^
remove: remove.c SPCTrim.o SPCLog.o
	${CC} ${CFLAG} -o $@ $^

.PHONY:clean
clean:
	rm *.o *.gch
	rm ${TARGETS}
