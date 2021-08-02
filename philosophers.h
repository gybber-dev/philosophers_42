#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_philosopher
{
	unsigned int	id;
	unsigned int	left_fork;
	unsigned int	right_fork;
	pthread_t		flow;
}			t_philosopher;

typedef struct s_all
{
	unsigned int	num;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	meals_number;
	pthread_t		davy_jones;
	t_philosopher	**philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*start;
}			t_all;

int		ft_atoi(const char *str);
int		ft_isdigit(int ch);
