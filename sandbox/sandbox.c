#include "../philosophers.h"

//int main()
//{
//	int num = 5;
//	pthread_mutex_t *arr;
//	arr = (pthread_mutex_t*)malloc(num * sizeof (pthread_mutex_t));
//	for (int i = 0; i < 5; ++i)
//	{
//		if (pthread_mutex_init(&arr[i], NULL) == -1)
//			printf("error\n");
//	}
//	return (0);
//}

//int main()
//{
//	struct timeval t;
//	int res;
//	res = gettimeofday(&t, NULL);
//	printf("res %d\n", res);
//	printf("sec %ld\n", t.tv_sec);
//	printf("usec %d\n", t.tv_usec);
//	printf("msec %ld\n", (t.tv_sec * 1000 + t.tv_usec / 1000));
//
//}