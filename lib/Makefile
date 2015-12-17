CC=gcc
CFLAG= -Wall -g
TARGETS=log
TARGETS=remove

all:${TARGETS}

SPCLog.o: SPCLog.c SPCLog.h
	${CC} ${CFLAG} -c $^
log: log.c SPCLog.o
	${CC} ${CFLAG} -o $@ $^

SPCRemove.o: SPCRemove.c SPCRemove.h
	${CC} ${CFLAG} -c $^
remove: remove.c SPCRemove.o
	${CC} ${CFLAG} -o $@ $^

.PHONY:clean
clean:
	rm ${TARGETS}
	rm *.o *.gch
