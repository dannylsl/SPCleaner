#include <stdio.h>
#include <string.h>
#include "SPCTrim.h"

int main(int argc, char *argv[]) {
    char    *filename = NULL;

    if(argc != 2) {
        printf("Input filename!\n");
    }
    filename = argv[1];
    SPC_TRIM(filename);

    return 0;
}
