#include <string.h>
#include "SPCLog.h"
#include "SPCStack.h"


void SPCStack_print(struct Stack *stk) {
    struct Element *ptr;
    printf("Stack size: %d\n", stk->size);
    printf("==============================\n");
    ptr = stk->tail;
    while(ptr != NULL) {
        printf("%s\t%x\n",ptr->data,ptr->dirp);
        ptr = ptr->before;
    }
    printf("\n");
    return;
}

char* SPCStack_get_path(struct Stack *stk, char *path) {
    struct Element *ptr;
    ptr = stk->head;
    while(ptr != NULL) {
        if(ptr != stk->head) {
            strcat(path,"/");
        }
        strcat(path,ptr->data);
        ptr = ptr->after;
    }
    return path;
}

struct Element *SPCStack_new_elem(const char *str, DIR *dirp) {
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
    elem->dirp = dirp;
    return elem;
}

struct Stack* SPCStack_init() {
    struct Stack *stk;
    stk = (struct Stack*)malloc(sizeof(struct Stack));
    memset(stk, 0x00, sizeof(struct Stack));

    stk->size = 0;
    stk->head = NULL;
    stk->tail = NULL;
    return stk;
}

struct Element* SPCStack_pop(struct Stack *stk, struct Element *e) {
    struct Element *ptr;
    if(stk->size == 0)
        return NULL;

    ptr = stk->tail->before;
    // The element popped should be independenc from the Stack Object
    // Specified the pointer in element popped to NULL
    strcpy(e->data, stk->tail->data);
    e->before = NULL;
    e->after = NULL;
    free(stk->tail);
    stk->tail = ptr;
    if(ptr != NULL) {
        stk->tail->after = NULL;
        stk->size--;
    }
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


struct Stack* SPCStack_free(struct Stack *stk) {
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
