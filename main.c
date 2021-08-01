#include "philosophers.h"

t_philosopher	**init_philosophers(unsigned int num)
{
	t_philosopher **phils;
	unsigned int	i;

	phils = (t_philosopher **) malloc(num * sizeof(t_philosopher *));
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
		phils[i]->left_fork = i;
		phils[i]->right_fork = (i + 1) % num;
		i++;
	}
	return (phils);
}

int	init_table(unsigned int num, pthread_mutex_t **forks)
{
	pthread_mutex_t	*arr;
	int				i;

	arr = (pthread_mutex_t*)malloc(num * sizeof (pthread_mutex_t));
	if (!arr)
		return (-1);
	i = -1;
	while (++i < num)
		if (pthread_mutex_init(&arr[i], NULL) != 0)
			return -1;  // TODO clear pthreads on error
	*forks = arr;
	return (0);
}

int	err_msg(char *msg, int res)
{
	printf("%s", msg);
	return (res);
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
	all->meals_number == -2;
	all->num = ft_atoi(argv[1]);
	all->time_to_die = ft_atoi(argv[2]);
	all->time_to_eat = ft_atoi(argv[3]);
	all->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5] != NULL)
		all->meals_number = ft_atoi(argv[5]);
	if (all->time_to_eat < 0 || all->num < 0 || all->time_to_die < 0 || \
			all->time_to_sleep < 0 || all->meals_number == -1)
		return (-1);
	if (init_table(all->num, &all->forks) == -1)
		return (-1);
	all->philosophers = init_philosophers(all->num);
	if (all->philosophers == NULL)
		return (-1);
	return (0);
}

void	philo_fight(t_all *all)
{

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
