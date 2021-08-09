#include "philosophers.h"

/**
** returns absolute time in ms or relative time if start_point is
** defined
** @return -1 on error
*/

long long	get_time(long long start_point)
{
	struct timeval	t;
	long long		res;

	res = gettimeofday(&t, NULL);
	if (res != 0)
		return (err_msg("gettimeofday error\n", -1));
	if (start_point == ABSOLUTE)
		return (t.tv_sec * 1000 + t.tv_usec / 1000);
	return ((t.tv_sec * 1000 + t.tv_usec / 1000) - start_point);
}

void	m_sleep(unsigned int ms)
{
	long long		res;

	res = get_time(ABSOLUTE);
	while (get_time(ABSOLUTE) < (ms + res))
		usleep(300);
}
