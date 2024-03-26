#include<stdio.h>
#ifdef DYNAMIC
    #include<dlfcn.h>
#endif
#include "collatz.h"

int main() {
    int numbers[] = {51, 102, 609, 1024};
    int size = sizeof(numbers) / sizeof(numbers[0]);

#ifdef DYNAMIC
    void *handle = dlopen("./libcollatz.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    int (*test_collatz_convergence)(int, int);
    *(void**) (&test_collatz_convergence) = dlsym(handle, "test_collatz_convergence");

    if (!test_collatz_convergence) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }
#endif

    for (int i = 0; i < size; i++) {
        int iterations = test_collatz_convergence(numbers[i], 1000);
        printf("Number: %d, Iterations: %d\n", numbers[i], iterations);
    }

#ifdef DYNAMIC
    dlclose(handle);
#endif

    return 0;
}