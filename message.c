#include "philosophers.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (s == NULL || fd < 0)
		return ;
	write(fd, s, ft_strlen(s));
}

void	ft_putnbr_fd(int n, int fd)
{
	int		ch;

	if (n > -10 && n < 10)
	{
		if (n < 0)
			write(fd, "-", 1);
	}
	else
		ft_putnbr_fd(n / 10, fd);
	if (n < 0)
		n = -(n % 10);
	else
		n = n % 10;
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

int	print_msg(long long time, unsigned int philo, char *msg, t_all *all)
{
	int		res;

	if (all->simulator_status != STOP)
	{
		res = pthread_mutex_lock(&all->print);
		if (res != 0)
			return (err_msg("pthread_mutex_lock error\n", 1));
		ft_putnbr_fd((int)time, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd(philo + 1, 1);
		ft_putstr_fd(msg, 1);
		res = pthread_mutex_unlock(&all->print);
		if (res != 0)
			return (err_msg("pthread_mutex_unlock error\n", 1));
	}
	return (0);
}
