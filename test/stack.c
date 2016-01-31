#include <stdlib.h>
#include "../lib/SPCLog.h"
#include "../lib/SPCStack.h"

int main() {
    struct Stack *stk = NULL;
    struct Element *elem;

    SPC_INIT();
    stk = SPCStack_init();

    SPCStack_print(stk);
    elem = SPCStack_new_elem("ELEM1", NULL);
    SPCStack_push(stk, elem);
    elem = SPCStack_new_elem("ELEM2", NULL);
    SPCStack_push(stk, elem);
    elem = SPCStack_new_elem("ELEM3", NULL);
    SPCStack_push(stk, elem);
    elem = SPCStack_new_elem("ELEM4", NULL);
    SPCStack_push(stk, elem);
    elem = SPCStack_new_elem("ELEM5", NULL);
    SPCStack_push(stk, elem);
    SPCStack_print(stk);

    SPCStack_pop(stk, elem);
    SPCStack_pop(stk, elem);
    SPCStack_pop(stk, elem);
    SPCStack_print(stk);

    SPC_FREE();
    return 0;
}
