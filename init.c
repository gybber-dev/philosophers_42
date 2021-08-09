#include "philosophers.h"

static void	init_philo(t_philosopher **philo, int i, t_all *all, \
				unsigned int num)
{
	(*philo)->meals_passed = 0;
	(*philo)->id = i;
	(*philo)->deadline = (long long)all->time_to_die;
	(*philo)->left_fork = i;
	(*philo)->right_fork = (i + 1) % num;
	(*philo)->all = all;
}

static t_philosopher	**init_philosophers(unsigned int num, t_all *all)
{
	t_philosopher	**phils;
	int				i;

	phils = (t_philosopher **) malloc((num + 1) * sizeof(t_philosopher *));
	if (!phils)
		return (NULL);
	i = -1;
	while (++i < (int)num)
	{
		phils[i] = (t_philosopher *) malloc(sizeof(t_philosopher));
		if (!phils[i])
		{
			while (--i >= 0)
				free(phils[i]);
			free(phils);
			return (NULL);
		}
		init_philo(&phils[i], i, all, num);
	}
	phils[i] = NULL;
	return (phils);
}

static int	init_table(unsigned int num, pthread_mutex_t **forks, \
				pthread_mutex_t *print)
{
	pthread_mutex_t	*arr;
	int				i;

	arr = (pthread_mutex_t *)malloc(num * sizeof (pthread_mutex_t));
	if (!arr)
		return (ERR);
	i = -1;
	while (++i < (int)num)
	{
		if (pthread_mutex_init(&arr[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&arr[i]);
			return (ft_free(&arr, ERR));
		}
	}
	*forks = arr;
	if (pthread_mutex_init(print, NULL) != 0)
	{
		while (--i >= 0)
			pthread_mutex_destroy(&arr[i]);
		return (ft_free(&arr, ERR));
	}
	return (EXIT_SUCCESS);
}

static int	check_args(char **argv)
{
	while (*argv)
	{
		if (!is_number(*argv))
			return (ERR);
		argv++;
	}
	return (EXIT_SUCCESS);
}

int	init_struct(t_all *all, char *argv[])
{
	memset(all, 0, sizeof(t_all));
	if (check_args(argv) == ERR)
		return (ERR);
	all->simulator_status = RUNNING;
	all->meals_number = -2;
	all->num = ft_atoi(argv[1]);
	all->time_to_die = ft_atoi(argv[2]);
	all->time_to_eat = ft_atoi(argv[3]);
	all->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5] != NULL)
		all->meals_number = ft_atoi(argv[5]);
	if (all->time_to_eat < 0 || all->num < 0 || all->time_to_die < 0 || \
			all->time_to_sleep < 0 || (int)all->meals_number == ERR)
		return (ERR);
	if (init_table(all->num, &all->forks, &all->print) == ERR)
		return (ERR);
	all->philosophers = init_philosophers(all->num, all);
	if (all->philosophers == NULL)
		return (ERR);
	return (EXIT_SUCCESS);
}
