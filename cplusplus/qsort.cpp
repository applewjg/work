#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <time.h>
void qsort(int l, int r, int a[])
{
    int x = a[(l + r) /2], i = l, j = r;
    printf("%d %d\n", l, r);
    while (i < j) {
        while (x > a[i]) i ++;
        while (x < a[j]) j --;
        if (i <= j) {
            int y;
            y = a[i], a[i] = a[j], a[j] = y;
            i ++; j --;
        }

    }
   // if ( i == j) i++; 
    if (l < j) qsort(l, j, a);
    if (i < r) qsort(i, r, a);
}
int main() {
    int a[100], n;
    srand(time(NULL));
    for (int i = 0; i < 10; i ++) a[i] = rand() % 200;
    qsort(0, 9, a);
    for (int i = 0; i < 10; i ++) printf("%d ", a[i]);
    printf("\n");
    return 0;
}
