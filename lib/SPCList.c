#include "SPCList.h"


void SPCList_print(struct Node *list) {
    struct Node *ptr = list;

    printf("================== SPCList =====================\n");
    while(ptr != NULL) {
        printf("[%s]-->", ptr->value);
        ptr = ptr->next;
    }
    printf("[NULL]\n");

    return;
}

void SPCList_Free(struct Node *list) {
    struct Node *ptr = NULL;

    return;
}

