#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define N 100
#define S 10
#define BREAKCOL i % S != 0 ? "" : "\n"
#define BREAKROW i != n - 1 ? "\t" : "\n\n"

#define T 1
#define TIMEOF(t) gettimeofday(&t, NULL)
#define TSPEED(i, f, callback) \
    TIMEOF(i);                 \
    callback;                  \
    TIMEOF(f);

float dt_ms(struct timeval i, struct timeval f);
void dump(int n, int *a);

void pop(int *n, int l[], int *p);
void poprand(int n, int a[]);

int main()
{
    srand(time(NULL));
    struct timeval ti, tf;

    int n = N;
    int a[n];

    for (int i = 0; i < n; i++)
        a[i] = i;

#if T
    TSPEED(ti, tf, poprand(n, a))
    printf("done: %f ms\n", dt_ms(ti, tf));
#else
    poprand(n, a);
#endif

    if (n <= 1000)
        dump(n, a);
    return 0;
}

float dt_ms(struct timeval i, struct timeval f)
{
    return ((f.tv_sec - i.tv_sec) * 1000000.0f) + ((f.tv_usec - i.tv_usec) / 1000.0f);
}

void dump(int n, int *a)
{
    for (int i = 0; i < n; i++)
        printf("%s%i%s", BREAKCOL, a[i], BREAKROW);
}

void pop(int *n, int l[], int *p)
{
    int r = rand() % *n;
    int R[*n - r];
    *p = l[r];
    for (int i = r; i < *n; i++)
        l[i] = R[*n - i] = l[i + 1];
    l = realloc(l, --*n * sizeof(int));
}
void poprand(int n, int a[])
{
    int *l = calloc(n, sizeof(int));
    int m = 0;
    for (int i = 0; i < n; i++)
        l[i] = a[i];
    while (n > 0)
        pop(&n, l, &a[m++]);
}