CC=gcc
CFLAG= -Wall -g
TEST_PATH=../test/
TARGETS=${TEST_PATH}log ${TEST_PATH}remove ${TEST_PATH}fetch ${TEST_PATH}ini ${TEST_PATH}trim

all:${TARGETS}

INIRead.o: INIRead.c INIRead.h
	${CC} ${CFLAG} -c $^
SPCLog.o: SPCLog.c SPCLog.h
	${CC} ${CFLAG} -c $^
SPCString.o: SPCString.c SPCString.h
	${CC} ${CFLAG} -c $^
SPCTrim.o: SPCTrim.c SPCTrim.h
	${CC} ${CFLAG} -c $^
SPCFetch.o: SPCFetch.c SPCFetch.h
	${CC} ${CFLAG} -c $^
SPC_RTRIM.o: SPC_RTRIM.c SPC_RTRIM.h
	${CC} ${CFLAG} -c $^


${TEST_PATH}log: ${TEST_PATH}log.c SPCLog.o
	${CC} ${CFLAG} -o $@ $^
${TEST_PATH}remove: ${TEST_PATH}remove.c SPCTrim.o SPCLog.o SPC_RTRIM.o
	${CC} ${CFLAG} -o $@ $^
${TEST_PATH}ini: ${TEST_PATH}ini.c INIRead.o SPCString.o
	${CC} ${CFLAG} -o $@ $^
${TEST_PATH}trim: ${TEST_PATH}trim.c SPCString.o SPCLog.o
	${CC} ${CFLAG} -o $@ $^
${TEST_PATH}fetch: ${TEST_PATH}fetch.c SPCFetch.o SPCLog.o SPCTrim.o SPC_RTRIM.o
	${CC} ${CFLAG} -o $@ $^

.PHONY:clean
clean:
	rm *.o *.gch
	rm ${TARGETS}
