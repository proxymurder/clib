#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <cstdlib>

int main()
{
    std::srand(time(NULL));

    int n = 10; // <1s -> 10k / 35s -> 100,000 lol
    int m = 0;
    int f[n];
    int *l = (int *)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        l[i] = i;
    while (n > 0)
    {
        int r = std::rand() % n;
        int A[r];
        int B[n - r];

        f[m++] = l[r];

        for (int i = 0; i < r; i++)
            A[i] = l[i];
        for (int i = r + 1; i < n; i++)
            B[n - 1 - i] = l[i];

        l = (int *)realloc(l, --n * sizeof(int));

        for (int i = 0; i < r; i++)
            l[i] = A[i];
        for (int i = r; i < n; i++)
            l[i] = B[n - 1 - i];

        // for (int i = 0; i < n; i++)
        //     std::cout << l[i];
        // std::cout << std::endl;
    }

    std::cout << "popped: ";
    for (auto &fi : f)
        std::cout << fi << ",";
    std::cout << std::endl;

    free(l);
    return 0;
}