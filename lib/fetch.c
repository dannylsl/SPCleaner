#include <stdio.h>
#include <string.h>
#include "SPCFetch.h"

int main(int argc, char *argv[]) {
    int     flag = 0;
    char    *path = NULL;

    if((argc < 2) || (argc > 3)) {
        printf("Input file`s path! and flag\n");
        return -1;
    }

    path = argv[1];
    if(3 == argc) {
     flag = atol(argv[2]);
    }
    SPC_FETCH(path, flag);

    return 0;
}
