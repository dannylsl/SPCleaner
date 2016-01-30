#ifndef __SPC_STACK_H
#define __SPC_STACK_H

#include <stdio.h>
#include <stdlib.h>

#define DATA_MAX_LEN 128

struct Element {
    char data[DATA_MAX_LEN];
    struct Element *before;
    struct Element *after;
};

struct Stack {
    int size;
    struct Element *head;
    struct Element *tail;
};

void SPCStack_print(struct Stack *stk);
struct Element *SPCStack_new_elem(const char *str);
struct Stack *SPCStack_init();
struct Element *SPCStack_pop(struct Stack *stk, struct Element *e);
void SPCStack_push(struct Stack *stk, struct Element *e);
struct Stack *SPCStack_free(struct Stack *stk);
#endif
