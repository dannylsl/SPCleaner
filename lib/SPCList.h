#ifndef __SPC_LIST_H
#define __SPC_LIST_H

#include <stdio.h>

#define NAME_LEN    128

struct Node {
    struct Node *next;
    char value[NAME_LEN];
};

void SPCList_print(struct Node *list);
void SPCList_Free(struct Node *list);

#endif
