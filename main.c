#include "philosophers.h"

int	create_philosophers(t_philosopher **arr, pthread_t *davy_jones, t_all *all)
{
	int		res;

	while (*arr)
	{
		res = pthread_create(&(*arr)->flow, NULL, it_s_my_life, (void *)(*arr));
		if (res != 0)
			return (ERR);
		arr++;
	}
	res = pthread_create(davy_jones, NULL, find_souls, all);
	if (res != 0)
		return (ERR);
	return (EXIT_SUCCESS);
}

int	join_threads(t_philosopher **arr, pthread_t *davy_jones)
{
	int	res;

	while (*arr)
	{
		res = pthread_join((*arr)->flow, NULL);
		if (res != 0)
			return (ERR);
		arr++;
	}
	res = pthread_join(*davy_jones, NULL);
	if (res != 0)
		return (ERR);
	return (EXIT_SUCCESS);
}

int	clear_all(t_all *all)
{
	int		i;

	i = -1;
	while (++i < (int)all->num)
		if (pthread_mutex_destroy(&all->forks[i]) != 0)
			return (err_msg("mutex destroy error\n", ERR));
	free(all->forks);
	i = -1;
	while (++i < (int)all->num)
		free(all->philosophers[i]);
	free(all->philosophers);
	return (EXIT_SUCCESS);
}

/**
 *
 * @param all
 * @return 0 on success
 */

int	philo_fight(t_all *all)
{
	int		res;

	all->s = get_time(ABSOLUTE);
	res = create_philosophers(all->philosophers, &all->davy_jones, all);
	if (res == ERR)
		return (ERR);
	res = join_threads(all->philosophers, &all->davy_jones);
	if (res == ERR)
		return (ERR);
	if (pthread_mutex_unlock(&all->print) != 0)
		return (err_msg("pthread_mutex_unlock error", ERR));
	clear_all(all);
	return (EXIT_SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_all	all;

	if (argc < 5 || argc > 6)
		return (err_msg("Error number of arguments\n", 1));
	if (init_struct(&all, argv) == ERR)
		return (err_msg("Error value of arguments\n", 1));
	if (philo_fight(&all) == ERR)
		return (err_msg("Simulation failed\n", 1));
	return (0);
}
