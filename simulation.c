#include "philosophers.h"

static int	take_a_meal(t_philosopher *philo)
{
	int		res;

	res = pthread_mutex_lock(&philo->all->forks[philo->left_fork]);
	if (res != 0)
		return (err_msg("pthread_mutex_lock\n", ERR));
	print_msg(get_time(philo->all->s), philo->id, \
				" has taken a fork\n", philo->all);
	res = pthread_mutex_lock(&philo->all->forks[philo->right_fork]);
	if (res != 0)
		return (err_msg("pthread_mutex_lock\n", ERR));
	print_msg(get_time(philo->all->s), philo->id, \
				" has taken a fork\n", philo->all);
	print_msg(get_time(philo->all->s), philo->id, " is eating\n", philo->all);
	philo->deadline = (get_time(philo->all->s) + philo->all->time_to_die);
	m_sleep(philo->all->time_to_eat);
	res = pthread_mutex_unlock(&philo->all->forks[philo->left_fork]);
	if (res != 0)
		return (err_msg("pthread_mutex_unlock\n", ERR));
	res = pthread_mutex_unlock(&philo->all->forks[philo->right_fork]);
	if (res != 0)
		return (err_msg("pthread_mutex_unlock\n", ERR));
	philo->meals_passed++;
	return (EXIT_SUCCESS);
}

static void	count_meals(t_philosopher **p, int *status)
{
	unsigned int	min;
	unsigned int	limit;

	if (*p == NULL)
	{
		*status = STOP;
		return ;
	}
	limit = (*p)->all->meals_number;
	min = (*p)->meals_passed;
	while (*p)
	{
		if ((*p)->meals_passed < min)
			min = (*p)->meals_passed;
		p++;
	}
	if (min < limit)
		*status = RUNNING;
	else
		*status = STOP;
}

static void	check_deadlines(t_philosopher **p, t_all *all)
{
	long long		now;

	while (all->simulator_status == RUNNING && *p)
	{
		now = get_time(all->s);
		if ((*p)->deadline < now)
		{
			print_msg(now, (*p)->id, " died\n", all);
			all->simulator_status = STOP;
		}
		p++;
	}
}

void	*find_souls(void *arg)
{
	t_all			*all;
	int				i;
	t_philosopher	**p;

	all = (t_all *)arg;
	while (all->simulator_status != STOP)
	{
		p = all->philosophers;
		count_meals(p, &all->simulator_status);
		check_deadlines(p, all);
		if (all->simulator_status == STOP)
		{
			pthread_mutex_lock(&all->print);
			i = -1;
			while (++i < (int)all->num)
				if (pthread_mutex_unlock(&all->forks[i]) != 0)
					err_msg_p("pthread_mutex_unlock error\n", NULL);
		}
	}
	return (NULL);
}

void	*it_s_my_life(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2)
		usleep(100);
	while (philo->all->simulator_status != STOP)
	{
		if (take_a_meal(philo) == ERR)
			return (err_msg_p("pthread_mutex_lock\n", NULL));
		print_msg(get_time(philo->all->s), philo->id, \
				" is sleeping\n", philo->all);
		m_sleep(philo->all->time_to_sleep);
		print_msg(get_time(philo->all->s), philo->id, \
				" is thinking\n", philo->all);
	}
	return (NULL);
}
