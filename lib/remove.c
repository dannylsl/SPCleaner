#include <stdio.h>
#include <string.h>
#include "SPCRemove.h"

int main(int argc, char *argv[]) {
    char    *filename = NULL;

    if(argc != 2) {
        printf("parameter error\n");
    }
    filename = argv[1];
    SPC_REMOVE(filename);

    return 0;
}
