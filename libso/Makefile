CC=gcc
CFLAGS=-Wall -g -o
TARGETS= func1.so func2.so main

all:$(TARGETS)

func1.so: func.h func1.c
	${CC} $^ -fPIC -shared ${CFLAGS} $@
func2.so: func.h func2.c
	${CC} $^ -fPIC -shared ${CFLAGS} $@
main:main.c
	${CC} $^ ${CFLAGS} $@ -ldl

.PHONY:clean
clean:
	rm main *.so



