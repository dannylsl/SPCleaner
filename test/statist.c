#include <stdio.h>
#include <string.h>
#include "../module/SPCStatis.h"

int main(int argc, char *argv[]) {
    char    *filename = NULL;

    SPC_INIT();
    if(argc != 2) {
        printf("Input filename!\n");
        return -1;
    }
    filename = argv[1];
    SPC_STATIS(filename);

    return 0;
}
