#ifndef __SPC_STACK_H
#define __SPC_STACK_H

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "Macros.h"

#define DATA_MAX_LEN 256

struct Element {
    char data[DATA_MAX_LEN];
    struct Element *before;
    struct Element *after;
    DIR *dirp;
};

struct Stack {
    int size;
    struct Element *head;
    struct Element *tail;
};

void SPCStack_print(struct Stack *stk);
char* SPCStack_get_path(struct Stack *stk, char *path);
struct Element *SPCStack_new_elem(const char *str, DIR *dirp);
struct Stack *SPCStack_init();
struct Element *SPCStack_pop(struct Stack *stk, struct Element *e);
void SPCStack_push(struct Stack *stk, struct Element *e);
struct Stack *SPCStack_free(struct Stack *stk);
#endif
