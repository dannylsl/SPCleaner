#include "SPCList.h"


struct List *SPCList_append(struct List *list, struct Node* nd) {

    if(list == NULL || list->len == 0) {
        list->head = nd;
        list->tail = nd;
    } else {
        list->tail->next = nd;
        list->tail = list->tail->next;
    }
    (list->len)++;
    return list;
}

void SPCList_print(struct List *list) {
    struct Node *ptr;

    if(list == NULL || list->len == 0) {
        printf("[NULL]\n");
        return;
    }
    ptr = list->head;

    printf("================== SPCList =====================\n");
    while(ptr != NULL) {
        printf("[%s=>[%s]] --> ", ptr->key, ptr->value);
        ptr = ptr->next;
    }
    printf("[NULL]\n");

    return;
}


struct List *SPCList_free(struct List *list) {
    struct Node *ptr;
    if(list == NULL || list->len == 0) {
        return NULL;
    }
    ptr = list->head;

    while(ptr != NULL) {
        ptr = ptr->next;
        printf("list->head->value[%s] free\n",list->head->value);
        free(list->head);
        list->head = ptr;
        (list->len)--;
    }

    free(list);
    list = NULL;
    return list;
}
