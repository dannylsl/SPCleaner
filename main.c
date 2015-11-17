#include <stdio.h>

#define INF 1
#define WAN 2
#define ERR 3

void print(int level, const char *msg) {
    switch(level){
        case INF:
            printf("[INF] %s\n",msg);
            break;
        case WAN:
            printf("[WAN] %s\n",msg);
            break;
        case ERR:
            printf("[ERR] %s\n",msg);
            break;
        default:
            break;
    }

    return;
}

int main(int argc, char *argv[]) {

    printf("Hello SPCleaner!\n");
    print(INF, "Hello World!\n");
    print(WAN, "Hello World!\n");
    print(ERR, "Hello World!\n");
    return 0;
}
