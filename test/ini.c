#include "../lib/INIRead.h"

int main() {
    struct List *sectList = NULL;
    struct List *itemList = NULL;
    char filename[] = "../Config.ini";

    sectList = GetSections(filename);
    SPCList_print(sectList);

    printf("----------- ITEMLIST ------------\n");
    itemList = GetItems(filename, sectList->head->value);
    SPCList_print(itemList);

    sectList = SPCList_free(sectList);
    SPCList_print(sectList);

    return 0;
}
