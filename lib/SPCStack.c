#include <string.h>
#include "SPCLog.h"
#include "SPCStack.h"


void SPCStack_print(struct Stack *stk) {
    struct Element *ptr;
    printf("Stack size: %d\n", stk->size);
    printf("==============================\n");
    ptr = stk->tail;
    while(ptr != NULL) {
        printf("%s\n",ptr->data);
        ptr = ptr->before;
    }
    printf("\n");
    return;
}


struct Element *SPCStack_new_elem(const char *str) {
    struct Element *elem;
    if(strlen(str) >= DATA_MAX_LEN) {
        printf("Warning:Input string len is longer than DATA_MAX_LEN.\n");
    }
    elem = (struct Element*)malloc(sizeof(struct Element));
    if(elem == NULL) {
        printf("Malloc Failed.");
        exit(EXIT_FAILURE);
    }
    memset(elem, 0x00, sizeof(struct Element));
    strcpy(elem->data, str);
    return elem;
}

struct Stack *SPCStack_init() {
    struct Stack *stk;
    stk = (struct Stack*)malloc(sizeof(struct Stack));
    memset(stk, 0x00, sizeof(struct Stack));

    stk->size = 0;
    stk->head = NULL;
    stk->tail = NULL;
    return stk;
}

struct Element *SPCStack_pop(struct Stack *stk, struct Element *e) {
    struct Element *ptr;
    // The element popped should be independenc from the Stack Object
    // Specified the pointer in element popped to NULL
    strcpy(e->data, stk->tail->data);
    ptr = stk->tail->before;
    e->before = NULL;
    e->after = NULL;
    free(stk->tail);
    stk->tail = ptr;
    stk->size--;
    return e;
}


void SPCStack_push(struct Stack *stk, struct Element *e) {
    if(0 == stk->size) {
        stk->tail = e;
        stk->head = e;
    }else{
        stk->tail->after = e;
        e->before = stk->tail;
        e->after = NULL;
        stk->tail = stk->tail->after;
    }
    stk->size++;
    return;
}


struct Stack *SPCStack_free(struct Stack *stk) {
    struct Element *ptr;
    while(stk->tail != NULL) {
        ptr = stk->tail->before;
        if(NULL == stk->tail) {
            return NULL;
        }else{
            free(stk->tail);
            stk->tail = ptr;
            stk->size--;
        }
    }
    return stk;
}
