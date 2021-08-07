#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

# define ABSOLUTE -1
# define RUNNING 1
# define STOP 0


struct s_all;

typedef struct s_philosopher
{
	unsigned int	id;
	unsigned int	left_fork;
	unsigned int	right_fork;
	long long		deadline;
	pthread_t		flow;
	struct s_all	*all;
}			t_philosopher;

typedef struct s_all
{
	unsigned int	num;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	meals_number;
	long long		start_point;
	pthread_t		davy_jones;
	t_philosopher	**philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	int				simulator_status;
}			t_all;

int			ft_atoi(const char *str);
int			ft_isdigit(int ch);
long long	get_time(long long start_point);

#endif