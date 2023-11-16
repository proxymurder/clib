#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define N 10
#define F (10 + N / 10)
#define FORMAT (i % F != 0 ? "" : "\n")
#define BREAK (i != n - 1 ? "," : "\n}\n")
#define T(t) (gettimeofday(&t, NULL))

float dt_ms(struct timeval i, struct timeval f);
void dump(int n, int *a, char *tag);
void pop(int *n, int l[], int *p);
void poprand(int n, int a[]);

// a <array> contains at the end of the algorithm enough information:
//
// From left to right you can read the first ones to have popped out
// and right-most part of <array> a shows the set of remiders or those left out.
//
int main()
{
    srand(time(NULL));
    struct timeval ti, tf;

    int n = N;
    int a[n];

    for (int i = 0; i < n; i++)
        a[i] = i;

    T(ti);
    poprand(n, a);
    T(tf);

    if (n <= 100)
        dump(n, a, "a[n]");
    printf("done: %f ms\n", dt_ms(ti, tf));

    return 0;
}

float dt_ms(struct timeval i, struct timeval f)
{
    return ((f.tv_sec - i.tv_sec) * 1000000.0f) + ((f.tv_usec - i.tv_usec) / 1000.0f);
}

void pop(int *n, int l[], int *p)
{
    int r = rand() % *n;
    int R[*n - r];

    *p = l[r];

    for (int i = r; i < *n; i++)
    {
        R[*n - i] = l[i + 1];
        l[i] = R[*n - i];
    }

    l = realloc(l, --*n * sizeof(int));
}
void poprand(int n, int a[])
{
    int *l = calloc(n, sizeof(int));
    int m = 0;
    for (int i = 0; i < n; i++)
        l[i] = a[i];
    while (n > 0)
    {
        // Uncomment to view how array modifies over time
        // dump(n, l, "");
        pop(&n, l, &a[m++]);
    }
}
void dump(int n, int *a, char *tag)
{
    printf("%s :=> {", tag);
    for (int i = 0; i < n; i++)
        printf("%s%i%s", FORMAT, a[i], BREAK);
}