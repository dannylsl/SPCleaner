#include "lib/SPCLog.h"
#include "lib/SPCList.h"
#include "lib/INIRead.h"

int traverse(const char *path, struct List *module_list) {

    return 0;
}

int main() {
    struct List *module_list = NULL;
    char path[] = "../SPCleaner";

    SPC_INIT();
    traverse(path, module_list);

    SPC_FREE();
    return 0;
}
