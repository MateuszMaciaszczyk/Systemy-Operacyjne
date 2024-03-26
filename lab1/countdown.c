#include <stdio.h>

int countdown() {
    for (int i = 10; i > 0; i--) {
        printf("%d\n", i);
    }
    return 0;
}

int main() {
    countdown();
    return 0;
}