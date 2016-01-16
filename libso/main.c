#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void *Solib;
    int (*So)();
    int ret = 0;
    Solib = dlopen("./func1.so", RTLD_LAZY);
    So = dlsym(Solib, "func1");
    (*So)();

    Solib = dlopen("./func2.so", RTLD_LAZY);
    So = dlsym(Solib, "func2");
    ret = (*So)(10, "main.c");
    printf("ret=%d\n",ret);
    return 0;
}
