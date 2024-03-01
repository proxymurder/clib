#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define train_count (sizeof(train)/sizeof(train[0]))
#define EPS 1e-3
#define RATE 1e-3
#define N 10000
#define LCOST(W) cost(W, function_linear)
#define FLIMIT(F,X,H) ((F(X+H) - F(X)) / H)

float rand_float(void)
{
	return (float) rand() / (float) RAND_MAX;
}

float function_linear(float a, float b)
{
	return a * b;
}

float train[][2] = {
	{0,0},
	{1,2},
	{2,4},
	{3,6},
	{4,8},
};

float cost(float w, float (*model)(float, float))
{
	float result = 0.0f;

	for (size_t i = 0; i < train_count; ++i) {
		float x = train[i][0];
		float y = model(w, x);
		float d = y - train[i][1];
		result += d*d;
	}

	return result /= train_count;
}

int main (void)
{
	// srand(time(0));
	srand(69);

	float w = rand_float() * 10.0f;
	for (size_t i = 0; i < N; ++i)
		w -= FLIMIT(LCOST, w, EPS) * RATE;

	printf("cost: %f,\tw:%f\n",LCOST(w),w);

	return 0;
}

