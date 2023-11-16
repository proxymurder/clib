#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void print_results(int n, int *a, const char *tag = "")
{
    printf("(%s) => {", tag);
    for (int i = 0; i < n; i++)
        printf("%i%s", a[i], (i != n - 1 ? "," : "}\n"));
}

void *pop(int &n, int *(&l), int &a)
{
    int r = rand() % n;
    int L[n - r];
    a = l[r];

    for (int i = r + 1; i < n; i++)
        L[n - 1 - i] = l[i];

    l = (int *)realloc(l, --n * sizeof(int));

    for (int i = r; i < n; i++)
        l[i] = L[n - 1 - i];
}

void rpop(int n, int (&a)[])
{
    int *l = (int *)calloc(n, sizeof(int));
    int m = 0;
    for (int i = 0; i < n; i++)
    {
        l[i] = a[i];
        a[i] = 0;
    }
    while (n > 0)
    {
        print_results(n, l, "l");
        pop(n, l, a[m++]);
    }

    free(l);
}

int main()
{
    srand(time(__null));

    int n = 10;
    int a[n];

    for (int i = 0; i < n; i++)
        a[i] = i;

    rpop(n, a);

    if (n <= 100)
        print_results(n, a, "pop");

    return 0;
}