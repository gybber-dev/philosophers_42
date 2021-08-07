#include "philosophers.h"

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (*str++ != 0)
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s == NULL || fd < 0)
		return ;
	write(fd, s, ft_strlen(s));
}

void		ft_putnbr_fd(int n, int fd)
{
	int		ch;

	if (n > -10 && n < 10)
	{
		if (n < 0)
			write(fd, "-", 1);
	}
	else
		ft_putnbr_fd(n / 10, fd);
	n = (n < 0) ? -(n % 10) : n % 10;
	ch = '0' + n;
	write(fd, &ch, 1);
}

int	err_msg(char *msg, int res)
{
	ft_putstr_fd(msg, 2);
	return (res);
}

char	*err_msg_p(char *msg, char *res)
{
	ft_putstr_fd(msg, 2);
	return (res);
}


/**
 *
 * @return -1 on error
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
	while(get_time(ABSOLUTE) < (ms + res))
	{
		usleep(300);
	}
}

t_philosopher	**init_philosophers(unsigned int num, t_all *all)
{
	t_philosopher **phils;
	unsigned int	i;

	phils = (t_philosopher **) malloc((num + 1) * sizeof(t_philosopher *));
	if (!phils)
		return (NULL);
	i = 0;
	while (i < num)
	{
		phils[i] = (t_philosopher *) malloc(sizeof(t_philosopher));
		if (!phils[i])
		{
			while (--i >= 0)
				free(phils[i]);
			free(phils);
			return (NULL);
		}
		phils[i]->id = i;
		phils[i]->deadline = (long long)all->time_to_die;
		phils[i]->left_fork = i;
		phils[i]->right_fork = (i + 1) % num;
		phils[i]->all = all;
		i++;
	}
	phils[i] = NULL;
	return (phils);
}

int	init_table(unsigned int num, pthread_mutex_t **forks, pthread_mutex_t *print)
{
	pthread_mutex_t	*arr;
	int				i;

	arr = (pthread_mutex_t*)malloc(num * sizeof (pthread_mutex_t));
	if (!arr)
		return (-1);
	i = -1;
	while (++i < (int)num)
		if (pthread_mutex_init(&arr[i], NULL) != 0)
			return -1;  // TODO clear pthreads on error
	*forks = arr;
	if (pthread_mutex_init(print, NULL) != 0)
		return (-1);
	return (0);
}


int				is_number(char *str)
{
	while (*str || (*str == ' ') || (*str == '\t') || (*str == '\n') ||
			(*str == '\v') || (*str == '\f') || (*str == '\r'))
		str++;
	while(*str)
	{
		if (!(ft_isdigit(*str)))
			return 0;
		str++;
	}
	return 1;
}

int	check_args(char **argv)
{
	while (*argv)
	{
		if (!is_number(*argv))
			return (-1);
		argv++;
	}
	return (0);
}

int	init_struct(t_all *all, char *argv[])
{
	memset(all, 0, sizeof(t_all));
	if (check_args(argv) == -1)
		return (-1);
	all->simulator_status = RUNNING;
	all->meals_number = -2;
	all->num = ft_atoi(argv[1]);
	all->time_to_die = ft_atoi(argv[2]);
	all->time_to_eat = ft_atoi(argv[3]);
	all->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5] != NULL)
		all->meals_number = ft_atoi(argv[5]);
	if (all->time_to_eat < 0 || all->num < 0 || all->time_to_die < 0 || \
			all->time_to_sleep < 0 || (int)all->meals_number == -1)
		return (-1);
	if (init_table(all->num, &all->forks, &all->print) == -1)
		return (-1);
	all->philosophers = init_philosophers(all->num, all);
	if (all->philosophers == NULL)
		return (-1);
	return (0);
}

int	print_msg(long long time, int philo, char *msg, t_all *all)
{
	int res;

	if (all->simulator_status != STOP)
	{
		res = pthread_mutex_lock(&all->print);
		if (res != 0)
			return (err_msg("pthread_mutex_lock\n", 1));
		printf("%lld", time);
//		ft_putnbr_fd((int)time, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd(philo + 1, 1);
//		ft_putstr_fd(" dl: ", 1);
//		ft_putnbr_fd(all->philosophers[philo]->deadline, 1);
		ft_putstr_fd(msg, 1);
		res = pthread_mutex_unlock(&all->print);
		if (res != 0)
			return (err_msg("pthread_mutex_unlock\n", 1));
	}
	return (0);
}

int	take_a_meal(t_philosopher *philo)
{
	int		res;

	res = pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
	if (res != 0)
		return (err_msg("pthread_mutex_lock\n", 0));
	print_msg(get_time(philo->all->start_point), philo->id, " has taken a fork\n", philo->all);
	res = pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
	if (res != 0)
		return (err_msg("pthread_mutex_lock\n", 0));
	philo->deadline = get_time(philo->all->start_point) + philo->all->time_to_eat;
	print_msg(get_time(philo->all->start_point), philo->id, " has taken a fork\n", philo->all);
	print_msg(get_time(philo->all->start_point), philo->id, " is eating\n", philo->all);
	m_sleep(philo->all->time_to_eat);
	philo->deadline = (get_time(philo->all->start_point) + philo->all->time_to_die);
	res = pthread_mutex_unlock(&philo->all->forks[philo->left_fork]);
	if (res != 0)
		return (err_msg("pthread_mutex_unlock\n", 0));
	res = pthread_mutex_unlock(&philo->all->forks[philo->right_fork]);
	if (res != 0)
		return (err_msg("pthread_mutex_unlock\n", 0));
	return (1);
}

void	*it_s_my_life(void *arg)
{
//	long long		now;
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
//	print_msg(philo->id, 1, "\tWAS BORN\n", &philo->all->print);
	if (philo->id % 2)
		usleep(100);
//	now = get_time(philo->all->start_point);
//	philo->deadline = now + (long long)philo->all->time_to_die;
	while(philo->all->simulator_status != STOP)
	{
		if (!take_a_meal(philo))
			return (err_msg_p("pthread_mutex_lock\n", NULL));
		print_msg(get_time(philo->all->start_point), philo->id, " is sleeping\n", philo->all);
		m_sleep(philo->all->time_to_sleep);
		print_msg(get_time(philo->all->start_point), philo->id, " is thinking\n", philo->all);
	}
	ft_putnbr_fd(philo->id, 1);
	ft_putstr_fd(" was exit\n", 1);
	return (NULL);
}

void	*find_souls(void *arg)
{
	t_all			*all;
	t_philosopher	**p;
	long long		now;

	all = (t_all *)arg;
//	print_msg(0, -1, "\tDavy Jones: Where is my souls?\n", &all->print);
//	ft_putstr_fd("Davy Jones: Where is my souls?\n", 1);

	while(all->simulator_status != STOP)
	{
		p = all->philosophers;
		while(*p)
		{
			now = get_time(all->start_point);
			if ((*p)->deadline < now)
			{
				printf("\n\n\n\n\n%d, Do you fear death? [now: %lld, dl: %lld]\n", (*p)->id + 1, now, (*p)->deadline);
				print_msg(now, (*p)->id, " died\n", all);
//
//				pthread_mutex_lock(&all->print);
				all->simulator_status = STOP;
				break;
			}
			p++;
		}

	}
	return (NULL);
}

int	create_philosophers(t_philosopher **arr, pthread_t *davy_jones, t_all *all)
{
	int		res;

	while (*arr)
	{
		res = pthread_create(&(*arr)->flow, NULL, it_s_my_life, (void *)(*arr));
		if (res != 0)
			return 0;
		arr++;
	}
	res = pthread_create(davy_jones, NULL, find_souls, all);
	if (res != 0)
		return 0;
	return (1);
}

int	join_threads(t_philosopher **arr, pthread_t *davy_jones)
{
	int	res;

	while (*arr)
	{
		res = pthread_join((*arr)->flow, NULL);
		if (res != 0)
			return (0);
		arr++;
	}
	res = pthread_join(*davy_jones, NULL);
	if (res != 0)
		return (0);
	return (1);
}


void	philo_fight(t_all *all)
{
	int res;

	all->start_point = get_time(ABSOLUTE);
	create_philosophers(all->philosophers, &all->davy_jones, all);
	res = join_threads(all->philosophers, &all->davy_jones);
	if (!res)
		ft_putstr_fd("finished by ERROR", 1);
	ft_putstr_fd("finished success", 1);
	// clear mutexes
}

int main(int argc, char *argv[])
{
	t_all all;
	if (argc < 5 || argc > 6)
		return (err_msg("Error number of arguments\n", 1));
	if (init_struct(&all, argv) == -1)
		return (err_msg("Error value of arguments\n", 1));
	philo_fight(&all);
	return (0);
}
