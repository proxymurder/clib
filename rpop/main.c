#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void print_results(int n, int *a, const char *tag)
{
    printf("(%s) => {", tag);
    for (int i = 0; i < n; i++)
        printf("%i%s", a[i], (i != n - 1 ? "," : "}\n"));
}

void pop(int *n, int *l, int *p)
{
    int r = rand() % *n;
    int L[*n - r];
    *p = l[r];

    for (int i = r + 1; i < *n; i++)
        L[*n - 1 - i] = l[i];

    l = realloc(l, --*n * sizeof(int));

    for (int i = r; i < *n; i++)
        l[i] = L[*n - 1 - i];
}

void rpop(int n, int a[])
{
    int *l = calloc(n, sizeof(int));
    int m = 0;
    for (int i = 0; i < n; i++)
    {
        l[i] = a[i];
        a[i] = 0;
    }
    while (n > 0)
    {
        print_results(n, l, "l");
        pop(&n, l, &a[m++]);
    }
    // free(l);
}

int main()
{
    srand(time(NULL));

    int n = 10;
    int a[n];

    for (int i = 0; i < n; i++)
        a[i] = i;

    rpop(n, a);

    if (n <= 100)
        print_results(n, a, "pop");

    return 0;
}