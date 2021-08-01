#include "../philosophers.h"

int main()
{
	int num = 5;
	pthread_mutex_t *arr;
	arr = (pthread_mutex_t*)malloc(num * sizeof (pthread_mutex_t));
	for (int i = 0; i < 5; ++i)
	{
		if (pthread_mutex_init(&arr[i], NULL) == -1)
			printf("error\n");
	}
	return (0);
}