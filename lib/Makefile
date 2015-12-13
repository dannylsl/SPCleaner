CC=gcc
CFLAG= -Wall -g
TARGETS=log

all:${TARGETS}

SPCLog.o: SPCLog.c SPCLog.h
	${CC} ${CFLAG} -c $^
log: log.c SPCLog.o
	${CC} ${CFLAG} -o $@ $^

.PHONY:clean
clean:
	rm ${TARGETS}
	rm *.o *.gch
