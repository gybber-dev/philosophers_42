#include "philosophers.h"

int	ft_isdigit(int ch)
{
	int	r;

	r = 1;
	if (!(ch >= 48 && ch <= 57))
		r = 0;
	return (r);
}

int	ft_atoi(const char *str)
{
	int		i;
	long	result;
	int		positive;

	i = 0;
	result = 0;
	positive = 1;
	while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == '\n') ||
	(str[i] == '\v') || (str[i] == '\f') || (str[i] == '\r'))
		i++;
	if (str[i] == '-')
		positive = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		if (((result > 2147483647) && positive) ||
		((result > 2147483648) && !positive))
			return (-1);
		i++;
	}
	if (positive == 0)
		result = result * (-1);
	return ((int)result);
}