#include <stdio.h>

int main(void) {
    int a, b, c, i;
    a = 100;
    b = 200;
    printf("a: %p\nb: %p\n", &a, &b);
    scanf("%d", &i);

    if (i > 0) {
        a = 200;
        b = 300;
    }
    c = a + b;
    printf("a = %d\nb = %d\nc = %d\n", a, b, c);

    return 0;
}
