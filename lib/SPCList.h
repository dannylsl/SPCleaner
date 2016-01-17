#ifndef __SPC_LIST_H
#define __SPC_LIST_H

#include <stdio.h>
#include <stdlib.h>

#define NAME_LEN    128

struct Node {
    struct Node *next;
    char key[NAME_LEN];
    char value[NAME_LEN];
};

struct List {
    int len;
    struct Node *head;
    struct Node *tail;
};

struct List *SPCList_append(struct List *list, struct Node* nd);
void SPCList_print(struct List *list);
struct List *SPCList_free(struct List *list);

#endif
