#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>

using std::cout;
using std::endl;

int main()
{
    // srand(time(NULL));

    int n = 10;
    int *Arr_ini = (int *)calloc(n, sizeof(int));
    int N = 100;
    for (int i = 0; i < 5; i++)
        cout << std::rand() % N << " ";
    cout << endl;
    for (int i = 0; i < 5; i++)
        cout << (rand() % (N - 0 + 1)) + 0 << " ";
    for (int i = 0; i < n; i++)
    {
        Arr_ini[i] = i;
        // int *Arr_tmp1 = (int *)calloc(n - 1, sizeof(int));
        // int *Arr_tmp2 = (int *)calloc(n, sizeof(int));

        // cout << Arr_ini[i] << endl;
    }
    free(Arr_ini);
    return 0;
}