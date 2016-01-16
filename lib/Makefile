CC=gcc
CFLAG= -Wall -g
TARGETS=log remove fetch ini trim

all:${TARGETS}

INIRead.o: INIRead.c INIRead.h
	${CC} ${CFLAG} -c $^
ini: ini.c INIRead.o
	${CC} ${CFLAG} -o $@ $^

SPCLog.o: SPCLog.c SPCLog.h
	${CC} ${CFLAG} -c $^
log: log.c SPCLog.o
	${CC} ${CFLAG} -o $@ $^

SPCString.o: SPCString.c SPCString.h
	${CC} ${CFLAG} -c $^
trim: trim.c SPCString.o SPCLog.o
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
